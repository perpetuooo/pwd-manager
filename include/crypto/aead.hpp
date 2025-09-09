#ifndef AEAD_HPP
#define AEAD_HPP

#include <vector>
#include <array>
#include <sodium.h>

struct EncryptedBlock {
    std::array<unsigned char, crypto_aead_chacha20poly1305_NPUBBYTES> nonce;
    std::vector<uint8_t> ciphertext;
};

EncryptedBlock encrypt(const unsigned char* msg, const unsigned int msg_len, std::array<unsigned char, crypto_box_SEEDBYTES>& key) ;
std::vector<uint8_t> decrypt(const EncryptedBlock& block, const std::array<unsigned char, crypto_box_SEEDBYTES>& key);

#endif