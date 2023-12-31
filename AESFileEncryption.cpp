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
    void EncryptFile(const std::string inputFilePath, const std::string outputFilePath, const char* key);
    void DecryptFile(const std::string inputFilePath, const std::string outputFilePath, const char* key);

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


void AESFileEncryption::EncryptFile(const std::string inputFilePath, const std::string outputFilePath, const char* key)
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

void AESFileEncryption::DecryptFile(const std::string inputFilePath, const std::string outputFilePath, const char* key){
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




int main(int argc, char* argv[])

{
    // read in command line arguments
    // if first command line argument is -e, encrypt
    // if first command line argument is -d, decrypt
    // if first command line argument is -h, print help
    // if first command line argument is -v, print version
    // ideas: key, input file, output file, all, status, config, log, time
    // else print help
    // if no command line arguments, print help
    
    // Strings containing returned info
    std::string helpInfo = "Help: \n"
                            "Usage: crypto-manager [OPTION]... [FILE]...\n"
                            "Encrypt or decrypt FILE using AES encryption.\n"
                            "\n"
                            "  -e, --encrypt\t\tencrypt FILE\n"
                            "  -d, --decrypt\t\tdecrypt FILE\n"
                            "  -h, --help\t\tprint this help message and exit\n"
                            "  -v, --version\t\tprint version and exit\n"
                            "\n"
                            "Report bugs to <" "email" ">.";

    std::string versionInfo = "Version: 0.1.0";



    AESFileEncryption aesEncryption;
    
    if (argc > 1) {
        std::string firstArg = argv[1];
        if (firstArg == "-e") {

            std::string key;

            std::string inputFilePath;
            std::cout << "Enter input file name to encrypt: ";
            std::cin >> inputFilePath;

            std::string outputFilePath;
            std::cout << "Enter output file path for the encrypted key: ";
            std::cin >> outputFilePath;

            //check if file exists
            std::ifstream inputFile(inputFilePath);
            if (!inputFile) {
                std::cerr << "Error: Could not open input file" << std::endl;
                return 1;
            }
            inputFile.close();
            std::cout << "File exists" << std::endl;

            std::cout << "Enter encryption key: ";
            std::cin >> key;

            std::cout << "Encrypting " << inputFilePath << std::endl;
            aesEncryption.EncryptFile(inputFilePath, outputFilePath, key.c_str());

    
        } else if (firstArg == "-d") {
            //take in file name and key
            std::string key;
            std::string inputFilePath;
            std::string outputFilePath;
            std::cout << "Enter input file name to decrypt: ";
            std::cin >> inputFilePath;
            std::cout << "Enter encryption key: ";
            std::cin >> key;
            std::cout << "Enter output file path for the decrypted key: ";
            std::cin >> outputFilePath;

            std::cout << "Decrypting" << inputFilePath << std::endl;
            aesEncryption.DecryptFile(inputFilePath, outputFilePath, key.c_str());

        } else if (firstArg == "-h") {
            std::cout << helpInfo << std::endl;
        } else if (firstArg == "-v") {
            std::cout << versionInfo << std::endl;
        } else {
            std::cout << "Invalid command line argument; \nuse crypto-manager -h for help" << std::endl;
        }
    } else {
        std::cout << "No command line arguments; \nuse crypto-manager -h for help" << std::endl;
    }


    return 0;
}
//TODO - Add comments
//TODO - Add error handling
//TODO - Add command line arguments
//TODO - Add file metadata (e.g. file name, file size, etc.)
