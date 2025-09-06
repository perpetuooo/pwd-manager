# include <iostream>
# include <fstream>
# include <filesystem>
# include <string>
# include <array>
# include <sodium.h>

# include "storage/vault.hpp"
# include "crypto/kdf.hpp"


// [HEADER]
// ----------------------------------------------------------
// 4 bytes     magic                "VLT1" (multiple vaults?)
// 16 bytes    salt                 crypto_pwhash_SALTBYTES
// 12 bytes    nonce                crypto_aead_chacha20poly1305_NPUBBYTES
// 4 bytes     verifier len         uint32_t
// N bytes     verifier             ...

// [ENTRIES]
// ----------------------------------------------------------
// 12 bytes    nonce                crypto_aead_chacha20poly1305_NPUBBYTES
// 4 bytes     ciphertxt len        uint32_t
// N bytes     ciphertxt            ...


bool Vault::fileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}


void Vault::createVaultFile(const std::array<unsigned char, crypto_pwhash_SALTBYTES>& salt, std::array<unsigned char, crypto_box_SEEDBYTES> key) {
    // Closed scope to ensure that the stream is closed before futher adjusts.
    {
        // Use a temp file first, to avoid corruption in case of errors.
        std::ofstream out("vault.temp", std::ios::binary | std::ios::trunc);

        // Write magic and salt.
        out.write(reinterpret_cast<const char*>(salt.data()), static_cast<std::streamsize>(salt.size())); 

        // Create verifier and encrypt it with the generated key.
        
        if (!out) throw std::runtime_error("Write failed for vault.temp");
        out.flush();
    }

    std::error_code ec;
    std::filesystem::rename("vault.temp", "vault.enc", ec);
    if (ec) {    // Delete .temp if rename failed.
        std::filesystem::remove("vault.temp");
        throw std::runtime_error("Failed to rename temp file: " + ec.message());
    }
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


// static std::vector<unsigned char> genVerifier() {

// }


// void appendEntry() {

// }


// void  loadEntries() {

// }


// void overwriteVault() {

// }


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

