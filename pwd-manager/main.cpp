# include <iostream>
# include <fstream>
# include <filesystem>
# include <string>

using namespace std;


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


int main () {
    int opt;
    string pwd, tag, entry;
    FileWriter writer;

    cout << "--- Password Manager ---";
    while (1) {
        cout << "\n\n1. Save passwords\n2. Read passwords\n3. Exit\n\n";
        cin >> opt;

        switch (opt) {
            // Save passwords.
            case 1:
            cout << "Type your tag: ";
            cin >> tag;
            cout << "Type your password: ";
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
            return 0;

            default:
            cout << "Invalid option.\n";
        }
    }

    return 0;
}