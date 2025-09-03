#ifndef VAULT_HPP
#define VAULT_HPP

# include <fstream>
# include <string>

class Vault {
private:
    static std::fstream file;

public:
    static bool fileExists(const std::string& filename);
    static void readFile(const std::string& filename);
    static void writeFile(const std::string& filename, const std::string& data);
};

#endif
