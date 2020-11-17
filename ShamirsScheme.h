#ifndef SHAMIR_SHAMIRSSCHEME_H
#define SHAMIR_SHAMIRSSCHEME_H
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <cassert>
#include "Key.h"
#include "Rational.h"

namespace ShamirsScheme {
  using Chunk = uint8_t ;
  namespace {
    CalcT p = 257; // calculations will be over Z_p ring

    CalcT poly(size_t x, const std::vector<CalcT>& coefs) {
      CalcT xPow = 1;
      CalcT result = coefs[0];
      for (size_t i = 1; i < coefs.size(); ++i) {
        xPow = x * xPow % p;
        result = (result + ((coefs[i] * xPow) % p)) % p;
      }
      return result % p;
    }
  }

  /// Creates shared keys for given string
  std::vector<Key> makeSharedKeys(const std::vector<Chunk>& data, size_t minimumKeys, size_t totalKeys) {
    std::random_device random_device;
    std::mt19937 engine{random_device()};
    std::uniform_int_distribution<CalcT> dist(1, p - 2);

    std::vector<Key> keys(totalKeys, Key(data.size()));
    for (size_t i = 1; i <= totalKeys; ++i)
      keys[i - 1].setX(i);
    for (const auto& chunk : data) {
      std::vector<CalcT> coefs;
      coefs.reserve(minimumKeys);
      coefs.push_back(chunk);
      for (size_t i = 0; i < minimumKeys - 1; ++i)
        coefs.push_back(dist(engine));
      for (size_t i = 0; i < totalKeys; i++) {
        CalcT f = poly(keys[i].getX(), coefs);
        if (f < 0)
          throw std::runtime_error("Too many keys");
        keys[i].addF(f);
        }
    }
    return keys;
  };

  /// Returns string decrypted with passed keys
  std::vector<Chunk> decrypt(const std::vector<Key>& keys) {
    const size_t secretSize = keys[0].valuesCount();
    std::vector<Chunk> result;
    result.reserve(secretSize);
    for (size_t k = 0; k < secretSize; ++k) {
      Rational<CalcT> chunk(0);
      for (const auto& key : keys) {
        CalcT xi = key.getX();
        CalcT fi = key.getF(k);
        Rational<CalcT> term(1);
        for (const auto& jkey : keys) {
          CalcT xj = jkey.getX();
          if (xj != xi) {
            term = (((term * Rational<CalcT>(xj)).reduce().mod(p) /
                     (Rational<CalcT>(xj) - Rational<CalcT>(xi))).reduce().mod(p)).mod(p);
          }
        }
        term = (term * Rational<CalcT>(fi)).reduce().mod(p);
        chunk = (chunk + term).reduce().mod(p);
      }
      result.push_back(static_cast<Chunk>(chunk.getNum()));
    }
    return result;
  }
}


#endif //SHAMIR_SHAMIRSSCHEME_H
