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
    const char* fileLocation = R"(/mnt/c/Users/julie/CLionProjects/PasswordManager/data.json)";
    ifstream file(fileLocation);
    if (!file) {
        cout << "File was not found" << endl;
        ofstream newFile(fileLocation);
        newFile << "{}";
        newFile.close();
    }
    cout << "File was found" << endl;
    FILE *fp = fopen(fileLocation, "rb");
    char buffer[65536];
    FileReadStream is(fp, buffer, sizeof(buffer));
    Document doc;
    doc.ParseStream(is);
    fclose(fp);
}





/*

JsonManager::JsonManager(string loginUser) {
    this->loginUser = loginUser;
    jsonInfo = JsonInfo();
}

JsonInfo JsonManager::getJsonInfo() {
    return jsonInfo;
}

void JsonManager::setJsonInfo(JsonInfo info) {
    this->jsonInfo = info;
}

// load/read file
void JsonManager::load() {

}

// save/write file
void JsonManager::save() {

}*/
