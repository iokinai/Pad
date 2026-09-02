#pragma once

#include <QObject>
#include <search/matcher.hpp>
#include <storage/notescache.hpp>

namespace pad {

class SearchController : public QObject {
  Q_OBJECT

  static constexpr const int tolerance = 1;

  Matcher *_matcher;
  NotesCache *_cache;

  QVector<QStringView> splitStringToStringView(const QString &text);
  void runSearchThread(QVector<LoadedNote> notes, QVector<QStringView> words);
  bool searchInText(const QString &text, const QStringView &word);
  bool searchInNote(const Note *note, const QStringView &word);

public:
  SearchController(Matcher *matcher, NotesCache *cache,
                   QObject *parent = nullptr);

  Q_INVOKABLE void search(const QString &prompt);
};

} // namespace pad
