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
    Modifier bold(Color::BOLD);
    Modifier reset(Color::FORMAT_RESET);
    // Modifier bold(Color::BOLD); combine bold and color modifiers ??
    cout << blue << "\n==== Personal Password Manager ====" << def << endl;
    cout << "Username -> ";
    cin >> username;
    if (jsonManager.getInfo().count(username) == 0) {
        cout << bold << "[" << username << " ... Signing Up]" << endl;
        jsonManager.getInfo().insert({username, vector<Data>{Data()}});
    } else {
        cout << bold << "[" << username << " ... Logging in]" << endl;
    }
    cout << reset << "Master Password -> ";
    cin >> masterPassword;
    // checkPassword(); encryption/decryption, validate password
    // if password is correct, then continue to displayMenu(), else exit with "incorrect password" message
    displayMenu();
    jsonManager.writeFile();
}

// check if json file exists and load file. If not, create new file
void PasswordManager::checkJsonFile() {
    jsonManager.findJsonFile();
    jsonManager.load();
}

// password manager main menu
void PasswordManager::displayMenu() {
    Modifier blue(Color::BLUE);
    Modifier red(Color::RED);
    Modifier def(Color::COLOR_DEFAULT);
    Modifier bold(Color::BOLD);
    Modifier reset(Color::FORMAT_RESET);
    cout << blue << "\n==== Menu ====" << def << endl;
    cout << bold << "[1] " << reset << "Add new password" << endl;
    cout << bold << "[2] " << reset << "View passwords" << endl;
    cout << bold << "[3] " << reset << "Exit password manager" << endl;
    cout << "\nWhat would you like to do -> ";
    // check valid input
    string input;
    cin >> input;
    while (input != "1" && input != "2" && input != "3") {
        cout << red << "Invalid input, please try again -> " << def;
        cin >> input;
    }
    if (input == "1") {
        addPassword();
    } else if (input == "2") {
        viewPasswords();
    } else if (input == "3") {
        cout << bold << "[Exiting password manager...]" << endl;
    }
}

// add new password data to json file
void PasswordManager::addPassword() {
    Modifier blue(Color::BLUE);
    Modifier green(Color::GREEN);
    Modifier def(Color::COLOR_DEFAULT);
    cout << blue << "\n==== Add Password ====" << def << endl;
    string name, password, website, authKey;
    cout << "Username/Email -> ";
    cin >> name;
    cout << "Password -> ";
    cin >> password;
    cout << "Website/App -> ";
    cin >> website;
    cout << "TOTP Authentication Key (if none, input \"none\") -> ";
    cin >> authKey;
    Data data(name, password, website, authKey);
    jsonManager.getInfo()[username].push_back(data);
    cout << green << "[Password info added successfully]" << def << endl;
    displayMenu();
}

// view all saved passwords
void PasswordManager::viewPasswords() {
    Modifier blue(Color::BLUE);
    Modifier def(Color::COLOR_DEFAULT);
    Modifier red(Color::RED);
    Modifier bold(Color::BOLD);
    Modifier reset(Color::FORMAT_RESET);
    cout << blue << "\n==== View Passwords ====" << def << endl;
    cout << bold << "[0] " << reset << "Back to main menu" << endl;
    for (int i = 0; i < jsonManager.getInfo()[username].size(); i++) {
        cout << bold << "[" << i + 1 << "] " << reset << jsonManager.getInfo()[username][i].getWebsite() << endl;
    }
    cout << "View info for website/app # -> " << endl;
    // check valid input
    string input;
    cin >> input;
    // check that index is an int
    int index;
    try {
        index = stoi(input);
    } catch (invalid_argument &e) { // how to repeat until valid input? do while loop?
        cout << red << "Invalid input, please try again -> " << def;
        cin >> input;
        index = stoi(input);
    }
    while (index < -1 || index > jsonManager.getInfo()[username].size()) {
        cout << red << "Invalid input, please try again -> " << def;
        cin >> input;
        index = stoi(input);
    }
    if (index == 0) {
        displayMenu();
        return;
    }
    cout << blue << "[Account information for " << jsonManager.getInfo()[username][index - 1].getWebsite() << "]" << def << endl;
    cout << "Username/Email -> " << jsonManager.getInfo()[username][index - 1].getName() << endl;
    cout << "Password -> " << jsonManager.getInfo()[username][index - 1].getPassword() << endl;
    cout << "TOTP Authentication Key -> " << jsonManager.getInfo()[username][index - 1].getAuthKey() << endl;
    optionsResult(index);
}

// view options for specific password
void PasswordManager::optionsResult(int index) {
    Modifier blue(Color::BLUE);
    Modifier def(Color::COLOR_DEFAULT);
    Modifier red(Color::RED);
    Modifier green(Color::GREEN);
    Modifier bold(Color::BOLD);
    Modifier reset(Color::FORMAT_RESET);
    string input;
    cout << blue << "\nOptions: " << def << endl;
    cout << bold << "[1] " << reset << "Edit username/email" << endl;
    cout << bold << "[2] " << reset << "Edit password" << endl;
    cout << bold << "[3] " << reset << "Edit TOTP key" << endl;
    cout << bold << "[4] " << reset << "Delete password info" << endl;
    cout << bold << "[5] " << reset << "View all passwords" << endl;
    cout << "-> ";
    cin >> input;
    while (input != "1" && input != "2" && input != "3" && input != "4" && input != "5") {
        cout << red << "Invalid input, please try again -> " << def;
        cin >> input;
    }
    if (input == "1") {
        cout << "New username/email -> ";
        cin >> input;
        jsonManager.getInfo()[username][index - 1].setName(input);
        cout << green << "[Username/email changed successfully]" << def << endl;
        optionsResult(index);
    } else if (input == "2") {
        cout << "New password -> ";
        cin >> input;
        jsonManager.getInfo()[username][index - 1].setPassword(input);
        cout << green << "[Password changed successfully]" << def << endl;
        optionsResult(index);
    } else if (input == "3") {
        cout << "New TOTP key -> ";
        cin >> input;
        jsonManager.getInfo()[username][index - 1].setAuthKey(input);
        cout << green << "[TOTP key changed successfully]" << def << endl;
        optionsResult(index);
    } else if (input == "4") {
        jsonManager.getInfo()[username].erase(jsonManager.getInfo()[username].begin() + index - 1);
        cout << green << "[Password info deleted successfully]" << def << endl;
        optionsResult(index);
    } else if (input == "5") {
        viewPasswords();
    }
}