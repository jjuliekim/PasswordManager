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

// load json files
void PasswordManager::checkJsonFile() {
    jsonManager.findJsonFile();
    jsonManager.loadFiles();
}

// start up page
void PasswordManager::loginWindow() {
    window = SDL_CreateWindow("Password Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // initializing: returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "error initializing SDL: " << SDL_GetError() << endl;
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
                break;
            }
            // if user clicks mouse anywhere
            if (event.type == SDL_MOUSEBUTTONDOWN) {
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

// load image onto window
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
    vector<string> typingImages{"username.bmp", "1Star.bmp", "2Star.bmp", "3Star.bmp", "4Star.bmp", "5Star.bmp",
                                "6Star.bmp", "7Star.bmp", "8Star.bmp", "9Star.bmp", "10Star.bmp"};
    int index = 0;

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    checkUsername();
                    break;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (username.length() > 0) {
                        username.pop_back();
                    }
                    if (index > 0) {
                        index--;
                        string prefix = "images/login/" + typingImages[index];
                        loadImage(prefix.c_str());
                    }
                } else {
                    username += event.key.keysym.sym;
                    if (index < typingImages.size() - 1) {
                        index++;
                        string prefix = "images/login/" + typingImages[index];
                        loadImage(prefix.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    checkUsername();
                    break;
                }
            }
        }
    }
}

// check if username already exists in json
void PasswordManager::checkUsername() {
    sha256 shaHash;
    username = shaHash.absorb(username).hexdigest();
    if (jsonManager.getLoginInfo().count(username) == 0) {
        loadImage("images/login/noUser.bmp");
        SDL_Delay(2000);
        jsonManager.getDataInfo().insert({username, vector<Data>{Data()}});
        firstTime = true;
    }
    jsonManager.writeDataFile();
    enterPassword();
}

// user input password screen
void PasswordManager::enterPassword() {
    loadImage("images/enterPW/password.bmp");
    vector<string> typingImages{"password.bmp", "1Star.bmp", "2Star.bmp", "3Star.bmp", "4Star.bmp", "5Star.bmp",
                                "6Star.bmp", "7Star.bmp", "8Star.bmp", "9Star.bmp", "10Star.bmp"};
    int index = 0;

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    checkPassword();
                    break;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (masterPassword.length() > 0) {
                        masterPassword.pop_back();
                    }
                    if (index > 0) {
                        index--;
                        string prefix = "images/enterPW/" + typingImages[index];
                        loadImage(prefix.c_str());
                    }
                } else {
                    masterPassword += event.key.keysym.sym;
                    if (index < typingImages.size() - 1) {
                        index++;
                        string prefix = "images/enterPW/" + typingImages[index];
                        loadImage(prefix.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    checkPassword();
                    break;
                }
            }
        }
    }
}

// validate password
void PasswordManager::checkPassword() {
    sha256 hash;
    masterPassword = hash.absorb(username + masterPassword + username).hexdigest();
    if (firstTime) {
        jsonManager.getLoginInfo().insert({username, masterPassword});
    } else if (masterPassword != jsonManager.getLoginInfo()[username]) {
        loadImage("images/enterPW/noPW.bmp");
        SDL_Delay(2000);
        enterPassword();
        return;
    } else {
        loadImage("images/enterPW/yesPW.bmp");
        SDL_Delay(2000);
    }
    jsonManager.writeLoginFile();
    displayMenu();
}

// display window for main menu
void PasswordManager::displayMenu() {
    loadImage("images/mainMenu.bmp");
    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 55 && x <= 100 && y >= 181 && y <= 226) {
                    viewPasswords();
                    break;
                } else if (x >= 57 && x <= 102 && y >= 279 && y <= 324) {
                    addPassword();
                    break;
                } else if (x >= 57 && x <= 102 && y >= 377 && y <= 422) {
                    generatePassword();
                    break;
                }
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1) {
                    viewPasswords();
                    break;
                } else if (event.key.keysym.sym == SDLK_2) {
                    addPassword();
                    break;
                } else if (event.key.keysym.sym == SDLK_3) {
                    generatePassword();
                    break;
                }
            }
        }
    }
}

#include "colormod.h"

using namespace Color;

Modifier blue(Color::BLUE);
Modifier red(Color::RED);
Modifier green(Color::GREEN);
Modifier bold(Color::BOLD);
Modifier reset(Color::FORMAT_RESET);
Modifier def(Color::COLOR_DEFAULT);

// view all password info
void PasswordManager::viewPasswords() {
    loadImage("images/viewPW/viewMenu.bmp");
    cout << blue << "\n==== View Passwords ====" << def << endl;
    cout << blue << "[0] " << def << "Back to main menu" << endl;
    if (!firstTime) {
        for (int i = 0; i < jsonManager.getDataInfo()[username].size(); i++) {
            cout << blue << "[" << i + 1 << "] " << def << jsonManager.getDataInfo()[username][i].getWebsite() << endl;
        }
    }
    string input;
    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_0 && input.empty()) {
                    displayMenu();
                    break;
                } else if (event.key.keysym.sym < SDLK_1 || event.key.keysym.sym > SDLK_9) {
                    loadImage("images/viewPW/invalidInput.bmp");
                } else if (event.key.keysym.sym != SDLK_RETURN) {
                    input += event.key.keysym.sym;
                } else {
                    loadImage("images/viewPW/validInput.bmp");
                    SDL_Delay(1500);
                    viewOptions(stoi(input) - 1);
                    break;
                }
            }
        }
    }
}

// view options for specific password
void PasswordManager::viewOptions(int index) {
    loadImage("images/viewPW/options.bmp");
    cout << blue << "[Account information for " << jsonManager.getDataInfo()[username][index].getWebsite() << "]"
         << def << endl;
    cout << "Username/Email -> " << jsonManager.getDataInfo()[username][index].getName() << endl;
    cout << "Password -> " << jsonManager.getDataInfo()[username][index].getPassword() << endl;
    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_0) {
                    viewPasswords();
                    break;
                } else if (event.key.keysym.sym == SDLK_1) {
                    editUsername(index);
                    SDL_Delay(1500);
                    viewPasswords();
                    break;
                } else if (event.key.keysym.sym == SDLK_2) {
                    editPassword(index);
                    SDL_Delay(1500);
                    viewPasswords();
                    break;
                } else if (event.key.keysym.sym == SDLK_3) {
                    jsonManager.getDataInfo()[username].erase(jsonManager.getDataInfo()[username].begin() + index);
                    loadImage("images/edit/deleted.bmp");
                    SDL_Delay(1500);
                    viewPasswords();
                    break;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 62 && x <= 106 && y >= 157 && y <= 202) {
                    viewPasswords();
                    break;
                } else if (x >= 62 && x <= 106 && y >= 243 && y <= 288) {
                    editUsername(index);
                    SDL_Delay(1500);
                    viewPasswords();
                    break;
                } else if (x >= 62 && x <= 106 && y >= 326 && y <= 371) {
                    editPassword(index);
                    SDL_Delay(1500);
                    viewPasswords();
                    break;
                } else if (x >= 62 && x <= 107 && y >= 404 && y <= 449) {
                    jsonManager.getDataInfo()[username].erase(jsonManager.getDataInfo()[username].begin() + index);
                    loadImage("images/edit/deleted.bmp");
                    SDL_Delay(1500);
                    viewPasswords();
                    break;
                }
            }
        }
    }
}

// edit username in json file
void PasswordManager::editUsername(int index) {
    loadImage("images/login/username.bmp");
    vector<string> typingImages{"username.bmp", "1Star.bmp", "2Star.bmp", "3Star.bmp", "4Star.bmp", "5Star.bmp",
                                "6Star.bmp", "7Star.bmp", "8Star.bmp", "9Star.bmp", "10Star.bmp"};
    int i = 0;
    string input;

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    loadImage("images/edit/editedName.bmp");
                    jsonManager.getDataInfo()[username][index].setName(input);
                    jsonManager.writeDataFile();
                    SDL_Delay(2000);
                    viewOptions(index);
                    break;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                    }
                    if (i > 0) {
                        i--;
                        string prefix = "images/login/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                } else {
                    input += event.key.keysym.sym;
                    if (i < typingImages.size() - 1) {
                        i++;
                        string prefix = "images/login/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    loadImage("images/edit/editedName.bmp");
                    jsonManager.getDataInfo()[username][index].setName(input);
                    jsonManager.writeDataFile();
                    SDL_Delay(2000);
                    viewOptions(index);
                    break;
                }
            }
        }
    }
}

// edit password in json file
void PasswordManager::editPassword(int index) {
    loadImage("images/enterPW/password.bmp");
    vector<string> typingImages{"password.bmp", "1Star.bmp", "2Star.bmp", "3Star.bmp", "4Star.bmp", "5Star.bmp",
                                "6Star.bmp", "7Star.bmp", "8Star.bmp", "9Star.bmp", "10Star.bmp"};
    int i = 0;
    string input;

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    loadImage("images/edit/editedPass.bmp");
                    jsonManager.getDataInfo()[username][index].setPassword(input);
                    jsonManager.writeDataFile();
                    SDL_Delay(2000);
                    viewOptions(index);
                    break;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                    }
                    if (i > 0) {
                        i--;
                        string prefix = "images/enterPW/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                } else {
                    input += event.key.keysym.sym;
                    if (i < typingImages.size() - 1) {
                        i++;
                        string prefix = "images/enterPW/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    loadImage("images/edit/editedPass.bmp");
                    jsonManager.getDataInfo()[username][index].setName(input);
                    jsonManager.writeDataFile();
                    SDL_Delay(2000);
                    viewOptions(index);
                    break;
                }
            }
        }
    }
}

// add password data to json file
void PasswordManager::addPassword() {
    string website = getWebsiteInput();
    string name = getNameInput();
    string password = getPasswordInput();
    Data data(name, password, website);
    jsonManager.getDataInfo()[username].push_back(data);
    if (firstTime) {
        jsonManager.getDataInfo()[username].erase(jsonManager.getDataInfo()[username].begin());
        firstTime = false;
    }
    jsonManager.writeDataFile();
    loadImage("images/edit/added.bmp");
    SDL_Delay(1500);
    displayMenu();
}

// get user input for new website info
string PasswordManager::getWebsiteInput() {
    string input;
    loadImage("images/enterWebsite/website.bmp");
    vector<string> typingImages{"website.bmp", "1Star.bmp", "2Star.bmp", "3Star.bmp", "4Star.bmp", "5Star.bmp",
                                "6Star.bmp", "7Star.bmp", "8Star.bmp", "9Star.bmp", "10Star.bmp"};
    int i = 0;

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    loadImage("images/add/websiteInputted.bmp");
                    SDL_Delay(1500);
                    return input;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                    }
                    if (i > 0) {
                        i--;
                        string prefix = "images/enterWebsite/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                } else {
                    input += event.key.keysym.sym;
                    if (i < typingImages.size() - 1) {
                        i++;
                        string prefix = "images/enterWebsite/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    loadImage("images/add/websiteInputted.bmp");
                    SDL_Delay(1500);
                    return input;
                }
            }
        }
    }
}

// get user input for new username info
string PasswordManager::getNameInput() {
    string input;
    loadImage("images/login/username.bmp");
    vector<string> typingImages{"username.bmp", "1Star.bmp", "2Star.bmp", "3Star.bmp", "4Star.bmp", "5Star.bmp",
                                "6Star.bmp", "7Star.bmp", "8Star.bmp", "9Star.bmp", "10Star.bmp"};
    int i = 0;

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    loadImage("images/add/nameInputted.bmp");
                    SDL_Delay(1500);
                    return input;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                    }
                    if (i > 0) {
                        i--;
                        string prefix = "images/login/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                } else {
                    input += event.key.keysym.sym;
                    if (i < typingImages.size() - 1) {
                        i++;
                        string prefix = "images/login/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    loadImage("images/add/nameInputted.bmp");
                    SDL_Delay(1500);
                    return input;
                }
            }
        }
    }
}

// get user input for new password info
string PasswordManager::getPasswordInput() {
    string input;
    loadImage("images/enterPW/password.bmp");
    vector<string> typingImages{"password.bmp", "1Star.bmp", "2Star.bmp", "3Star.bmp", "4Star.bmp", "5Star.bmp",
                                "6Star.bmp", "7Star.bmp", "8Star.bmp", "9Star.bmp", "10Star.bmp"};
    int i = 0;

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    loadImage("images/add/passInputted.bmp");
                    SDL_Delay(1500);
                    return input;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                    }
                    if (i > 0) {
                        i--;
                        string prefix = "images/enterPW/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                } else {
                    input += event.key.keysym.sym;
                    if (i < typingImages.size() - 1) {
                        i++;
                        string prefix = "images/enterPW/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    loadImage("images/add/passInputted.bmp");
                    SDL_Delay(1500);
                    return input;
                }
            }
        }
    }
}

// generate a random password for user
void PasswordManager::generatePassword() {
    int pwLength = getPasswordLength();
    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    loadImage("images/add/passInputted.bmp");
                    cout << password << endl;
                    SDL_Delay(1500);
                    break;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                    }
                    if (i > 0) {
                        i--;
                        string prefix = "images/enterPW/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                } else {
                    input += event.key.keysym.sym;
                    if (i < typingImages.size() - 1) {
                        i++;
                        string prefix = "images/enterPW/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    loadImage("images/add/passInputted.bmp");
                    SDL_Delay(1500);
                    break;
                }
            }
        }
    }
}

// get user input for password length
int PasswordManager::getPasswordLength() {
    string input;
    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    loadImage("images/add/passInputted.bmp");
                    SDL_Delay(1500);
                    break;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                    }
                    if (i > 0) {
                        i--;
                        string prefix = "images/enterPW/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                } else {
                    input += event.key.keysym.sym;
                    if (i < typingImages.size() - 1) {
                        i++;
                        string prefix = "images/enterPW/" + typingImages[i];
                        loadImage(prefix.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    loadImage("images/add/passInputted.bmp");
                    SDL_Delay(1500);
                    break;
                }
            }
        }
    }
}
