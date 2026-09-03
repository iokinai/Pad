#include <search/dameraulevenshteinmatcher.hpp>

namespace pad {

DamerauLevenshteinMatcher::DamerauLevenshteinMatcher(QObject *parent)
    : Matcher(parent) {}

int DamerauLevenshteinMatcher::calcDistance(const QStringView &s1,
                                            const QStringView &s2) {
  if (s1.size() > s2.size()) {
    return calcDistance(s2, s1);
  }

  const qsizetype m = s1.size();
  const qsizetype n = s2.size();

  QVector<int> prevRow(m + 1);
  QVector<int> currentRow(m + 1);
  QVector<int> twoBefore(m + 1);

  std::iota(prevRow.begin(), prevRow.end(), 0);

  for (size_t i = 1; i <= n; ++i) {
    currentRow[0] = static_cast<int>(i);

    for (size_t j = 1; j <= m; ++j) {
      currentRow[j] = calcCell(currentRow, prevRow, twoBefore, i, j, s1, s2);
    }

    twoBefore = std::move(prevRow);
    prevRow = std::move(currentRow);
    currentRow.resize(m + 1);
  }

  return prevRow[m];
}

int DamerauLevenshteinMatcher::maxTolerance(const QStringView &word) const {
  switch (word.length()) {
  case 0:
  case 1:
    return 0;
  // case 2:
  //   return 1;
  default:
    return 2;
  }
}

int DamerauLevenshteinMatcher::calcCell(const QVector<int> &currentRow,
                                        const QVector<int> &prevRow,
                                        QVector<int> &twoBefore, qsizetype i,
                                        qsizetype j, const QStringView &s1,
                                        const QStringView &s2) {
  int remove = prevRow[j] + 1;
  int add = currentRow[j - 1] + 1;
  int cost = (s1[j - 1] == s2[i - 1]) ? 0 : 1;
  int change = prevRow[j - 1] + cost;

  if (i > 1 && j > 1 && s1[j - 1] == s2[i - 2] && s1[j - 2] == s2[i - 1]) {
    int swap = twoBefore[j - 2] + 1;
    return std::min({remove, add, change, swap});
  }

  return std::min({remove, add, change});
}

bool DamerauLevenshteinMatcher::match(const QStringView &s1,
                                      const QStringView &s2) {
  int dist = calcDistance(s1, s2);

  return dist < maxTolerance(s1);
}

} // namespace pad
