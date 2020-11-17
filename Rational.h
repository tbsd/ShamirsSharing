#ifndef SHAMIR_RATIONAL_H
#define SHAMIR_RATIONAL_H
#include <stdexcept>
#include <numeric>
#include <utility>

template <typename T>
class Rational {
  T numerator;
  T denominator;

  T gcd(T u, T v) {
    const auto inversions = extendedEuclid(u, v);
    return inversions.first * u + inversions.second * v;
  }

  std::pair<T, T> extendedEuclid(int u, int v) {
    if (u == 0)
      return std::make_pair(0, 1);
    const auto tmp = extendedEuclid(v % u, u);
    T x = tmp.first;
    T y = tmp.second;
    return std::make_pair((y - (v / u) * x), x);
  }

public:
  constexpr Rational() : numerator(0), denominator(1) {}

  Rational(T numerator) {
    this->numerator = numerator;
    denominator = 1;
  }

  Rational(T numerator, T denominator) {
    setNum(numerator);
    setDen(denominator);
  }

  T getNum() const {
    return numerator;
  }

  T getDen() const {
    return denominator;
  }

  void setNum (T numerator) {
    this->numerator = numerator;
  }

  void setDen (T denominator) {
    if (denominator == 0)
      throw std::runtime_error("Denominator must be nonzero value.");
    if (denominator < 0) {
      numerator = -numerator;
      this->denominator = -denominator;
    } else
      this->denominator = denominator;
  }

  std::string toString() const {
    return std::to_string(numerator) + "/" + std::to_string(denominator);
  }

  Rational<T> operator-() const {
    return Rational<T>(-numerator, denominator);
  }

  Rational<T> operator+(const Rational<T>& rhs) const {
    return Rational<T>(numerator * rhs.denominator + rhs.numerator * denominator,
                       denominator * rhs.denominator);
  }

  Rational<T> operator-(const Rational<T>& rhs) const {
    return Rational<T>(numerator * rhs.denominator - rhs.numerator * denominator,
                       denominator * rhs.denominator);
  }

  Rational<T> operator*(const Rational<T>& rhs) const {
    return Rational<T>(numerator * rhs.numerator, denominator * rhs.denominator);
  }

  Rational<T> operator/(const Rational<T>& rhs) const {
    return Rational<T>(numerator * rhs.denominator, denominator * rhs.numerator);
  }

  Rational<T> flip() const {
    return Rational<T>(denominator, numerator);
  }

  Rational<T>& reduce() {
    if (numerator == 0) {
      denominator = 1;
    } else {
      T divider = gcd(numerator, denominator);
      numerator /= divider;
      setDen(denominator / divider);
    }
    return *this;
  }

  bool operator<(const Rational<T> rhs) const {
    return (*this - rhs).numerator < 0;
  }

  /// Returns this number over ring Z_p:
  /// Ration<T>(x, y).mod(p) == Ration<T>((x * multiplicativeInverse(y)) % p)
  /// P must be positive
  Rational<T> mod(T p) {
    if (denominator == 1) {
      if (numerator >= 0) {
        return numerator % p;
      } else {
        return p + (numerator % p);
      }
    }
    T inverseDen = extendedEuclid(p, denominator).second;
    if (inverseDen < 0)
      inverseDen = p + (inverseDen % p);
    T result = (inverseDen * (numerator >= 0 ? numerator : p + (numerator % p))) % p;
    return result;
  }
};

#endif //SHAMIR_RATIONAL_H
