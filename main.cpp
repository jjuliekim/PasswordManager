#include "lib/include/PasswordManager.h"
#include "lib/digestpp/digestpp.hpp"

using namespace std;
using namespace digestpp;

int main() {
    PasswordManager app;
    app.loadJson();
    app.loginWindow();

    return 0;
}