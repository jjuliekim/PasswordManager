//
// Created by julie on 11/27/2023.
//

#ifndef PASSWORDMANAGER_PASSWORDMANAGER_H
#define PASSWORDMANAGER_PASSWORDMANAGER_H

#include "JsonManager.h"
#include <iostream>

using std::cout, std::endl;

class PasswordManager {
private:
    JsonManager jsonManager;
    string username;
    string masterPassword;
public:
    void startup();

    void checkJsonFile();

    void displayMenu();

    void addPassword();

    void viewPasswords();

    void optionsResult(int index);
};


#endif //PASSWORDMANAGER_PASSWORDMANAGER_H
