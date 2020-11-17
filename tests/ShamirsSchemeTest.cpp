#include "gtest/gtest.h"
#include "../ShamirsScheme.h"
#include "gmock/gmock-matchers.h"

std::vector<ShamirsScheme::Chunk> data = {'a', 'b', 'c', ' ', '!', 'H', 'e', 'l', 'l', 'o', ',', '@'};

TEST(ShamirsSchemeTest, returnsRightAmountOfKeys) {
  EXPECT_EQ(ShamirsScheme::makeSharedKeys(data, 1, 1).size(), 1);
  EXPECT_EQ(ShamirsScheme::makeSharedKeys(data, 2, 3).size(), 3);
  EXPECT_EQ(ShamirsScheme::makeSharedKeys(data, 3, 3).size(), 3);
  EXPECT_EQ(ShamirsScheme::makeSharedKeys(data, 50, 50).size(), 50);
  EXPECT_EQ(ShamirsScheme::makeSharedKeys(data, 5, 50).size(), 50);
}

TEST(ShamirsSchemeTest, decodesRight) {
  size_t minKeys = 10;
  size_t totalKeys = 12;
  auto keys = ShamirsScheme::makeSharedKeys(data, minKeys, totalKeys);
  ASSERT_THAT(decrypt(keys), data);
  std::vector<ShamirsScheme::Key> lessKeys;
  lessKeys.reserve(totalKeys);
  for (size_t i = 0; i < totalKeys; ++i) {
    lessKeys.clear();
    lessKeys.insert(lessKeys.cbegin(), keys.begin(), keys.end());
    lessKeys.erase(lessKeys.begin() + i);
    for (size_t j = 0; j < totalKeys - 1; ++j) {
      ShamirsScheme::Key t = lessKeys[j];
      lessKeys.erase(lessKeys.begin() + j);
      ASSERT_THAT(decrypt(lessKeys), data);
      lessKeys.insert(lessKeys.cbegin() + j, t);
    }
  }
}

