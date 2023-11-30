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
        cout << "[" << username << " ... Signing Up]" << endl;
        jsonManager.getInfo().insert({username, vector<Data>{Data()}});
    } else {
        cout << "[" << username << " ... Logging in]" << endl;
    }
    cout << "Master Password -> ";
    cin >> masterPassword;
    // checkPassword(); encryption/decryption, validate password
    // if password is correct, then continue to displayMenu(), else exit with "incorrect password" message
    displayMenu();
    jsonManager.writeFile();
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
    cout << "\nWhat would you like to do ->";
    // check valid input
    string input;
    cin >> input;
    while (input != "1" && input != "2" && input != "3") {
        cout << "Invalid input, please try again -> ";
        cin >> input;
    }
    if (input == "1") {
        addPassword();
    } else if (input == "2") {
        viewPasswords();
    } else if (input == "3") {
        cout << "[Exiting password manager...]" << endl;
    }
}

// add new password data to json file
void PasswordManager::addPassword() {
    cout << "\n==== Add Password ====" << endl;
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
    cout << "[Password info added successfully]" << endl;
    displayMenu();
}

// view all saved passwords
void PasswordManager::viewPasswords() {
    cout << "\n==== View Passwords ====" << endl;
    for (int i = 0; i < jsonManager.getInfo()[username].size(); i++) {
        cout << "[" << i + 1 << "] " << jsonManager.getInfo()[username][i].getWebsite() << endl;
    }
    cout << "View info for website/app # \n\"0\" to go back to menu \n-> ";
    // check valid input
    string input;
    cin >> input;

    // check that index is an int
    int index;
    try {
        index = stoi(input);
    } catch (invalid_argument &e) { // how to repeat until valid input? do while loop?
        cout << "Invalid input, please try again -> ";
        cin >> input;
        index = stoi(input);
    }
    while (index < -1 || index > jsonManager.getInfo()[username].size()) {
        cout << "Invalid input, please try again -> ";
        cin >> input;
        index = stoi(input);
    }
    if (index == 0) {
        displayMenu();
        return;
    }
    cout << "[Account information for " << jsonManager.getInfo()[username][index - 1].getWebsite() << "]" << endl;
    cout << "Username/Email -> " << jsonManager.getInfo()[username][index - 1].getName() << endl;
    cout << "Password -> " << jsonManager.getInfo()[username][index - 1].getPassword() << endl;
    cout << "TOTP Authentication Key -> " << jsonManager.getInfo()[username][index - 1].getAuthKey() << endl;
    optionsResult(index);
}

// view options for specific password
void PasswordManager::optionsResult(int index) {
    string input;
    cout << "\nOptions: " << endl;
    cout << "1. Edit username/email" << endl;
    cout << "2. Edit password" << endl;
    cout << "3. Edit TOTP key" << endl;
    cout << "4. Delete password info" << endl;
    cout << "5. Main menu" << endl;
    cout << "-> ";
    cin >> input;
    while (input != "1" && input != "2" && input != "3" && input != "4" && input != "5") {
        cout << "Invalid input, please try again -> ";
        cin >> input;
    }
    if (input == "1") {
        cout << "New username/email -> ";
        cin >> input;
        jsonManager.getInfo()[username][index - 1].setName(input);
        cout << "[Username/email changed successfully]" << endl;
        optionsResult(index);
    } else if (input == "2") {
        cout << "New password -> ";
        cin >> input;
        jsonManager.getInfo()[username][index - 1].setPassword(input);
        cout << "[Password changed successfully]" << endl;
        optionsResult(index);
    } else if (input == "3") {
        cout << "New TOTP key -> ";
        cin >> input;
        jsonManager.getInfo()[username][index - 1].setAuthKey(input);
        cout << "[TOTP key changed successfully]" << endl;
        optionsResult(index);
    } else if (input == "4") {
        jsonManager.getInfo()[username].erase(jsonManager.getInfo()[username].begin() + index - 1);
        cout << "[Password info deleted successfully]" << endl;
        optionsResult(index);
    } else if (input == "5") {
        displayMenu();
    }
}