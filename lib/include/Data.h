//
// Created by julie on 11/27/2023.
//

#ifndef PASSWORDMANAGER_DATA_H
#define PASSWORDMANAGER_DATA_H

#include <string>

using std::string;

class Data {
private:
    string name;
    string password;
    string website;
    string authKey;
public:
    Data(string name, string password, string website, string authKey);

    Data();

    string getName();

    string getPassword();

    string getWebsite();

    string getAuthKey();

    void setName(string str);

    void setPassword(string str);

    void setWebsite(string str);

    void setAuthKey(string str);
};


#endif //PASSWORDMANAGER_DATA_H
