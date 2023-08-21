#include "AESEncryption.h"
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>



std::string AESEncryption::GenerateRandomIV(){
    // Generate a random IV
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    prng.GenerateBlock(iv, iv.size());

    // Convert to string
    std::string ivString;
    CryptoPP::StringSource(iv, iv.size(), true,
        new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(ivString)
        )
    );

    return ivString;
}

std::string AESEncryption::Encrypt(const std::string& input, const std::string& key, const std::string& iv, std::string& output) {

   // Initialize encryption objects
    CryptoPP::AES::Encryption aesEncryption(
        reinterpret_cast<const CryptoPP::byte*>(key.c_str()),
        CryptoPP::AES::DEFAULT_KEYLENGTH
    );

    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(
        aesEncryption,
        reinterpret_cast<const CryptoPP::byte*>(iv.c_str())
    );

    // Perform encryption
    CryptoPP::StringSource(input, true,
        new CryptoPP::StreamTransformationFilter(cbcEncryption,
            new CryptoPP::StringSink(output)
        )
    );
    
}

std::string AESEncryption::Decrypt(const std::string& input, const std::string& key, const std::string& iv, std::string& output) {
    // Initialize decryption objects
    CryptoPP::AES::Decryption aesDecryption(      
        reinterpret_cast<const CryptoPP::byte*>(key.c_str()),
        CryptoPP::AES::DEFAULT_KEYLENGTH
        );
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(
        aesDecryption,
        reinterpret_cast<const CryptoPP::byte*>(iv.c_str())
    );

    // Perform decryption
    CryptoPP::StringSource(input, true,
        new CryptoPP::StreamTransformationFilter(cbcDecryption,
            new CryptoPP::StringSink(output)
        )
    );
}

