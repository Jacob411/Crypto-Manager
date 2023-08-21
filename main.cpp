#include <iostream>
#include <fstream>
#include <string>
#include <cryptopp/cryptlib.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>


std::string GenerateRandomIV() {
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
    std::cout << "Enter text: ";
    std::getline(std::cin, input);

    // Generate random IV
    std::string strIV = GenerateRandomIV();
    std::cout << "IV: " << strIV << std::endl;


    std::cout << "Input: " << input << std::endl;
    std::string key = "MySecretKey12345";

    // Encrypt
    std::string encryptedData;
    Encrypt(input, key, strIV, encryptedData);
    std::cout << "Encrypted: " << encryptedData << std::endl;

    // Decrypt
    std::string decryptedData;
    Decrypt(encryptedData, key, strIV, decryptedData);
    std::cout << "Decrypted: " << decryptedData << std::endl;


    return 0;
}