//
// Created by julie on 11/27/2023.
//

#include "../include/PasswordManager.h"
#include "colormod.h"
#include <iostream>

using std::cout, std::endl, std::cin;


void PasswordManager::startup() {
    Color::Modifier def(Color::COLOR_DEFAULT);
    Color::Modifier blue(Color::BLUE);
    cout << blue << "\n==== Personal Password Manager ====" << def << endl;
    cout << "Username: ";
    string username;
    cin >> username;
    cout << "Master Password: ";
    string masterPassword;
    cin >> masterPassword;
}

void PasswordManager::checkJsonFile() {
    jsonManager.findJsonFile();
    jsonManager.load();
    // can call jsonManager.getInfo(); whenever?
    cout << "Loaded" << endl;
}

