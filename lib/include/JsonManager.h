//
// Created by julie on 11/27/2023.
//

#ifndef PASSWORDMANAGER_JSONMANAGER_H
#define PASSWORDMANAGER_JSONMANAGER_H

#include <map>
#include <string>
#include "Data.h"
#include <vector>

using namespace std;
class JsonManager {
private:
    const char *location = R"(data.json)";
    map<string, vector<Data>> info;
public:
    void findJsonFile();

    map<string, vector<Data>>& getInfo();

    void setInfo(map<string, vector<Data>> jsonInfo);

    void load();

    void writeFile();
};


#endif //PASSWORDMANAGER_JSONMANAGER_H
