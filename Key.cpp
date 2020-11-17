#include "Key.h"

namespace ShamirsScheme {
  Key::Key(const std::string &str) {
    std::stringstream keyStrStream(str);
    static const std::string errorMsg = "invalid key. Key format: x,value_1,value_2,...,value_t";
    if (!std::isdigit(keyStrStream.peek()))
      throw std::invalid_argument(errorMsg);
    keyStrStream >> x;
    keyStrStream.ignore(1);
    if (!std::isdigit(keyStrStream.peek()))
      throw std::invalid_argument(errorMsg);
    CalcT tmp;
    while ((keyStrStream >> tmp)) {
      addF(tmp);
      keyStrStream.ignore(1);
      if (!keyStrStream.eof() && !std::isdigit(keyStrStream.peek()))
        throw std::invalid_argument(errorMsg);
    }
  }

  std::string Key::toString() const {
    std::stringstream stream;
    stream << x;
    for (const auto &val : values) {
      stream << "," << val;
    }
    return stream.str();
  }
}
