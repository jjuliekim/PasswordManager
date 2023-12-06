//
// Created by julie on 12/3/2023.
//

#ifndef PASSWORDMANAGER_PASSWORDMANAGER_H
#define PASSWORDMANAGER_PASSWORDMANAGER_H


#include <SDL2/SDL.h>
#include <string>
#include "JsonManager.h"

using namespace std;

class PasswordManager {
private:
    SDL_Window *window{};
    SDL_Renderer *renderer{};
    int width = 400;
    int height = 500;
    JsonManager jsonManager;
    string username;
    string masterPassword;
    bool firstTime = false;
public:
    void loadJson();

    void loginWindow();

    void enterUsername();

    void checkUsername();

    void loadImage(const char *img);

    void enterPassword();

    void checkPassword();

    void displayMenu();

    void viewPasswords();

    void addPassword();

    void generatePassword();

    void viewOptions(int index);

    void editUsername(int index);

    void editPassword(int index);

    string getNameInput();

    string getPasswordInput();

    string getWebsiteInput();

    int getPasswordLength();

    void login(const string& prefix, string ogImage, string &input);
};

#endif //PASSWORDMANAGER_PASSWORDMANAGER_H
