//
// Created by julie on 11/27/2023.
//

#ifndef PASSWORDMANAGER_PASSWORDMANAGER_H
#define PASSWORDMANAGER_PASSWORDMANAGER_H

#include "JsonManager.h"
#include <iostream>

class PasswordManager {
private:
    JsonManager jsonManager;
    string username;
    string masterPassword;
    bool firstTime = false;
public:
    void startup();

    void checkJsonFile();

    void displayMenu();

    void addPassword();

    void viewPasswords();

    void optionsResult(int index);

    void generatePassword();
};


#endif //PASSWORDMANAGER_PASSWORDMANAGER_H
