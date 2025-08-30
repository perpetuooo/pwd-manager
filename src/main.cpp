# include <iostream>
# include <sodium.h>
# include <string>
# include "files.hpp"

using namespace std;

int main () {
    if (sodium_init() == -1) return 1;

    int opt;
    string mpwd, pwd, tag, entry;
    FileWriter writer;
    
    // Master password not defined.
    // if (!writer.fileExists("secrets.txt")) {
    //     cout << "Create your master password: ";
    //     cin >> mpwd;
    // }


    cout << "--- Password Manager ---";


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
            writer.writeFile("vault.txt", entry);
            cout << "Password saved successfuly!";
            break;

            // Read passwords.
            case 2:
            writer.readFile("vault.txt");
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