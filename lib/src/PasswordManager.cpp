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
    cout << blue << "\n==== Personal Password Manager ====" << def << endl;
    cout << "Username: ";
    cin >> username;
    if (jsonManager.getInfo().count(username) == 0) {
        cout << "User does not exist -- making new account" << endl;
    } else {
        cout << "User exists" << endl;
    }
    cout << "Master Password: ";
    cin >> masterPassword;
    // checkPassword(); encryption/decryption, validate password
    // if password is correct, then continue to displayMenu(), else exit with "incorrect password" message
}

void PasswordManager::checkJsonFile() {
    cout << "Finding json file." << endl;
    jsonManager.findJsonFile();
    jsonManager.load();
    cout << "Loaded json file." << endl;
}

void PasswordManager::displayMenu() {
    cout << "\n==== Menu ====" << endl;
}

