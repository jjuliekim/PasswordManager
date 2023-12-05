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
public:
    Data(string name, string password, string website);

    Data();

    string getName();

    string getPassword();

    string getWebsite();

    void setName(string str);

    void setPassword(string str);

    void setWebsite(string str);
};


#endif //PASSWORDMANAGER_DATA_H
