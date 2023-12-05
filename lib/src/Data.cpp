//
// Created by julie on 11/27/2023.
//

#include "Data.h"

Data::Data(string name, string password, string website) {
    this->name = name;
    this->password = password;
    this->website = website;
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

void Data::setName(string str) {
    name = str;
}

void Data::setPassword(string str) {
    password = str;
}

Data::Data() {
    this->name = "";
    this->password = "";
    this->website = "";
}
