#include <iostream>
#include <fstream>
#include <string>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>





class AESFileEncryption
{
public:
    void EncryptFile(const char* inputFilePath, const char* outputFilePath, const char* key);
    void DecryptFile(const char* inputFilePath, const char* outputFilePath, const char* key);

private:
    CryptoPP::SecByteBlock GenerateRandomIV();

};

CryptoPP::SecByteBlock AESFileEncryption::GenerateRandomIV(){
    // Generate a random IV
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    prng.GenerateBlock(iv, iv.size());

    return iv;
}


void AESFileEncryption::EncryptFile(const char* inputFilePath, const char* outputFilePath, const char* key)
{
    CryptoPP::SecByteBlock iv = GenerateRandomIV();

   // Write the IV to the output file
    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error: Could not open output file" << std::endl;
        return;
    }
    outputFile.write(reinterpret_cast<const char*>(iv.data()), iv.size());


    // Initialize encryption objects
    CryptoPP::AES::Encryption aesEncryption(
        reinterpret_cast<const CryptoPP::byte*>(key),
        CryptoPP::AES::DEFAULT_KEYLENGTH
    );




    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(
        aesEncryption,
        iv
    );

    // Open input file
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile)
    {
        std::cerr << "Error: Could not open input file" << std::endl;
        return;
    }


    // Copy input file to output file, encrypting as we go
    CryptoPP::FileSource(
        inputFile,
        true,
        new CryptoPP::StreamTransformationFilter(
            cbcEncryption,
            new CryptoPP::FileSink(
                outputFile
            )
        )
    );


}

void AESFileEncryption::DecryptFile(const char* inputFilePath, const char* outputFilePath, const char* key){
    // Read the IV from the input file
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Could not open input file" << std::endl;
        return;
    }
    inputFile.read(reinterpret_cast<char*>(iv.data()), iv.size());





    // Initialize decryption objects
    CryptoPP::AES::Decryption aesDecryption(
        reinterpret_cast<const CryptoPP::byte*>(key),
        CryptoPP::AES::DEFAULT_KEYLENGTH
    );
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(
        aesDecryption,
        iv
    );


    // Open output file
    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile)
    {
        inputFile.close();
        std::cerr << "Error: Could not open output file" << std::endl;
        return;
    }

    // Copy input file to output file, decrypting as we go
    CryptoPP::FileSource(
        inputFile,
        true,
        new CryptoPP::StreamTransformationFilter(
            cbcDecryption,
            new CryptoPP::FileSink(
                outputFile
            )
        )
    );

}


int main()
{
    AESFileEncryption aesEncryption;
    std::string key = "0123456789abcdef";


    aesEncryption.EncryptFile("files/input.txt", "files/test.enc", key.c_str());



    aesEncryption.DecryptFile("files/test.enc", "files/output1.txt", key.c_str());

    return 0;
}
//TODO - Add comments
//TODO - Add error handling
//TODO - Add command line arguments
//TODO - Add file metadata (e.g. file name, file size, etc.)
