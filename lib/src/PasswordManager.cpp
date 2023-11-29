//
// Created by julie on 11/27/2023.
//

#include "../include/PasswordManager.h"
#include "colormod.h"
#include <iostream>

using std::cout, std::endl, std::cin;
using Color::Modifier;

void PasswordManager::startup() {
    Modifier def(Color::COLOR_DEFAULT);
    Modifier blue(Color::BLUE);
    // Modifier bold(Color::BOLD); combine bold and color modifiers ??
    cout << blue << "\n==== Personal Password Manager ====" << def << endl;
    cout << "Username: ";
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
        jsonManager.save();
    }
    cout << "Master Password: ";
    cin >> masterPassword;
    // checkPassword(); encryption/decryption, validate password
    // if password is correct, then continue to displayMenu(), else exit with "incorrect password" message
    displayMenu();
}

void PasswordManager::checkJsonFile() {
    cout << "Finding json file." << endl;
    jsonManager.findJsonFile();
    jsonManager.load();
    cout << "Loaded json file." << endl;
}

void PasswordManager::displayMenu() {
    cout << "\n==== Menu ====" << endl;
    cout << "1. Add new password" << endl;
    cout << "2. View passwords" << endl;
    cout << "3. Exit password manager" << endl;
}

