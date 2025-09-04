#ifndef KDF_HPP
#define KDF_HPP

# include <string>
# include <sodium.h>

std::string binToHex(const unsigned char* bin, size_t len);
std::array<unsigned char, crypto_pwhash_SALTBYTES> genSalt();
std::array<unsigned char, crypto_box_SEEDBYTES> deriveKey(const std::string& mpwd, const std::array<unsigned char, crypto_pwhash_SALTBYTES>& salt);

#endif
