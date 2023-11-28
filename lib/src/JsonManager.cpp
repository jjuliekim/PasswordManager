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
using rapidjson::Document, rapidjson::FileWriteStream, rapidjson::Writer, rapidjson::FileReadStream;
using rapidjson::Value, rapidjson::StringBuffer;

void JsonManager::findJsonFile() {
    ifstream file(location);
    if (!file) {
        cout << "File was not found" << endl;
        ofstream newFile(location);
        newFile << "{}";
        newFile.close();
    } else {
        cout << "File was found" << endl;
    }
}

map<string, Data> JsonManager::getInfo() {
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
        const auto& dataObj = element["data"];
        string name = dataObj["name"].GetString();
        string password = dataObj["password"].GetString();
        string website = dataObj["website"].GetString();
        string authKey = dataObj["authKey"].GetString();
        Data data(name, password, website, authKey);
        info.insert({username, data});
    }

    // testing: print map contents
        for (auto& element : info) {
        // first = key
        // second = value
        cout << "account username = " << element.first << endl;
        cout << "printing data contents..." << endl;
        cout << "name = " << element.second.getName() << endl;
        cout << "password = " << element.second.getPassword() << endl;
        cout << "website = " << element.second.getWebsite() << endl;
        cout << "authKey = " << element.second.getAuthKey() << endl;
    }
}


/*

void JsonManager::save() {
    Document doc;
    doc.SetObject();
    for (auto& map : info) {
        string username = map.first;
        Data data = map.second;
        doc.AddMember(rapidjson::Value(username.c_str(), doc.GetAllocator()).Move(), rapidjson::Value().Move(), doc.GetAllocator());
        doc[username.c_str()].AddMember("name", rapidjson::Value(data.getName().c_str(), doc.GetAllocator()).Move(), doc.GetAllocator());
        doc[username.c_str()].AddMember("password", rapidjson::Value(data.getPassword().c_str(), doc.GetAllocator()).Move(), doc.GetAllocator());
        doc[username.c_str()].AddMember("website", rapidjson::Value(data.getWebsite().c_str(), doc.GetAllocator()).Move(), doc.GetAllocator());
        doc[username.c_str()].AddMember("authKey", rapidjson::Value(data.getAuthKey().c_str(), doc.GetAllocator()).Move(), doc.GetAllocator());
    }

    FILE *file = fopen(location, "wb");
    char buffer[65536];
    FileWriteStream os(file, buffer, sizeof(buffer));
    Writer<FileWriteStream> writer(os);
    doc.Accept(writer);
    fclose(file);
}
*/
