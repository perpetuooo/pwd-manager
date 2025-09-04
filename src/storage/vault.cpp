# include <iostream>
# include <fstream>
# include <filesystem>
# include <string>
# include <sodium.h>
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

std::array<unsigned char, crypto_pwhash_SALTBYTES> Vault::loadSalt() {
    if (!fileExists("secrets.txt")) {
        throw std::runtime_error("Could not find secrets.txt");
    }

    file.open("secrets.txt", std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open secrets.txt");
    }

    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    if (buffer.size() != crypto_pwhash_SALTBYTES) {
        throw std::runtime_error("Invalid salt size in secrets.txt");
    }

    std::array<unsigned char, crypto_pwhash_SALTBYTES> salt{};
    std::copy(buffer.begin(), buffer.end(), salt.begin());

    return salt;
}
