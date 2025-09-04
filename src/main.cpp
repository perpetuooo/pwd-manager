# include <iostream>
# include <sodium.h>
# include <string>
# include <array>
# include "crypto/crypto.hpp"
# include "storage/vault.hpp"


int main () {
    if (sodium_init() == -1) return 1;

    int opt;
    std::string mpwd, pwd, tag, entry;

    std::cout << "--- Password Manager ---";

    // Master password not defined.
    if (!Vault::fileExists("secrets.txt")) {    // <-- more verification needed
        auto salt = genSalt();

        std::cout << "Create your master password: ";
        std::cin >> mpwd;
        auto key = deriveKey(mpwd, salt);

    } else {
        auto salt = Vault::loadSalt();
        std::cout << "Enter your master password: ";
        std::cin >> mpwd;

        // try to decrypt vault
        auto key = deriveKey(mpwd, salt);
    }

    while (1) {
        std::cout << "\n\n1. Save passwords\n2. Read passwords\n3. Exit\n\n";
        std::cin >> opt;

        switch (opt) {
            // Save passwords.
            case 1:
            std::cout << "Enter your tag: ";
            std::cin >> tag;
            std::cout << "Enter your password: ";
            std::cin >> pwd;

            entry = tag + " : " + pwd;
            Vault::writeFile("vault.txt", entry);
            std::cout << "Password saved successfuly!";
            break;

            // Read passwords.
            case 2:
            Vault::readFile("vault.txt");
            break;

            // Exit.
            case 3:
            std::cout << "Exiting...";
            return 0;

            default:
            std::cout << "Invalid option.\n";
        }
    }

    return 0;
}
