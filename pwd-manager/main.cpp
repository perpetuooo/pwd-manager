# include <iostream>
# include <fstream>
# include <filesystem>
# include <string>

using namespace std;

// --- simple master password plan --- 
// Master Password + KDF (PBKDF2 with HMAC) = Encryption Key
// Symmetric Cipher (AES) + Encryption Key --> Passwords = Encrypted vault.txt


class FileWriter {
    private:
        fstream file;

        bool fileExists(string& filename) {
            return filesystem::exists(filename);
        }
    
    public:
        void readFile() {
            string line;

            file.open("vault.txt", ios::in);

            if (!file.is_open()) {
                throw runtime_error("Could not open vault.txt");
            }
            
            cout << "--- Passwords ---\n";
            while (getline(file, line)) {
                cout << line << endl;
            }

            file.close();
        }

        void writeFile(string& data) {
            file.open("vault.txt", ios::out | ios::app);

            if (!file.is_open()) {
                throw runtime_error("Could not open vault.txt");
            }

            file << data << endl;
            file.close();
        }
};


void managerLoop() {
    int opt;
    string pwd, tag, entry;
    FileWriter writer;

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
            writer.writeFile(entry);
            cout << "Password saved successfuly!";
            break;

            // Read passwords.
            case 2:
            writer.readFile();
            break;

            // Exit.
            case 3:
            cout << "Exiting...";
            return;

            default:
            cout << "Invalid option.\n";
        }
    }
}


int main () {
    cout << "--- Password Manager ---";
    cout << "Enter your master password: ";

    return 0;
}