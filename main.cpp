#include <iostream>
#include <fstream>
#include <string>
#include <cryptopp/cryptlib.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>


void GenerateRandomIV(CryptoPP::byte* iv) {
    // Generate a random IV
    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(iv, CryptoPP::AES::BLOCKSIZE);
}

void Encrypt(const std::string& input, const std::string& key, const std::string& iv, std::string& output) {

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


void Decrypt(const std::string& input, const std::string& key, const std::string& iv, std::string& output) {
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

int main(){
    // Input data, encryption key, and IV
    std::string input;
    std::getline(std::cin, input);

    std::cout << "Input: " << input << std::endl;
    std::string key = "MySecretKey12345";
    std::string iv = "InitializationVec";

    // Encrypt
    std::string encryptedData;
    Encrypt(input, key, iv, encryptedData);
    std::cout << "Encrypted: " << encryptedData << std::endl;

    // Decrypt
    std::string decryptedData;
    Decrypt(encryptedData, key, iv, decryptedData);
    std::cout << "Decrypted: " << decryptedData << std::endl;

    CryptoPP::SecByteBlock iv;
    GenerateRandomIV(iv);

    return 0;
}