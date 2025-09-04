#ifndef VAULT_HPP
#define VAULT_HPP

# include <fstream>
# include <string>
# include <array>
# include <sodium.h>

class Vault {
public:
    static std::array<unsigned char, crypto_pwhash_SALTBYTES> genSalt();
    static std::array<unsigned char, crypto_pwhash_SALTBYTES> loadSalt();
    static bool fileExists(const std::string& filename);
    static void readFile(const std::string& filename);
    static void writeFile(const std::string& filename, const std::string& data);
};

#endif
