#include <iostream>
#include <fstream>
#include <string>




#include "AESStringEncryption.h"

int main(){
    // Input data, encryption key, and IV
    std::string input;
    std::cout << "Enter text: ";
    std::getline(std::cin, input);

    AESStringEncryption aesEncryption;
    std::string key = "0123456789abcdef";

    // Generate random IV
    std::string strIV = aesEncryption.GenerateRandomIV();
    std::cout << "IV: " << strIV << std::endl;


    std::cout << "Input: " << input << std::endl;

    // Encrypt
    std::string encryptedData;
    aesEncryption.Encrypt(input, key, strIV, encryptedData);
    std::cout << "Encrypted: " << encryptedData << std::endl;

    // Decrypt
    std::string decryptedData;
    aesEncryption.Decrypt(encryptedData, key, strIV, decryptedData);
    std::cout << "Decrypted: " << decryptedData << std::endl;


    return 0;



}