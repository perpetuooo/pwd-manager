# include <iostream>
# include <sodium.h>
# include <string>
# include <array>
# include <stdexcept>

# include "crypto/kdf.hpp"
# include "storage/vault.hpp"


int main () {
    if (sodium_init() == -1) return 1;

    std::string mpwd{}, mpwd2{1};
    std::cout << "--- Password Manager ---";

    // Master password not defined.
    if (!Vault::fileExists("vault.enc")) {    // <-- more verification needed
        auto salt = genSalt();

        do {
            std::cout << "\nCreate your master password: ";
            std::cin >> mpwd;
            std::cout << "Confirm your master password: ";
            std::cin >> mpwd2;

            if (mpwd != mpwd2) {
                std::cout << "Passwords dont match, try again.\n";
            }
        } while (mpwd != mpwd2);
        std::cout << "Master password created successfully!\n";

        auto key = deriveKey(mpwd, salt);
        Vault::createVaultFile(salt, key);

    } else {
        auto salt = Vault::loadSalt();
        std::cout << "\nEnter your master password: ";
        std::cin >> mpwd;

        // Verify key authenticity.
        auto key = deriveKey(mpwd, salt);   // <-- Try 3 times before lock up?
        if (!Vault::verifyVaultKey(key)) throw std::runtime_error("Invalid master password...");
    }

    int opt;
    std::string pwd, tag, entry;

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
