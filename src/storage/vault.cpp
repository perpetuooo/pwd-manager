# include <iostream>
# include <fstream>
# include <filesystem>
# include <stdexcept>
# include <string>
# include <array>
# include <sodium.h>

# include "storage/vault.hpp"
# include "crypto/crypto.hpp"


// [HEADER]
// ----------------------------------------------------------
// 4 bytes     magic                "VLT1" (multiple vaults?)
// 16 bytes    salt                 crypto_pwhash_SALTBYTES
// 12 bytes    nonce                crypto_aead_chacha20poly1305_NPUBBYTES
// 16 bytes    verifier             randombytes_buf

// [ENTRIES]
// ----------------------------------------------------------
// 12 bytes    nonce                crypto_aead_chacha20poly1305_NPUBBYTES
// 4 bytes     ciphertxt len        uint32_t
// N bytes     ciphertxt            encrypted data
// ...         ...                  ...


bool Vault::fileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}


std::fstream accessVaultFile(std::ios::openmode mode, bool temp) {
    std::string fname = temp ? "vault.temp" : "vault.enc";

    mode |= std::ios::binary;   // Always use binary mode.

    if (std::filesystem::exists(fname)) {
        auto perms = std::filesystem::status(fname).permissions();

        if (((mode & std::ios::in) != 0) && ((perms & std::filesystem::perms::owner_read) == std::filesystem::perms::none)) {
            throw std::runtime_error("No read permission for " + fname);
        }

        if (((mode & std::ios::out) != 0) && ((perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none)) {
            throw std::runtime_error("No write permission for " + fname);
        }
    // Reading requires the file to exist.
    } else if ((mode & std::ios::in) != 0) {
        throw std::runtime_error("File does not exist: " + fname);
    }

    std::fstream file(fname, mode);
    if (!file) throw std::runtime_error("Failed to open file " + fname);

    return file;
}


void Vault::createVaultFile(const std::array<unsigned char, crypto_pwhash_SALTBYTES>& salt, std::array<unsigned char, crypto_box_SEEDBYTES>& key) {
    // Closed scope to ensure that the stream is closed before futher adjusts.
    {
        // Use a temp file first, to avoid corruption in case of errors.
        auto out = accessVaultFile(std::ios::out | std::ios::trunc, true);
        
        // Write magic and salt.
        out.write("VLT1", 4);
        out.write(reinterpret_cast<const char*>(salt.data()), static_cast<std::streamsize>(salt.size())); 
        
        // Create verifier and encrypt it with the generated key.
        std::array<unsigned char, 16> verifier{};
        randombytes_buf(verifier.data(), verifier.size());
        auto block = encrypt(verifier.data(), verifier.size(), key);

        out.write(reinterpret_cast<const char*>(block.nonce.data()), static_cast<std::streamsize>(block.nonce.size()));
        out.write(reinterpret_cast<const char*>(block.ciphertext.data()), static_cast<std::streamsize>(block.ciphertext.size()));  
        out.flush();
    }

    std::error_code ec;
    std::filesystem::rename("vault.temp", "vault.enc", ec);
    if (ec) {    // Delete .temp if rename failed.
        std::filesystem::remove("vault.temp");
        throw std::runtime_error("Failed to rename temp file: " + ec.message());
    }
}


bool Vault::verifyVaultKey(std::array<unsigned char, crypto_box_SEEDBYTES>& key) {
    EncryptedBlock block{};
    auto in = accessVaultFile(std::ios::in);

    // Skip magic + salt and read nonce + verifier.
    in.seekg(20, std::ios::beg);
    in.read(reinterpret_cast<char*>(block.nonce.data()), static_cast<std::streamsize>(block.nonce.size()));
    in.read(reinterpret_cast<char*>(block.ciphertext.data()), static_cast<std::streamsize>(block.ciphertext.size()));

    auto verifier = decrypt(block, key);
    return true;
}


std::array<unsigned char, crypto_pwhash_SALTBYTES> Vault::loadSalt() {
    std::array<unsigned char, crypto_pwhash_SALTBYTES> salt{};
    auto in = accessVaultFile(std::ios::in);
    
    //Skip magic, reads SALTBYTES size into the array and returns the number of bytes read.
    in.seekg(4, std::ios::beg);
    in.read(reinterpret_cast<char*>(salt.data()), static_cast<std::streamsize>(salt.size()));
    std::streamsize tbytes = in.gcount();

    if (tbytes != static_cast<std::streamsize>(salt.size())) {   // File does not contains the same size as the original salt.
        throw std::runtime_error("Invalid salt size in vault.enc (read " + std::to_string(tbytes) + " bytes)");
    }

    return salt;
}


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

