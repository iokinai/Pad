#pragma once

#include <QObject>
#include <memory>
#include <search/currentsearch.hpp>
#include <search/matcher.hpp>
#include <storage/notescache.hpp>

namespace pad {

class SearchController : public QObject {
  Q_OBJECT

  static constexpr const int tolerance = 1;

  Matcher *_matcher;
  NotesCache *_cache;
  std::shared_ptr<CurrentSearch> _currentSearch = nullptr;

  QVector<QStringView> splitStringToStringView(const QString &text);

  QCoro::Task<QVector<LoadedNote>>
  runSearchThread(QVector<LoadedNote> notes, QString prompt,
                  std::shared_ptr<CurrentSearch> currentSearch);
  bool searchInText(const QString &text, QStringView word);
  bool searchInNote(const Note *note, QStringView word);
  std::vector<QCoro::Task<QVector<LoadedNote>>>
  runWorkers(qsizetype threadCount, qsizetype chunkSize, qsizetype notesCount,
             const QString &prompt,
             std::shared_ptr<CurrentSearch> currentSearch);

public:
  SearchController(Matcher *matcher, NotesCache *cache,
                   QObject *parent = nullptr);

  QCoro::Task<void> search(const QString &prompt);

signals:
  void searchEnded(QVector<LoadedNote> notes);
};

} // namespace pad
