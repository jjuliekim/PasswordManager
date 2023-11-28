//
// Created by julie on 11/27/2023.
//

#include "Data.h"

Data::Data(string email, string password, string website) {
    this->email = email;
    this->password = password;
    this->website = website;
}

string Data::getEmail() {
    return email;
}

string Data::getPassword() {
    return password;
}

string Data::getWebsite() {
    return website;
}

void Data::setEmail(string email) {
    this->email = email;
}

void Data::setPassword(string password) {
    this->password = password;
}

void Data::setWebsite(string website) {
    this->website = website;
}