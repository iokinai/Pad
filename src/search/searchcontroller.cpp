#include <QCoro/QCoroFuture>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <algorithm>
#include <pages/editor/codenode.hpp>
#include <pages/editor/textnode.hpp>
#include <search/currentsearch.hpp>
#include <search/matcher.hpp>
#include <search/searchcontroller.hpp>
#include <storage/notescache.hpp>

namespace pad {

QVector<QStringView>
SearchController::splitStringToStringView(const QString &text) {
  QVector<QStringView> result;

  QStringView textView = text;

  qsizetype start = 0;

  while (start < textView.size()) {
    while (start < textView.size() && textView[start].isSpace())
      ++start;

    if (start >= textView.size())
      break;

    qsizetype end = start;

    while (end < textView.size() && !textView[end].isSpace())
      ++end;

    result.emplace_back(textView.sliced(start, end - start));

    start = end;
  }

  return result;
}

// we use std::vector because QVector triggers copy constructor
// which is deleted on QCoro::Task
std::vector<QCoro::Task<QVector<LoadedNote>>>
SearchController::runWorkers(qsizetype threadCount, qsizetype chunkSize,
                             qsizetype notesCount, const QString &prompt,
                             std::shared_ptr<CurrentSearch> currentSearch) {
  std::vector<QCoro::Task<QVector<LoadedNote>>> workers;
  workers.reserve(threadCount);

  for (qsizetype i = 0; i < threadCount; ++i) {
    QVector<LoadedNote> notes;

    const qsizetype begin = i * chunkSize;
    const qsizetype end = std::min(begin + chunkSize, notesCount);

    for (qsizetype j = begin; j < end; ++j)
      notes.emplace_back(_cache->allNotes()[j]);

    if (!notes.isEmpty())
      workers.push_back(
          std::move(runSearchThread(std::move(notes), prompt, currentSearch)));
  }

  return workers;
}

SearchController::SearchController(Matcher *matcher, NotesCache *cache,
                                   QObject *parent)
    : QObject(parent), _matcher(matcher), _cache(cache) {}

QCoro::Task<void> SearchController::search(const QString &prompt) {
  const qsizetype threadCount = QThread::idealThreadCount();

  if (_currentSearch) {
    _currentSearch->cancelled = true;
  }

  _currentSearch = std::make_shared<CurrentSearch>(false);
  auto currentSearch = _currentSearch;

  const qsizetype notesCount = _cache->allNotes().count();

  const qsizetype chunkSize =
      std::ceil(static_cast<double>(notesCount) / threadCount);

  auto workers =
      runWorkers(threadCount, chunkSize, notesCount, prompt, currentSearch);

  QVector<LoadedNote> result;

  for (const auto &worker : workers) {
    result << co_await worker;
  }

  if (currentSearch->cancelled) {
    co_return;
  }

  emit searchEnded(result);
}

bool SearchController::searchInText(const QString &text,
                                    QStringView queryWord) {
  QVector<QStringView> wordsInSection = splitStringToStringView(text);

  for (const auto &candidate : wordsInSection) {
    if (candidate.size() < queryWord.size())
      continue;

    const qsizetype prefixLength =
        std::min(candidate.size(), queryWord.size() + tolerance);

    QStringView prefix = candidate.sliced(0, prefixLength);

    if (_matcher->match(queryWord, prefix)) {
      return true;
    }
  }

  return false;
}

bool SearchController::searchInNote(const Note *note, QStringView word) {
  if (note->title().contains(word, Qt::CaseInsensitive)) {
    return true;
  }

  for (const auto *node : note->nodes()) {
    switch (node->type()) {
    case Node::Text: {
      auto textNode = dynamic_cast<const TextNode *>(node);
      if (textNode && searchInText(textNode->text(), word)) {
        return true;
      }
      break;
    }
    case Node::Code: {
      auto codeNode = dynamic_cast<const CodeNode *>(node);
      if (codeNode && searchInText(codeNode->code(), word)) {
        return true;
      }
      break;
    }
    default:
      break;
    }
  }

  return false;
}

QCoro::Task<QVector<LoadedNote>> SearchController::runSearchThread(
    QVector<LoadedNote> notes, QString prompt,
    std::shared_ptr<CurrentSearch> currentSearch) {
  co_return co_await QtConcurrent::run([notes, prompt, this, currentSearch]() {
    QVector<QStringView> words = splitStringToStringView(prompt);
    QVector<LoadedNote> found;

    for (const auto &note : notes) {
      if (!currentSearch || currentSearch->cancelled) {
        return found;
      }

      for (const auto &word : words) {
        if (searchInNote(note.note, word)) {
          found.push_back(note);
          break;
        }
      }
    }

    return found;
  });
}

} // namespace pad
