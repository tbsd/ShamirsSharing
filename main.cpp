#include <iostream>
#include <string>
#include <vector>
#include "ShamirsScheme.h"
#include <fstream>

int main(int argc, char** argv) {
  std::string helpMessage =
      "Usage:\nEncrypt: shamir -e input_file t n "
      "\nDecrypt: shamir -d  key_1 key_2 ... key_t "
      "\n-e - encryption mode"
      "\n-d - decryption mode"
      "\n t - minimal number of keys needed for decryption"
      "\n n - total number of keys\n";
  if (argc >= 3 && (std::string(argv[1]) == "-e" && argc >= 5 || std::string(argv[1]) == "-d")) {
    if (std::string(argv[1]) == "-e") {
      const char* filePath = argv[2];
      int minimumKeys = std::stoi(argv[3]);
      int totalKeys = std::stoi(argv[4]);
      if (minimumKeys < 1 || totalKeys < 1) {
        std::cout << "Error: t and n must be at least 1\n" << std::endl;
        std:: cout << helpMessage;
        return 1;
      }
      if (minimumKeys > totalKeys) {
        std::cout << "Error: t must be less or equal to n\n" << std::endl;
        std:: cout << helpMessage;
        return 1;
      }
      std::ifstream inFile(filePath, std::ios::binary|std::ios::in);
      if (inFile.is_open()) {
        inFile.unsetf(std::ios::skipws);
        inFile.seekg(0, std::ios::end);
        auto fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);
        std::vector<ShamirsScheme::Chunk> data;
        data.reserve(fileSize);
        data.insert(data.cbegin(), std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>());
        inFile.close();
        try {
          std::vector<ShamirsScheme::Key> keys = ShamirsScheme::makeSharedKeys(data, minimumKeys, totalKeys);
          for (auto &key : keys)
            std::cout << key.toString() << " ";
          std::cout << std::endl;
        } catch (std::runtime_error& ex) {
          std::cout << "Error: " << ex.what() << std::endl;
        }
      } else {
        std::cout << "Error: file " << filePath << " does not exist or could not be read\n" << std::endl;
        std::cout << helpMessage;
      }
    } else {
      std::vector<ShamirsScheme::Key> keys;
      keys.reserve(argc - 2);
      for (size_t i = 2; i < argc; ++i)
        try {
          keys.emplace_back(argv[i]);
        } catch (std::invalid_argument &ex) {
          std::cout << "Error: " << ex.what() << std::endl;
          return 1;
        }
      const size_t count = keys[0].valuesCount();
      for (const auto& key : keys)
        if (key.valuesCount() != count) {
          std::cout << "Error: key sizes does not match" << std::endl;
          return 1;
        }
      std::vector<ShamirsScheme::Chunk> decryptedData = ShamirsScheme::decrypt(keys);
      for (auto& ch : decryptedData)
        std::cout << ch;
    }
  } else {
    std::cout << helpMessage;
    return 1;
  }
  return 0;
}

