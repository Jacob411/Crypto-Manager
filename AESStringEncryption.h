#ifndef AESSTRINGENCRYPTION_H
#define AESSTRINGENCRYPTION_H

#include <string>

class AESStringEncryption
{
public:
    std::string GenerateRandomIV();
    std::string Encrypt(const std::string& input, const std::string& key, const std::string& iv, std::string& output);
    std::string Decrypt(const std::string& input, const std::string& key, const std::string& iv, std::string& output);
    void fileEncrypt(std::string inputFile, std::string outputFile, std::string& key);
    void fileDecrypt(std::string inputFile, std::string outputFile, std::string& key);

private:
};

#endif
