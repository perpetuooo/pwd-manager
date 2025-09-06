#ifndef VAULT_HPP
#define VAULT_HPP

# include <fstream>
# include <string>
# include <array>
# include <vector>
# include <sodium.h>

class Vault {
private:
    static std::vector<unsigned char> genVerifier();

public:
    static bool fileExists(const std::string& filename);
    static void createVaultFile(const std::array<unsigned char, crypto_pwhash_SALTBYTES>& salt, std::array<unsigned char, crypto_box_SEEDBYTES> key);
    static std::array<unsigned char, crypto_pwhash_SALTBYTES> loadSalt();
    static void readFile(const std::string& filename);
    static void writeFile(const std::string& filename, const std::string& data);
};

#endif
