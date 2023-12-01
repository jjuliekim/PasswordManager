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
        ofstream newFile(location);
        newFile << "[]";
        newFile.close();
    }
}

// load json file into map
void JsonManager::load() {
    // load file
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
}

// write map to json file
void JsonManager::writeFile() {
    Document doc;
    doc.SetArray();
    // add data from map to JSON array
    for (auto &element: info) {
        Value user(kObjectType);
        user.AddMember("username", StringRef(element.first.c_str()), doc.GetAllocator());
        Value dataArray(kArrayType);
        // encrypt values of data vector
        for (auto &data: element.second) {
            Value dataObj(kObjectType);
            dataObj.AddMember("name", Value(StringRef(data.getName().c_str()), doc.GetAllocator()), doc.GetAllocator());
            dataObj.AddMember("password", Value(StringRef(data.getPassword().c_str()), doc.GetAllocator()), doc.GetAllocator());
            dataObj.AddMember("website", Value(StringRef(data.getWebsite().c_str()), doc.GetAllocator()), doc.GetAllocator());
            dataObj.AddMember("authKey", Value(StringRef(data.getAuthKey().c_str()), doc.GetAllocator()), doc.GetAllocator());
            dataArray.PushBack(dataObj, doc.GetAllocator());
        }
        user.AddMember("data", dataArray, doc.GetAllocator());
        doc.PushBack(user, doc.GetAllocator());
    }
    // write to json file
    FILE* file = fopen(location, "wb");
    char buffer[65536];
    FileWriteStream os(file, buffer, sizeof(buffer));
    Writer<FileWriteStream> writer(os);
    doc.Accept(writer);
    fclose(file);
}