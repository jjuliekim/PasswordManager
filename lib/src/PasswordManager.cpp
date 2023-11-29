//
// Created by julie on 11/27/2023.
//

#include "../include/PasswordManager.h"
#include "colormod.h"
#include <iostream>

using std::cout, std::endl, std::cin;
using Color::Modifier;

// log in with username and master password
void PasswordManager::startup() {
    Modifier def(Color::COLOR_DEFAULT);
    Modifier blue(Color::BLUE);
    // Modifier bold(Color::BOLD); combine bold and color modifiers ??
    cout << blue << "\n==== Personal Password Manager ====" << def << endl;
    cout << "Username -> ";
    cin >> username;
    if (jsonManager.getInfo().count(username) == 0) {
        cout << "User does not exist -- making new account for " << username << endl;
        jsonManager.getInfo().insert({username, Data()});
        /*for (auto &element: jsonManager.getInfo()) {
            // first = key
            // second = value
            cout << "account username = " << element.first << endl;
            cout << "printing data contents..." << endl;
            cout << "name = " << element.second.getName() << endl;
            cout << "password = " << element.second.getPassword() << endl;
            cout << "website = " << element.second.getWebsite() << endl;
            cout << "authKey = " << element.second.getAuthKey() << endl;
        }*/
        jsonManager.writeFile();
    }
    cout << "Master Password -> ";
    cin >> masterPassword;
    // checkPassword(); encryption/decryption, validate password
    // if password is correct, then continue to displayMenu(), else exit with "incorrect password" message
    displayMenu();
}

// check if json file exists and load file. If not, create new file
void PasswordManager::checkJsonFile() {
    cout << "Finding json file." << endl;
    jsonManager.findJsonFile();
    jsonManager.load();
    cout << "Loaded json file." << endl;
}

// password manager main menu
void PasswordManager::displayMenu() {
    cout << "\n==== Menu ====" << endl;
    cout << "1. Add new password" << endl;
    cout << "2. View passwords" << endl;
    cout << "3. Exit password manager" << endl;
    cout << "What would you like to do ->";
    int choice;
    cin >> choice;
    while (cin.fail() || choice > 3 || choice < 1) {
        cout << "Input valid option -> ";
        cin >> choice;
    }
    if (choice == 1) {
        addPassword();
    } else if (choice == 2) {
        viewPasswords();
    } else if (choice == 3) {
        cout << "Exiting password manager" << endl;
        exit(0);
    }
}

// add new password data to json file
void PasswordManager::addPassword() {
    cout << "\n==== Add Password ====" << endl;
}

// view all saved passwords
void PasswordManager::viewPasswords() {
    cout << "\n==== View Passwords ====" << endl;
}