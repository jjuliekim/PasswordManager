//
// Created by julie on 11/27/2023.
//

#ifndef PASSWORDMANAGER_JSONMANAGER_H
#define PASSWORDMANAGER_JSONMANAGER_H

#include <map>
#include <string>
#include "Data.h"

using namespace std;

class JsonManager {
private:
    const char *location = R"(/mnt/c/Users/julie/CLionProjects/PasswordManager/data.json)";
    map<string, Data> info;
public:
    void findJsonFile();

    map<string, Data>& getInfo();

    void setInfo(map<string, Data> jsonInfo);

    void load();

    void writeFile();
};


#endif //PASSWORDMANAGER_JSONMANAGER_H
