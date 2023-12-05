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
    const char *dataLocation = R"(data.json)";
    const char *loginLocation = R"(login.json)";
    map<string, vector<Data>> info;
    map<string, string> loginInfo;
public:
    void findJsonFile();

    map<string, vector<Data>>& getDataInfo();

    void setInfo(map<string, vector<Data>> jsonInfo);

    void loadDataFile();

    void writeDataFile();

    void loadLoginFile();

    void writeLoginFile();

    map<string, string>& getLoginInfo();

    void setLoginInfo(map<string, string> jsonLoginInfo);

    void loadFiles();

    void writeFiles();
};


#endif //PASSWORDMANAGER_JSONMANAGER_H
