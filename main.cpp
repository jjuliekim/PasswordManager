#include "../include/PasswordManager.h"
#include "../include/MyApp.h"
#include "../digestpp/digestpp.hpp"

using namespace std;
using namespace digestpp;

int main() {
    //PasswordManager manager;
    /*manager.checkJsonFile();
    manager.startup();*/
    /*MyApp app;
    app.OnInit();*/
    string password = "password";
    sha256 hasher;
    cout << password << " = " << hasher.absorb(password).hexdigest() << endl;
    // if loginInfo.json getPassword(at username) == hash(masterPassword) -> login
    // salt with username?
    // how shoj
}