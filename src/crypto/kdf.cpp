# include <iostream>
# include <string>
# include <sodium.h>
# include "crypto/kdf.hpp"

using namespace std;

string binToHex(const unsigned char* bin, size_t len) {
    static const char hex[] = "0123456789abcdef";
    string s; s.resize(len*2);

    for (size_t i=0; i<len; i++) {
        s[2*i] = hex[(bin[i]>>4) & 0xF];
        s[2*i+1] = hex[bin[i] & 0xF];
    }

    return s;
}

void deriveKey(const string& mpwd) {
    unsigned char key[crypto_box_SEEDBYTES];
    unsigned char salt[crypto_pwhash_SALTBYTES];

    // Generate random bytes for salt.
    randombytes_buf(salt, sizeof salt);
    cout << "salt: " + binToHex(salt, sizeof salt) << endl;

    // Generate key.
    if (crypto_pwhash(
        key, sizeof key, 
        mpwd.data(), mpwd.length(), 
        salt, 
        crypto_pwhash_OPSLIMIT_INTERACTIVE, 
        crypto_pwhash_MEMLIMIT_INTERACTIVE, 
        crypto_pwhash_ALG_DEFAULT
    ) != 0) throw runtime_error("Out of memory...");
    cout << "key: " + binToHex(key, sizeof key) << endl;

    // store salt (plaintext?)
    // return key for encryption
}
