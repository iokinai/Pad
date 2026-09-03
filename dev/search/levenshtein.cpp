#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

int calcCell(const std::vector<int> &currentRow,
             const std::vector<int> &prevRow, size_t i, size_t j,
             const std::string &s1, const std::string &s2) {
  if (i == 0)
    return j;
  if (j == 0)
    return i;

  int remove = prevRow[j] + 1;
  int add = currentRow[j - 1] + 1;
  int cost = (s1[j - 1] == s2[i - 1]) ? 0 : 1;
  int change = prevRow[j - 1] + cost;

  return std::min({remove, add, change});
}

int calc(std::string s1, std::string s2) {
  if (s1.size() > s2.size()) {
    return calc(s2, s1);
  }

  const size_t m = s1.size();
  const size_t n = s2.size();

  std::vector<int> prevRow(m + 1);
  std::vector<int> currentRow(m + 1);

  std::iota(prevRow.begin(), prevRow.end(), 0);

  for (size_t i = 1; i <= n; ++i) {
    currentRow[0] = static_cast<int>(i);

    for (size_t j = 1; j <= m; ++j) {
      currentRow[j] = calcCell(currentRow, prevRow, i, j, s1, s2);
    }

    prevRow = currentRow;
  }

  return prevRow[m];
}

struct CheckPair {
  std::string w1, w2;
  int result;
};

int main() {
  std::println("TEST: Levenshtein");
  std::vector<CheckPair> testData = {{"", "", 0},
                                     {"a", "", 1},
                                     {"", "a", 1},
                                     {"hello", "", 5},
                                     {"", "world", 5},

                                     {"a", "a", 0},
                                     {"hello", "hello", 0},
                                     {"levenshtein", "levenshtein", 0},

                                     {"cat", "hat", 1},
                                     {"cat", "cats", 1},
                                     {"cats", "cat", 1},
                                     {"cat", "cast", 1},
                                     {"cast", "cat", 1},
                                     {"at", "cat", 1},
                                     {"cat", "at", 1},

                                     {"abc", "def", 3},
                                     {"abc", "abcdef", 3},
                                     {"abcdef", "abc", 3},

                                     {"kitten", "sitting", 3},
                                     {"saturday", "sunday", 3},
                                     {"rosettacode", "raisethysword", 8},
                                     {"gumbo", "gambol", 2},
                                     {"intention", "execution", 5},

                                     {"book", "back", 2},
                                     {"unfocused", "focused", 2},
                                     {"prefix", "pre", 3},

                                     {"ab", "ba", 2},
                                     {"aunt", "antu", 2},

                                     {"Hello", "hello", 1},
                                     {"Short", "VeryLongString", 12}};

  for (const auto &pair : testData) {
    auto result = calc(pair.w1, pair.w2);

    if (result != pair.result) {
      std::println("Test failed on pair: {} and {}. The result was {} but "
                   "should be {}",
                   pair.w1, pair.w2, result, pair.result);
      std::abort();
    }
  }

  std::cout << "Test passed successfully!" << std::endl;
}
