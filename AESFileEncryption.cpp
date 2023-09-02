#include <iostream>
#include <fstream>
#include <string>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>

#include <ncurses.h>
#include <menu.h>


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
    std::cout << "\e[32m"; // Set the text color to green
    std::cout << "\e[?25l"; // Hide the cursor
    initscr(); // Initialize NCurses
    keypad(stdscr, TRUE); // Enable special keys

    int choice;
    int highlight = 1;
    int max_choices = 3;

    while (true) {
        clear(); // Clear the screen

        // Display the menu
        mvprintw(1, 1, "Select an option:");
        mvprintw(3, 1, "1. Encrypt");
        mvprintw(4, 1, "2. Decrypt");
        mvprintw(5, 1, "3. Exit");

        // Highlight the current choice
        for (int i = 1; i <= max_choices; i++) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            mvprintw(i + 2, 1, " "); // Clear any previous highlighting
            mvprintw(i + 2, 2, "%d", i);
            attroff(A_REVERSE);
        }

        // Get user input
        choice = getch();

        switch (choice) {
            case KEY_UP:
                if (highlight > 1) {
                    highlight--;
                }
                break;
            case KEY_DOWN:
                if (highlight < max_choices) {
                    highlight++;
                }
                break;
            case '\n': // Enter key
                std::cout << "\e[?25h";
                if (highlight == 1) {
                    // Add your encryption logic here
                    mvprintw(8, 1, "Encrypting...");
                } else if (highlight == 2) {
                    // Add your decryption logic here
                    mvprintw(8, 1, "Decrypting...");
                } else if (highlight == 3) {
                    endwin(); // End NCurses
                    return 0;
                }
                break;
            default:
                break;
        }

        refresh();
    }

    endwin(); // End NCurses

    AESFileEncryption aesEncryption;

    std::string key;

    std::string inputFilePath;
    std::cout << "Enter input file name to encrypt: ";
    std::cin >> inputFilePath;


    std::cout << "Enter encryption key: ";
    std::cin >> key;

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



    aesEncryption.EncryptFile(inputFilePath, outputFilePath, key.c_str());



    aesEncryption.DecryptFile(outputFilePath, "files/output2.txt", key.c_str());

    return 0;
}
//TODO - Add comments
//TODO - Add error handling
//TODO - Add command line arguments
//TODO - Add file metadata (e.g. file name, file size, etc.)
