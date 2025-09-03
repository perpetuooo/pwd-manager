# include <iostream>
# include <fstream>
# include <filesystem>
# include "storage/vault.hpp"

using namespace std;

fstream Vault::file;

bool Vault::fileExists(const string& filename) {
    return filesystem::exists(filename);
}

void Vault::readFile(const string& filename) {
    string line;

    file.open(filename, ios::in);

    if (!file.is_open()) {
        throw runtime_error("Could not open " + filename);
    }

    cout << "--- Passwords ---\n";
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

void Vault::writeFile(const string& filename, const string& data) {
    file.open(filename, ios::out | ios::app);

    if (!file.is_open()) {
        throw runtime_error("Could not open " + filename);
    }

    file << data << endl;
    file.close();
}
