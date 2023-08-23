#include <iostream>
#include <fstream>
#include <string>

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>



#include "AESStringEncryption.h"

int main(){
    // // Input data, encryption key, and IV
    // std::string input;
    // std::cout << "Enter text: ";
    // std::getline(std::cin, input);

    AESStringEncryption aesEncryption;


    // // Generate random IV
    // std::string strIV = aesEncryption.GenerateRandomIV();
    // std::cout << "IV: " << strIV << std::endl;


    // std::cout << "Input: " << input << std::endl;
    std::string key = "MySecretKey12345";

    // // Encrypt
    // std::string encryptedData;
    // aesEncryption.Encrypt(input, key, strIV, encryptedData);
    // std::cout << "Encrypted: " << encryptedData << std::endl;

    // // Decrypt
    // std::string decryptedData;
    // aesEncryption.Decrypt(encryptedData, key, strIV, decryptedData);
    // std::cout << "Decrypted: " << decryptedData << std::endl;

    // File encryption
    std::string inputFile = "input.txt";
    std::string outputFile = "encrypted.txt";

    aesEncryption.fileEncrypt(inputFile, outputFile, key);

    // File decryption
    std::string decryptedFile = "decrypted.txt";
    aesEncryption.fileDecrypt(outputFile, decryptedFile, key);



    return 0;


}