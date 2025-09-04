# include <iostream>
# include <string>
# include <array>
# include <sodium.h>
# include "crypto/kdf.hpp"
# include "storage/vault.hpp"


std::string binToHex(const unsigned char* bin, size_t len) {
    static const char hex[] = "0123456789abcdef";
    std::string s; s.resize(len*2);

    for (size_t i=0; i<len; i++) {
        s[2*i] = hex[(bin[i]>>4) & 0xF];
        s[2*i+1] = hex[bin[i] & 0xF];
    }

    return s;
}


std::array<unsigned char, crypto_pwhash_SALTBYTES> genSalt() {
    std::array<unsigned char, crypto_pwhash_SALTBYTES> salt{};

    // Generate salt.
    randombytes_buf(salt.data(), salt.size());
    std::cout << "\ngenerated salt: " + binToHex(salt.data(), salt.size()) << std::endl;

    // Save generated salt
    std::ofstream out("secrets.txt", std::ios::binary);
    out.write(reinterpret_cast<const char*>(salt.data()), salt.size());
    
    return salt;
}


std::array<unsigned char, crypto_box_SEEDBYTES> deriveKey(const std::string& mpwd, const std::array<unsigned char, crypto_pwhash_SALTBYTES>& salt) {
    std::array<unsigned char, crypto_box_SEEDBYTES> key{};

    // Generate key.
    if (crypto_pwhash(
        key.data(), key.size(), 
        mpwd.data(), mpwd.length(), 
        salt.data(), 
        crypto_pwhash_OPSLIMIT_INTERACTIVE, 
        crypto_pwhash_MEMLIMIT_INTERACTIVE, 
        crypto_pwhash_ALG_DEFAULT
    ) != 0) throw std::runtime_error("Out of memory...");
    std::cout << "generated key: " + binToHex(key.data(), sizeof key);

    return key;
}
