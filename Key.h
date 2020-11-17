#ifndef SHAMIR_KEY_H
#define SHAMIR_KEY_H
#include <cstdint>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include "CalcT.h"

namespace ShamirsScheme {
  class Key {
    CalcT x = 0;
    std::vector<CalcT> values;

  public:
    explicit Key(size_t nOfChunks) {
      values.reserve(nOfChunks);
    };

    explicit Key(const std::string &str);

    CalcT getX() const {
      return x;
    }

    void setX(CalcT x) {
      this->x = x;
    }

    CalcT getF(size_t i) const {
      return values[i];
    }

    void addF(CalcT f) {
      values.push_back(f);
    }

    size_t valuesCount() const {
      return values.size();
    }

    std::string toString() const;
  };
}


#endif //SHAMIR_KEY_H
