#pragma once

#include <search/matcher.hpp>

namespace pad {

class DamerauLevenshteinMatcher : public Matcher {
  Q_OBJECT
private:
  int maxTolerance(const QStringView &word) const;

  int calcDistance(const QStringView &s1, const QStringView &s2);
  int calcCell(const QVector<int> &currentRow, const QVector<int> &prevRow,
               QVector<int> &twoBefore, qsizetype i, qsizetype j,
               const QStringView &s1, const QStringView &s2);

public:
  explicit DamerauLevenshteinMatcher(QObject *parent = nullptr);
  virtual bool match(const QStringView &s1, const QStringView &s2) override;
};

} // namespace pad
