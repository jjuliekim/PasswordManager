//
// Created by julie on 11/27/2023.
//

#ifndef PASSWORDMANAGER_DATA_H
#define PASSWORDMANAGER_DATA_H

#include <string>
using std::string;

class Data {
private:
    string email;
    string password;
    string website;
    // string authKey;
public:
    Data(string email, string password, string website);
    string getEmail();
    string getPassword();
    string getWebsite();
    // string getAuthKey();
    void setEmail(string email);
    void setPassword(string password);
    void setWebsite(string website);
    // void setAuthKey(string authKey);
};


#endif //PASSWORDMANAGER_DATA_H
