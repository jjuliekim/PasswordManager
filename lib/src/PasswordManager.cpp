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
    cout << "Loaded" << endl;
}





/*PasswordManager::PasswordManager(string loginUser) {
    this->loginUser = loginUser;
    jsonManager = JsonManager(loginUser);
}


void PasswordManager::load() {
  // jsonManager = JsonManager();
   jsonManager.load();
   cout << "Loaded" << endl;
}*/




