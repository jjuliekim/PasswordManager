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

map<string, Data>& JsonManager::getInfo() {
    return info;
}

void JsonManager::setInfo(map<string, Data> jsonInfo) {
    info = jsonInfo;
}

void JsonManager::load() {
    FILE *file = fopen(location, "rb");
    char buffer[65536];
    FileReadStream is(file, buffer, sizeof(buffer));
    Document doc;
    doc.ParseStream(is);
    if (doc.HasParseError()) {
        cout << "Error parsing json file" << endl;
        exit(1);
    }

    // read through json file and add to map
    for (auto &element: doc.GetArray()) {
        string username = element["username"].GetString();

        const auto &dataObj = element["data"];
        string name = dataObj["name"].GetString();
        string password = dataObj["password"].GetString();
        string website = dataObj["website"].GetString();
        string authKey = dataObj["authKey"].GetString();
        Data data(name, password, website, authKey);

        info.insert({username, data});
    }
/*
    for (auto &element: info) {
        // first = key
        // second = value
        cout << "account username = " << element.first << endl;
        cout << "printing data contents..." << endl;
        cout << "name = " << element.second.getName() << endl;
        cout << "password = " << element.second.getPassword() << endl;
        cout << "website = " << element.second.getWebsite() << endl;
        cout << "authKey = " << element.second.getAuthKey() << endl;
    }*/

}

void JsonManager::writeFile() {
    Document doc;
    doc.SetArray();

    // add data to JSON array from map
    for (auto& element : info ) {
        Value content(kObjectType);
        content.AddMember("username", StringRef(element.first.c_str()), doc.GetAllocator());

        Value data(kObjectType);
        data.AddMember("name", StringRef(element.second.getName().c_str()), doc.GetAllocator());
        data.AddMember("password", StringRef(element.second.getPassword().c_str()), doc.GetAllocator());
        data.AddMember("website", StringRef(element.second.getWebsite().c_str()), doc.GetAllocator());
        data.AddMember("authKey", StringRef(element.second.getAuthKey().c_str()), doc.GetAllocator());

        content.AddMember("data", data, doc.GetAllocator());
        doc.PushBack(content, doc.GetAllocator());
    }

    // save array to file
    FILE *file = fopen(location, "wb");
    char buffer[65536];
    FileWriteStream os(file, buffer, sizeof(buffer));
    Writer<FileWriteStream> writer(os);
    doc.Accept(writer);
    fclose(file);
}