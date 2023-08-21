#ifndef AESENCRYPTION_H
#define AESENCRYPTION_H

#include <string>

class AESEncryption
{
public:
    std::string GenerateRandomIV();
    std::string Encrypt(const std::string& input, const std::string& key, const std::string& iv, std::string& output);
    std::string Decrypt(const std::string& input, const std::string& key, const std::string& iv, std::string& output);

private:
    std::string encryptionKey;

};

#endif
