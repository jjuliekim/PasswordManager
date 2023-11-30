//
// Created by julie on 11/27/2023.
//

#include "JsonManager.h"
#include <fstream>
#include <iostream>
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/filewritestream.h"
#include "../rapidjson/filereadstream.h"

using namespace std;
using namespace rapidjson;

// getters and setters
map<string, vector<Data>> &JsonManager::getInfo() {
    return info;
}

void JsonManager::setInfo(map<string, vector<Data>> jsonInfo) {
    info = jsonInfo;
}

// check if json file exists and load file. If not, create new file
void JsonManager::findJsonFile() {
    ifstream file(location);
    if (!file) {
        cout << "File was not found, creating new file" << endl;
        ofstream newFile(location);
        newFile << "[]";
        newFile.close();
    } else {
        cout << "File was found" << endl;
    }
}

// load json file into map
void JsonManager::load() {
    FILE *file = fopen(location, "rb");
    char buffer[65536];
    FileReadStream is(file, buffer, sizeof(buffer));
    Document doc;
    doc.ParseStream(is);
    if (doc.HasParseError()) {
        cout << "Error parsing json file" << endl;
    }

    // read through json file and add to map
    for (auto &element: doc.GetArray()) {
        string username = element["username"].GetString();
        for (auto &dataContainer: element["data"].GetArray()) {
            string name = dataContainer["name"].GetString();
            string password = dataContainer["password"].GetString();
            string website = dataContainer["website"].GetString();
            string authKey = dataContainer["authKey"].GetString();
            Data data(name, password, website, authKey);
            info[username].push_back(data);
        }
    }

    /*  print info map
        for (auto &element: info) {
        cout << "account username = " << element.first << endl;
        cout << "printing data contents..." << endl;
        for (auto &data: element.second) {
            cout << "name = " << data.getName() << endl;
            cout << "password = " << data.getPassword() << endl;
            cout << "website = " << data.getWebsite() << endl;
            cout << "authKey = " << data.getAuthKey() << endl;
        }
    }*/

}

// write map to json file
void JsonManager::writeFile() {
    Document doc;
    doc.SetArray();

    // add data to JSON array from map
    for (auto &element: info) {
        Value user(kObjectType);
        user.AddMember("username", StringRef(element.first.c_str()), doc.GetAllocator());
        Value dataArray(kArrayType);
        for (auto &data: element.second) {
            Value dataObj(kObjectType);
            // deep copy value
            dataObj.AddMember("name", Value(StringRef(data.getName().c_str()), doc.GetAllocator()), doc.GetAllocator());
            dataObj.AddMember("password", Value(StringRef(data.getPassword().c_str()), doc.GetAllocator()), doc.GetAllocator());
            dataObj.AddMember("website", Value(StringRef(data.getWebsite().c_str()), doc.GetAllocator()), doc.GetAllocator());
            dataObj.AddMember("authKey", Value(StringRef(data.getAuthKey().c_str()), doc.GetAllocator()), doc.GetAllocator());
            dataArray.PushBack(dataObj, doc.GetAllocator());
        }
        user.AddMember("data", dataArray, doc.GetAllocator());
        doc.PushBack(user, doc.GetAllocator());
    }

    // save array to file
    FILE *file = fopen(location, "wb");
    char buffer[65536];
    FileWriteStream os(file, buffer, sizeof(buffer));
    Writer<FileWriteStream> writer(os);
    doc.Accept(writer);
    fclose(file);
}