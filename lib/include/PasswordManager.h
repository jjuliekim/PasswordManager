//
// Created by julie on 11/27/2023.
//

#ifndef PASSWORDMANAGER_PASSWORDMANAGER_H
#define PASSWORDMANAGER_PASSWORDMANAGER_H

#include "JsonManager.h"

#include <iostream>
using std::cout, std::endl;

class PasswordManager {
private:
    JsonManager jsonManager;
public:
    void startup();

    void checkJsonFile();
};


#endif //PASSWORDMANAGER_PASSWORDMANAGER_H
