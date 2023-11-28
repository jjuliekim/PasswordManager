//
// Created by julie on 11/27/2023.
//

#include "../include/PasswordManager.h"
#include "colormod.h"
#include <iostream>

using std::cout, std::endl, std::cin;


void PasswordManager::startup() {
    Color::Modifier def(Color::COLOR_DEFAULT);
    Color::Modifier reset(Color::FORMAT_RESET);
    Color::Modifier bold(Color::BOLD);
    Color::Modifier blue(Color::BLUE);
    cout << blue << "\nPersonal Password Manager\n" << def << endl;
    cout << bold << "Username: ";
    string username;
    cin >> username;
    cout << "Master Password: " << reset;
    string masterPassword;
    cin >> masterPassword;
    cout << "username = " << username << endl;
    cout << "masterPassword = " << masterPassword << endl;
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




