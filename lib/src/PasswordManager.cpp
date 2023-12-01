//
// Created by julie on 11/27/2023.
//

#include "../include/PasswordManager.h"
#include "colormod.h"
#include <iostream>
#include "../cryptopp/cryptlib.h"
#include "../cryptopp/rijndael.h"
#include "../cryptopp/modes.h"
#include "../cryptopp/files.h"
#include "../cryptopp/osrng.h"
#include "../cryptopp/hex.h"
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
using Color::Modifier;

// color modifier variables
Modifier blue(Color::BLUE);
Modifier red(Color::RED);
Modifier green(Color::GREEN);
Modifier bold(Color::BOLD);
Modifier reset(Color::FORMAT_RESET);
Modifier def(Color::COLOR_DEFAULT);

// log in with username and master password
void PasswordManager::startup() {
    cout << blue << "\n==== Personal Password Manager ====" << def << endl;
    cout << "Username -> ";
    cin >> username;
    if (jsonManager.getInfo().count(username) == 0) {
        cout << bold << "[" << username << " ... Signing Up]" << endl;
        jsonManager.getInfo().insert({username, vector<Data>{Data()}});
        firstTime = true;
    } else {
        cout << bold << "[" << username << " ... Logging in]" << endl;
    }
    cout << reset << "Master Password -> ";
    cin >> masterPassword;
    if (!checkPassword()) {
        cout << red << "[Incorrect password]" << endl;
        return;
    }
    cout << green << "[Logged in successfully]" << endl;
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
        cout << bold << "[Exiting password manager...]" << endl;
    }
}

// add new password data to json file
void PasswordManager::addPassword() {
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
    if (firstTime) {
        jsonManager.getInfo()[username].erase(jsonManager.getInfo()[username].begin());
        firstTime = false;
    }
    displayMenu();
}

// view all saved passwords
void PasswordManager::viewPasswords() {
    cout << blue << "\n==== View Passwords ====" << def << endl;
    cout << bold << "[0] " << reset << "Back to main menu" << endl;
    if (!firstTime) {
        for (int i = 0; i < jsonManager.getInfo()[username].size(); i++) {
            cout << bold << "[" << i + 1 << "] " << reset << jsonManager.getInfo()[username][i].getWebsite() << endl;
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
    while (index < -1 || index > jsonManager.getInfo()[username].size()) {
        cout << red << "Invalid input, please try again -> " << def;
        cin >> input;
        index = stoi(input);
    }
    if (index == 0) {
        displayMenu();
        return;
    }
    cout << blue << "[Account information for " << jsonManager.getInfo()[username][index - 1].getWebsite() << "]" << def
         << endl;
    cout << "Username/Email -> " << jsonManager.getInfo()[username][index - 1].getName() << endl;
    cout << "Password -> " << jsonManager.getInfo()[username][index - 1].getPassword() << endl;
    cout << "TOTP Authentication Key -> " << jsonManager.getInfo()[username][index - 1].getAuthKey() << endl;
    optionsResult(index);
}

// view options for specific password
void PasswordManager::optionsResult(int index) {
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
    } else if (input == "2") {
        cout << "New password -> ";
        cin >> input;
        jsonManager.getInfo()[username][index - 1].setPassword(input);
        cout << green << "[Password changed successfully]" << def << endl;
    } else if (input == "3") {
        cout << "New TOTP key -> ";
        cin >> input;
        jsonManager.getInfo()[username][index - 1].setAuthKey(input);
        cout << green << "[TOTP key changed successfully]" << def << endl;
    } else if (input == "4") {
        jsonManager.getInfo()[username].erase(jsonManager.getInfo()[username].begin() + index - 1);
        cout << green << "[Password info deleted successfully]" << def << endl;
    } else if (input == "5") {
        viewPasswords();
        return;
    }
    optionsResult(index);
}

// decrypt json file to check if master password is correct
bool PasswordManager::checkPassword() {
    return decrypt() == masterPassword;
}

// password generator
void PasswordManager::generatePassword() {
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
        cout << "making password: " << password << endl;
    }
    // shuffle
    vector<char> passwordVector(password.begin(), password.end());
    shuffle(passwordVector.begin(), passwordVector.end(), mt19937(random_device()()));
    password = string(passwordVector.begin(), passwordVector.end());
    cout << "Generated password: " << password << endl;
    displayMenu();
}
/*

// encrypt data
void PasswordManager::encrypt() {
    using namespace CryptoPP;

    AutoSeededRandomPool prng;
    HexEncoder encoder(new FileSink(std::cout));

    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    SecByteBlock iv(AES::BLOCKSIZE);

    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());

    std::string plain = "CBC Mode Test";
    std::string cipher, recovered;

    std::cout << "plain text: " << plain << std::endl;

    */
/*********************************\
    \*********************************//*


    try {
        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(key, key.size(), iv);

        StringSource s(plain, true,
                       new StreamTransformationFilter(e,
                                                      new StringSink(cipher)
                       ) // StreamTransformationFilter
        ); // StringSource
    }
    catch (const Exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    */
/*********************************\
    \*********************************//*


    std::cout << "key: ";
    encoder.Put(key, key.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "iv: ";
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "cipher text: ";
    encoder.Put((const CryptoPP::byte *) &cipher[0], cipher.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    */
/*********************************\
    \*********************************//*


    try {
        CBC_Mode<AES>::Decryption d;
        d.SetKeyWithIV(key, key.size(), iv);

        StringSource s(cipher, true,
                       new StreamTransformationFilter(d,
                                                      new StringSink(recovered)
                       ) // StreamTransformationFilter
        ); // StringSource

        std::cout << "recovered text: " << recovered << std::endl;
    }
    catch (const Exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}
*/
string PasswordManager::decrypt() {
    return "j";
}

