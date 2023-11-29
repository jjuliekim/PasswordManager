//
// Created by julie on 11/27/2023.
//

#include "Data.h"

Data::Data(string name, string password, string website, string authKey) {
    this->name = name;
    this->password = password;
    this->website = website;
    this->authKey = authKey;
}

string Data::getName() {
    return name;
}

string Data::getPassword() {
    return password;
}

string Data::getWebsite() {
    return website;
}

string Data::getAuthKey() {
    return authKey;
}

void Data::setName(string str) {
    name = str;
}

void Data::setPassword(string str) {
    password = str;
}

void Data::setWebsite(string str) {
    website = str;
}

void Data::setAuthKey(string str) {
    authKey = str;
}

Data::Data() {
    this->name = "";
    this->password = "";
    this->website = "";
    this->authKey = "";
}
