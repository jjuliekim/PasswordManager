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

void Data::setName(string name) {
    this->name = name;
}

void Data::setPassword(string password) {
    this->password = password;
}

void Data::setWebsite(string website) {
    this->website = website;
}

void Data::setAuthKey(string authKey) {
    this->authKey = authKey;
}