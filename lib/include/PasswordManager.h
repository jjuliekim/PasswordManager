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
    void loginWindow();

    void enterUsername();

    void checkUsername(const string& username);

    void loadImage(const char *img);

    void enterPassword();

    void checkPassword(string pw);

    void displayMenu();
};

#endif //PASSWORDMANAGER_PASSWORDMANAGER_H
