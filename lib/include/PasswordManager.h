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

    void checkUsername();

    void loadImage(const char *img);

    void checkPassword();

    void displayMenu();

    void viewPasswords();

    void addPassword();

    void generatePassword();

    void viewOptions(int index);

    int getPasswordLength();

    void login(const string &prefix, string ogImage, string &input);

    void edit(int index, const string &prefix, const string &ogImage);

    string getDataInput(const string &prefix, const string &ogImage);
};

#endif //PASSWORDMANAGER_PASSWORDMANAGER_H
