# include <iostream>
# include <sodium.h>
# include <string>
# include <array>
# include "crypto/crypto.hpp"
# include "storage/vault.hpp"

using namespace std;

int main () {
    if (sodium_init() == -1) return 1;

    int opt;
    string mpwd, pwd, tag, entry;

    cout << "--- Password Manager ---";

    // Master password not defined.
    if (!Vault::fileExists("secrets.txt")) {    // <-- more verification needed
        auto salt = genSalt();

        cout << "Create your master password: ";
        cin >> mpwd;
        auto key = deriveKey(mpwd, salt);

    } else {
        // auto salt = Vault::loadFile("secrets.txt");
        cout << "Enter your master password: ";
        cin >> mpwd;

        // try to decrypt vault
        // auto key = deriveKey(mpwd, salt);
    }

    while (1) {
        cout << "\n\n1. Save passwords\n2. Read passwords\n3. Exit\n\n";
        cin >> opt;

        switch (opt) {
            // Save passwords.
            case 1:
            cout << "Enter your tag: ";
            cin >> tag;
            cout << "Enter your password: ";
            cin >> pwd;

            entry = tag + " : " + pwd;
            Vault::writeFile("vault.txt", entry);
            cout << "Password saved successfuly!";
            break;

            // Read passwords.
            case 2:
            Vault::readFile("vault.txt");
            break;

            // Exit.
            case 3:
            cout << "Exiting...";
            return 0;

            default:
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
