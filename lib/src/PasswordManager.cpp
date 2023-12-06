//
// Created by julie on 12/3/2023.
//

#include "PasswordManager.h"
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <random>
#include "../digestpp/digestpp.hpp"
#include "colormod.h"

using namespace std;
using namespace digestpp;
using namespace Color;

Modifier blue(Color::BLUE);
Modifier red(Color::RED);
Modifier green(Color::GREEN);
Modifier bold(Color::BOLD);
Modifier reset(Color::FORMAT_RESET);
Modifier def(Color::COLOR_DEFAULT);

// find and parse json files
void PasswordManager::loadJson() {
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
                login("images/login/", "username.bmp", username);
                break;
            }
        }
    }
    // save and exit
    jsonManager.writeFiles();
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

// check if username already exists in json
void PasswordManager::checkUsername() {
    sha256 shaHash;
    username = shaHash.absorb(username).hexdigest();
    if (jsonManager.getLoginInfo().count(username) == 0) {
        loadImage("images/login/noUser.bmp");
        SDL_Delay(1500);
        jsonManager.getDataInfo().insert({username, vector<Data>{Data()}});
        firstTime = true;
    }
    jsonManager.writeDataFile();
    login("images/enterPW/", "password.bmp", masterPassword);
}

// validate password
void PasswordManager::checkPassword() {
    sha256 hash;
    masterPassword = hash.absorb(username + masterPassword + username).hexdigest();
    if (firstTime) {
        jsonManager.getLoginInfo().insert({username, masterPassword});
    } else if (masterPassword != jsonManager.getLoginInfo()[username]) {
        loadImage("images/enterPW/noPW.bmp");
        masterPassword = "";
        SDL_Delay(1500);
        login("images/enterPW/", "password.bmp", masterPassword);
        return;
    } else {
        loadImage("images/enterPW/yesPW.bmp");
        SDL_Delay(1500);
    }
    jsonManager.writeLoginFile();
    displayMenu();
}

// get user input for username/password log in page
void PasswordManager::login(const string &prefix, string ogImage, string &input) {
    loadImage((prefix + ogImage).c_str());
    vector<string> typingImages{ogImage, "1Star.bmp", "2Star.bmp", "3Star.bmp", "4Star.bmp", "5Star.bmp",
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
                    if (ogImage == "username.bmp") {
                        checkUsername();
                    } else {
                        checkPassword();
                    }
                    break;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                        index--;
                        string fp = prefix + typingImages[index];
                        loadImage(fp.c_str());
                    }
                } else {
                    input += event.key.keysym.sym;
                    if (index < typingImages.size() - 1) {
                        index++;
                        string fp = prefix + typingImages[index];
                        loadImage(fp.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    if (ogImage == "username.bmp") {
                        checkUsername();
                    } else {
                        checkPassword();
                    }
                    break;
                }
            }
        }
    }
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
                }
                if (event.key.keysym.sym == SDLK_RETURN && !input.empty()) {
                    if (stoi(input) > 0 && stoi(input) <= jsonManager.getDataInfo()[username].size()) {
                        loadImage("images/viewPW/validInput.bmp");
                        SDL_Delay(500);
                        viewOptions(stoi(input) - 1);
                        break;
                    } else {
                        loadImage("images/viewPW/invalidInput.bmp");
                        SDL_Delay(1000);
                        loadImage("images/viewPW/viewMenu.bmp");
                        input = "";
                    }
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                    }
                    if (input.length() == 0) {
                        loadImage("images/viewPW/viewMenu.bmp");
                    }
                } else if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
                    loadImage("images/viewPW/validInput.bmp");
                    input += event.key.keysym.sym;
                }
            }
        }
    }
    jsonManager.writeFiles();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

// view options for specific password
void PasswordManager::viewOptions(int index) {
    loadImage("images/viewPW/options.bmp");
    cout << blue << "\n[Account information for " << jsonManager.getDataInfo()[username][index].getWebsite() << "]"
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
                    edit(index, "images/login/", "username.bmp");
                    SDL_Delay(1500);
                    viewPasswords();
                    break;
                } else if (event.key.keysym.sym == SDLK_2) {
                    edit(index, "images/enterPW/", "password.bmp");
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
                    edit(index, "images/login/", "username.bmp");
                    SDL_Delay(1500);
                    viewPasswords();
                    break;
                } else if (x >= 62 && x <= 106 && y >= 326 && y <= 371) {
                    edit(index, "images/enterPW/", "password.bmp");
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

// edit username/password in json file
void PasswordManager::edit(int index, const string &prefix, const string &ogImage) {
    loadImage((prefix + ogImage).c_str());
    vector<string> typingImages{ogImage, "1Star.bmp", "2Star.bmp", "3Star.bmp", "4Star.bmp", "5Star.bmp",
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
                if (event.key.keysym.sym == SDLK_RETURN && input.length() > 0) {
                    if (ogImage == "username.bmp") {
                        loadImage("images/edit/editedName.bmp");
                        jsonManager.getDataInfo()[username][index].setName(input);
                    } else {
                        loadImage("images/edit/editedPass.bmp");
                        jsonManager.getDataInfo()[username][index].setPassword(input);
                    }
                    jsonManager.writeDataFile();
                    SDL_Delay(1500);
                    viewOptions(index);
                    break;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                    }
                    if (i > 0) {
                        i--;
                        string fp = prefix + typingImages[i];
                        loadImage(fp.c_str());
                    }
                } else {
                    input += event.key.keysym.sym;
                    if (i < typingImages.size() - 1) {
                        i++;
                        string fp = prefix + typingImages[i];
                        loadImage(fp.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && input.length() > 0) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    if (ogImage == "username.bmp") {
                        loadImage("images/edit/editedName.bmp");
                        jsonManager.getDataInfo()[username][index].setName(input);
                    } else {
                        loadImage("images/edit/editedPass.bmp");
                        jsonManager.getDataInfo()[username][index].setPassword(input);
                    }
                    jsonManager.writeDataFile();
                    SDL_Delay(1500);
                    viewOptions(index);
                    break;
                }
            }
        }
    }
}

// add password data to json file
void PasswordManager::addPassword() {
    string website = getDataInput("images/enterWebsite/", "website.bmp");
    string name = getDataInput("images/login/", "username.bmp");
    string password = getDataInput("images/enterPW/", "password.bmp");
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

// get user input for new password data
string PasswordManager::getDataInput(const string &prefix, const string &ogImage) {
    string input;
    loadImage((prefix + ogImage).c_str());
    vector<string> typingImages{ogImage, "1Star.bmp", "2Star.bmp", "3Star.bmp", "4Star.bmp", "5Star.bmp",
                                "6Star.bmp", "7Star.bmp", "8Star.bmp", "9Star.bmp", "10Star.bmp"};
    int i = 0;

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN && input.length() > 0) {
                    if (ogImage == "username.bmp") {
                        loadImage("images/add/nameInputted.bmp");
                    } else if (ogImage == "password.bmp") {
                        loadImage("images/add/passInputted.bmp");
                    } else {
                        loadImage("images/add/websiteInputted.bmp");
                    }
                    SDL_Delay(1500);
                    return input;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                    }
                    if (i > 0) {
                        i--;
                        string fp = prefix + typingImages[i];
                        loadImage(fp.c_str());
                    }
                } else {
                    input += event.key.keysym.sym;
                    if (i < typingImages.size() - 1) {
                        i++;
                        string fp = prefix + typingImages[i];
                        loadImage(fp.c_str());
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && input.length() > 0) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    if (ogImage == "username.bmp") {
                        loadImage("images/add/nameInputted.bmp");
                    } else if (ogImage == "password.bmp") {
                        loadImage("images/add/passInputted.bmp");
                    } else {
                        loadImage("images/add/websiteInputted.bmp");
                    }
                    SDL_Delay(1500);
                    return input;
                }
            }
        }
    }
    return input;
}

// generate a random password for user
void PasswordManager::generatePassword() {
    int length = getPasswordLength();
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
    cout << blue << "\n==== Generated password ====" << def << endl;
    cout << password << endl;

    loadImage("images/genPW/pwGen.bmp");
    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 380 && y <= 414) {
                    displayMenu();
                    break;
                }
            }
        }
    }
}

// get user input for password length
int PasswordManager::getPasswordLength() {
    loadImage("images/genPW/pwLength.bmp");
    string input;
    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN && input.length() > 0) {
                    if (stoi(input) > 5) {
                        return stoi(input);
                    } else {
                        loadImage("images/genPW/invalidLength.bmp");
                        input = "";
                    }
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input.length() > 0) {
                        input.pop_back();
                    }
                    if (input.length() == 0) {
                        loadImage("images/genPW/pwLength.bmp");
                    }
                } else if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
                    input += event.key.keysym.sym;
                    loadImage("images/genPW/validLength.bmp");
                } else {
                    loadImage("images/genPW/invalidLength.bmp");
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && input.length() > 0) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 150 && x <= 250 && y >= 290 && y <= 324) {
                    if (stoi(input) > 5) {
                        return stoi(input);
                    } else {
                        loadImage("images/genPW/invalidLength.bmp");
                        SDL_Delay(1500);
                        input = "";
                    }
                }
            }
        }
    }
    return stoi(input);
}