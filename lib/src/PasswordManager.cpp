//
// Created by julie on 12/3/2023.
//

#include "PasswordManager.h"
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "../digestpp/digestpp.hpp"

using namespace std;
using namespace digestpp;

// start up page
void PasswordManager::loginWindow() {
    window = SDL_CreateWindow("Password Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // initializing: returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return;
    }
    // load image to surface
    SDL_Surface *image = SDL_LoadBMP("images/main.bmp");

    // create a texture
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);

    // update the surface
    SDL_UpdateWindowSurface(window);


    SDL_Rect rect{0, 0, width, height};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            // if user closes window
            if (event.type == SDL_QUIT) {
                cout << "pressed quit" << endl;
                break;
            }
            // if user clicks mouse anywhere
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                cout << "clicked mouse" << endl;
                enterUsername();
                break;
            }
        }
    }
    // destroy and exit
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void PasswordManager::loadImage(const char *img) {
    SDL_Surface *image = SDL_LoadBMP(img);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_UpdateWindowSurface(window);
    SDL_Rect rect{0, 0, width, height};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
}

// log in page
void PasswordManager::enterUsername() {
    loadImage("images/login/username.bmp");
    vector<string> typingImages{"username.bmp", "1Star.bmp", "2Star.bmp", "3Star.bmp",
                                "4Star.bmp", "5Star.bmp", "6Star.bmp", "7Star.bmp",
                                "8Star.bmp", "9Star.bmp", "10Star.bmp"};
    int index = 0;
    string username;

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            // if user closes window
            if (event.type == SDL_QUIT) {
                cout << "pressed quit" << endl;
                break;
            }
            // if user presses a key
            if (event.type == SDL_KEYDOWN) {
                // if user presses enter
                if (event.key.keysym.sym == SDLK_RETURN) {
                    cout << "pressed enter" << endl;
                    checkUsername(username);
                    cout << "username inputted: " << username << endl;
                    break;
                }
                    // if user presses backspace
                else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    cout << "pressed backspace" << endl;
                    // remove last character from username
                    if (username.length() > 0) {
                        username.pop_back();
                    }
                    if (index > 0) {
                        index--;
                        string prefix = "images/login/" + typingImages[index];
                        loadImage(prefix.c_str());
                    }
                }
                    // if user presses any other key
                else {
                    // add character to username
                    cout << "pressed " << event.key.keysym.sym << endl;
                    username += event.key.keysym.sym;
                    if (index < typingImages.size() - 1) {
                        index++;
                        string prefix = "images/login/" + typingImages[index];
                        loadImage(prefix.c_str());
                    }
                }
            }
            // if user clicks mouse
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                cout << "clicked mouse" << endl;
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    cout << "clicked submit" << endl;
                    checkUsername(username);
                    cout << "username inputted: " << username << endl;
                    break;
                }
            }
        }
    }
}


// check if username exists in json
void PasswordManager::checkUsername(const string& name) {
    sha256 shaHash;
    username = shaHash.absorb(name).hexdigest();
    if (jsonManager.getLoginInfo().count(username) == 0) {
        cout << "new account" << endl;
        jsonManager.getDataInfo().insert({username, vector<Data>{Data()}});
        firstTime = true;
    }
    cout << "username: " << username << endl;
    jsonManager.writeDataFile();
    enterPassword();
}

// graphics for entering password screen
void PasswordManager::enterPassword() {
    loadImage("images/enterPW/password.bmp");
    vector<string> typingImages{"password.bmp", "1Star.bmp", "2Star.bmp", "3Star.bmp",
                                "4Star.bmp", "5Star.bmp", "6Star.bmp", "7Star.bmp",
                                "8Star.bmp", "9Star.bmp", "10Star.bmp"};
    int index = 0;
    string password;

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            // if user closes window
            if (event.type == SDL_QUIT) {
                cout << "pressed quit" << endl;
                break;
            }
            // if user presses a key
            if (event.type == SDL_KEYDOWN) {
                // if user presses enter
                if (event.key.keysym.sym == SDLK_RETURN) {
                    cout << "pressed enter" << endl;
                    cout << "password inputted: " << endl;
                    checkPassword(password);
                    break;
                }
                    // if user presses backspace
                else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    cout << "pressed backspace" << endl;
                    // remove last character from username
                    if (password.length() > 0) {
                        password.pop_back();
                    }
                    if (index > 0) {
                        index--;
                        string prefix = "images/enterPW/" + typingImages[index];
                        loadImage(prefix.c_str());
                    }
                }
                    // if user presses any other key
                else {
                    // add character to password
                    cout << "pressed " << event.key.keysym.sym << endl;
                    password += event.key.keysym.sym;
                    if (index < typingImages.size() - 1) {
                        index++;
                        string prefix = "images/enterPW/" + typingImages[index];
                        loadImage(prefix.c_str());
                    }
                }
            }
            // if user clicks mouse
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                cout << "clicked mouse" << endl;
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    cout << "clicked submit" << endl;
                    cout << "password inputted: " << password << endl;
                    checkPassword(password);
                    break;
                }
            }
        }
    }
}

void PasswordManager::checkPassword(string pw) {
    sha256 hash;
    masterPassword = hash.absorb(username + pw + username).hexdigest();
    if (firstTime) {
        cout << "signing up" << endl;
        jsonManager.getLoginInfo().insert({username, masterPassword});
    } else if (masterPassword != jsonManager.getLoginInfo()[username]) {
        cout << "incorrect password" << endl;
        loadImage("images/enterPW/incorrectPW.bmp");
        SDL_Delay(1000);
        enterPassword();
        return;
    } else {
        cout << "correct password! logging in" << endl;
    }
    cout << "masterPassword: " << masterPassword << endl;
    jsonManager.writeLoginFile();
    displayMenu();
}

void PasswordManager::displayMenu() {
    cout << "displaying menu" << endl;
    loadImage("images/mainMenu.bmp");
}

