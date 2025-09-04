# include <iostream>
# include <fstream>
# include <filesystem>
# include "storage/vault.hpp"


std::fstream Vault::file;

bool Vault::fileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}

void Vault::readFile(const std::string& filename) {
    std::string line;

    file.open(filename, std::ios::in);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open " + filename);
    }

    std::cout << "--- Passwords ---\n";
    while (getline(file, line)) {
        std::cout << line << std::endl;
    }

    file.close();
}

void Vault::writeFile(const std::string& filename, const std::string& data) {
    file.open(filename, std::ios::out | std::ios::app);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open " + filename);
    }

    file << data << std::endl;
    file.close();
}
