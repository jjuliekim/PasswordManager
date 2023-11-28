#include "PasswordManager.h"

using namespace std;

int main() {
    PasswordManager manager;
    manager.checkJsonFile();
    manager.startup();
}