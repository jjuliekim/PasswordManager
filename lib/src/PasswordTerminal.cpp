//
// Created by julie on 11/27/2023.
//

#include "../include/PasswordTerminal.h"
#include "colormod.h"
#include <iostream>
#include <algorithm>
#include <random>
#include "../digestpp/digestpp.hpp"

using namespace std;
using namespace digestpp;
using Color::Modifier;

// color modifier variables
Modifier blue(Color::BLUE);
Modifier red(Color::RED);
Modifier green(Color::GREEN);
Modifier bold(Color::BOLD);
Modifier reset(Color::FORMAT_RESET);
Modifier def(Color::COLOR_DEFAULT);

// log in with username and master password
void PasswordTerminal::startup() {
    cout << blue << "\n==== Personal Password Manager ====" << def << endl;
    cout << "Username -> ";
    cin >> username;
    sha256 shaHash;
    username = shaHash.absorb(username).hexdigest();
    if (jsonManager.getLoginInfo().count(username) == 0) {
        cout << green << "[Signing Up]" << endl;
        jsonManager.getDataInfo().insert({username, vector<Data>{Data()}});
        firstTime = true;
    } else {
        cout << green << "[Logging in]" << endl;
    }
    cout << def << "Master Password -> ";
    cin >> masterPassword;
    sha256 hasher;
    masterPassword = hasher.absorb(username + masterPassword + username).hexdigest();

    if (firstTime) {
        jsonManager.getLoginInfo().insert({username, masterPassword});
    }
    if (masterPassword != jsonManager.getLoginInfo()[username]) {
        cout << red << "[Incorrect password]" << endl;
        return;
    }
    cout << green << "[Logged in successfully]" << endl;
    displayMenu();
    jsonManager.writeFiles();
}

// check if json file exists and load file. If not, create new file
void PasswordTerminal::checkJsonFile() {
    jsonManager.findJsonFile();
    jsonManager.loadFiles();
}

// password manager main menu
void PasswordTerminal::displayMenu() {
    cout << blue << "\n==== Menu ====" << def << endl;
    cout << bold << "[1] " << reset << "Add new password info" << endl;
    cout << bold << "[2] " << reset << "View passwords" << endl;
    cout << bold << "[3] " << reset << "Generate a password" << endl;
    cout << bold << "[4] " << reset << "Exit password manager" << endl;
    cout << "\nWhat would you like to do -> ";
    // check valid input
    string input;
    cin >> input;
    while (input != "1" && input != "2" && input != "3" && input != "4") {
        cout << red << "Invalid input, please try again -> " << def;
        cin >> input;
    }
    if (input == "1") {
        addPassword();
    } else if (input == "2") {
        viewPasswords();
    } else if (input == "3") {
        generatePassword();
    } else if (input == "4") {
        cout << blue << "[Exiting password manager...]" << endl;
    }
}

// add new password data to json file
void PasswordTerminal::addPassword() {
    cout << blue << "\n==== Add Password ====" << def << endl;
    string name, password, website, authKey;
    cout << "Username/Email -> ";
    cin >> name;
    cout << "Password -> ";
    cin >> password;
    cout << "Website/App -> ";
    cin >> website;
    Data data(name, password, website);
    jsonManager.getDataInfo()[username].push_back(data);
    cout << green << "[Password info added successfully]" << def << endl;
    if (firstTime) {
        jsonManager.getDataInfo()[username].erase(jsonManager.getDataInfo()[username].begin());
        firstTime = false;
    }
    displayMenu();
}

// view all saved passwords
void PasswordTerminal::viewPasswords() {
    cout << blue << "\n==== View Passwords ====" << def << endl;
    cout << bold << "[0] " << reset << "Back to main menu" << endl;
    if (!firstTime) {
        for (int i = 0; i < jsonManager.getDataInfo()[username].size(); i++) {
            cout << bold << "[" << i + 1 << "] " << reset << jsonManager.getDataInfo()[username][i].getWebsite() << endl;
        }
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
    while (index < -1 || index > jsonManager.getDataInfo()[username].size()) {
        cout << red << "Invalid input, please try again -> " << def;
        cin >> input;
        index = stoi(input);
    }
    if (index == 0) {
        displayMenu();
        return;
    }
    cout << blue << "[Account information for " << jsonManager.getDataInfo()[username][index - 1].getWebsite() << "]" << def
         << endl;
    cout << "Username/Email -> " << jsonManager.getDataInfo()[username][index - 1].getName() << endl;
    cout << "Password -> " << jsonManager.getDataInfo()[username][index - 1].getPassword() << endl;
    optionsResult(index);
}

// view options for specific password
void PasswordTerminal::optionsResult(int index) {
    string input;
    cout << blue << "\nOptions: " << def << endl;
    cout << bold << "[1] " << reset << "Edit username/email" << endl;
    cout << bold << "[2] " << reset << "Edit password" << endl;
    cout << bold << "[3] " << reset << "Delete password info" << endl;
    cout << bold << "[4] " << reset << "View all passwords" << endl;
    cout << "-> ";
    cin >> input;
    while (input != "1" && input != "2" && input != "3" && input != "4") {
        cout << red << "Invalid input, please try again -> " << def;
        cin >> input;
    }
    if (input == "1") {
        cout << "New username/email -> ";
        cin >> input;
        jsonManager.getDataInfo()[username][index - 1].setName(input);
        cout << green << "[Username/email changed successfully]" << def << endl;
    } else if (input == "2") {
        cout << "New password -> ";
        cin >> input;
        jsonManager.getDataInfo()[username][index - 1].setPassword(input);
        cout << green << "[Password changed successfully]" << def << endl;
    } else if (input == "3") {
        jsonManager.getDataInfo()[username].erase(jsonManager.getDataInfo()[username].begin() + index - 1);
        cout << green << "[Password info deleted successfully]" << def << endl;
    } else if (input == "4") {
        viewPasswords();
        return;
    }
    optionsResult(index);
}

// password generator
void PasswordTerminal::generatePassword() {
    cout << "Password length (> 5) -> ";
    int length;
    cin >> length;
    while (length < 5) {
        cout << "Invalid input, please try again -> ";
        cin >> length;
    }
    // generate password
    string password;
    string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string lowercase = "abcdefghijklmnopqrstuvwxyz";
    string number = "0123456789";
    string symbol = "!@#$%^&*?";
    for (int i = 0; i < length; i++) {
        if (i % 4 == 0) {
            password += lowercase[rand() % lowercase.length()];
        } else if (i % 4 == 1) {
            password += number[rand() % number.length()];
        } else if (i % 4 == 2) {
            password += symbol[rand() % symbol.length()];
        } else {
            password += uppercase[rand() % uppercase.length()];
        }
    }
    // shuffle
    vector<char> passwordVector(password.begin(), password.end());
    shuffle(passwordVector.begin(), passwordVector.end(), mt19937(random_device()()));
    password = string(passwordVector.begin(), passwordVector.end());
    cout << "Generated password: " << password << endl;
    displayMenu();
}
