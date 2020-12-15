#pragma once

#include "ebc/BitcodeType.h"
#include "ebc/EmbeddedFile.h"

#include <string>

namespace ebc {
class EmbeddedBitcode : public EmbeddedFile {
 public:
  EmbeddedBitcode(std::string file, BitcodeType bitcodeType)
      : EmbeddedFile(std::move(file), EmbeddedFile::Type::Bitcode), _bitcodeType(bitcodeType) {}
  EmbeddedBitcode(std::string file)
      : EmbeddedFile(std::move(file), EmbeddedFile::Type::Bitcode), _bitcodeType(BitcodeType::Bitcode) {}
  EmbeddedBitcode(char *buffer, size_t bufferSize, BitcodeType bitcodeType)
      : EmbeddedFile(buffer, bufferSize, EmbeddedFile::Type::Bitcode), _bitcodeType(bitcodeType) {}
  EmbeddedBitcode(char *buffer, size_t bufferSize)
      : EmbeddedFile(buffer, bufferSize, EmbeddedFile::Type::Bitcode), _bitcodeType(BitcodeType::Bitcode) {}

  BitcodeType GetBitcodeType() {
    return _bitcodeType;
  }

 private:
  BitcodeType _bitcodeType;
};
}
