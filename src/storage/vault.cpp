# include <iostream>
# include <fstream>
# include <filesystem>
# include <string>
# include <array>
# include <sodium.h>

# include "storage/vault.hpp"
# include "crypto/kdf.hpp"


bool Vault::fileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}


std::array<unsigned char, crypto_pwhash_SALTBYTES> Vault::genSalt() {
    std::array<unsigned char, crypto_pwhash_SALTBYTES> salt{};  

    // Generate salt.
    randombytes_buf(salt.data(), salt.size());
    std::cout << "\ngenerated salt: " + binToHex(salt.data(), salt.size()) << std::endl;

    // Closed scope to ensure that the stream is closed before futher adjusts.
    {
        std::ofstream out("salt.temp", std::ios::binary | std::ios::trunc);  // Write salt in a temp file first, to avoid corruption in case of errors.
        out.write(reinterpret_cast<const char*>(salt.data()), salt.size());
        if (!out) throw std::runtime_error("Could not create salt.temp");
        out.write(reinterpret_cast<const char*>(salt.data()), static_cast<std::streamsize>(salt.size()));
        if (!out) throw std::runtime_error("Write failed for salt.temp");
        out.flush();
    }

    std::error_code ec;
    std::filesystem::rename("salt.temp", "salt.bin", ec);
    if (ec) {    // Delete .temp if rename failed.
        std::filesystem::remove("salt.temp");
        throw std::runtime_error("Failed to rename temp file: " + ec.message());
    }
    
    return salt;
}


std::array<unsigned char, crypto_pwhash_SALTBYTES> Vault::loadSalt() {
    if (!fileExists("salt.bin")) throw std::runtime_error("Could not find salt.bin");

    std::ifstream in("salt.bin", std::ios::binary);
    if (!in) throw std::runtime_error("Could not open salt.bin");
    
    std::array<unsigned char, crypto_pwhash_SALTBYTES> salt{};
    in.read(reinterpret_cast<char*>(salt.data()), static_cast<std::streamsize>(salt.size()));   // Reads SALTBYTES size into the array.

    std::streamsize readBytes = in.gcount();    // Returns the number of bytes read on the last operation.
    if (readBytes != static_cast<std::streamsize>(salt.size())) {   // File does not contains the same size as the original salt.
        throw std::runtime_error("Invalid salt size in salt.bin (read " + std::to_string(readBytes) + " bytes)");
    }

    return salt;
}


void Vault::readFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Could not open " + filename + " for reading");

    std::cout << "--- Passwords ---\n";
    
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << '\n';
    }
}


void Vault::writeFile(const std::string& filename, const std::string& data) {
    std::ofstream out(filename, std::ios::app);
    if (!out) throw std::runtime_error("Could not open " + filename + " for writing");

    out << data << '\n';
}

