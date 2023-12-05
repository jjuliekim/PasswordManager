//
// Created by julie on 11/27/2023.
//

#ifndef PASSWORDMANAGER_PASSWORDTERMINAL_H
#define PASSWORDMANAGER_PASSWORDTERMINAL_H

#include "JsonManager.h"
#include <iostream>

// terminal application for password manager
class PasswordTerminal {
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


#endif //PASSWORDMANAGER_PASSWORDTERMINAL_H
