# include <stdexcept>
# include <string>
# include <vector>
# include <array>
# include <sodium.h>

# include "crypto/aead.hpp"


// https://doc.libsodium.org/secret-key_cryptography/aead#limitations

EncryptedBlock encrypt(const unsigned char* msg, const unsigned int msg_len, std::array<unsigned char, crypto_box_SEEDBYTES>& key) {
    EncryptedBlock block{};
    
    // Generate nonce.
    randombytes_buf(block.nonce.data(), block.nonce.size());
    
    // Allocate ciphertext size.
    block.ciphertext.resize(msg_len + crypto_aead_chacha20poly1305_ABYTES);
    unsigned long long ciphertext_len;

    // Encrypt message.
    if (crypto_aead_chacha20poly1305_encrypt(
        block.ciphertext.data(), &ciphertext_len, 
        msg, msg_len, 
        nullptr, 0, nullptr, 
        block.nonce.data(), key.data()) != 0) {
            throw std::runtime_error("Encryption failed");
    }

    block.ciphertext.resize(ciphertext_len);
    return block;
}


std::vector<uint8_t> decrypt(const EncryptedBlock& block ,const std::array<unsigned char, crypto_box_SEEDBYTES>& key) {
    // Allocate plaintext size.
    std::vector<uint8_t> plaintext(block.ciphertext.size() - crypto_aead_chacha20poly1305_ABYTES);
    unsigned long long plaintext_len;

    // Decrypt ciphertext.
    if (crypto_aead_chacha20poly1305_decrypt(
        plaintext.data(), &plaintext_len,
        NULL,
        block.ciphertext.data(), block.ciphertext.size(),
        nullptr, 0,
        block.nonce.data(), key.data()) != 0) {
            // throw std::runtime_error("Decryption failed (wrong key or corrupted data)");
            return {};  // Return an empty vector in case of failure to handle it outside this function.
    }

    plaintext.resize(plaintext_len);
    return plaintext;
}
