#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <algorithm>
#include <pages/editor/codenode.hpp>
#include <pages/editor/textnode.hpp>
#include <qnamespace.h>
#include <search/matcher.hpp>
#include <search/searchcontroller.hpp>
#include <storage/notescache.hpp>

namespace pad {

QVector<QStringView>
SearchController::splitStringToStringView(const QString &text) {
  QVector<QStringView> result;

  qsizetype start = 0;

  while (start < text.size()) {
    while (start < text.size() && text[start].isSpace())
      ++start;

    if (start >= text.size())
      break;

    qsizetype end = start;

    while (end < text.size() && !text[end].isSpace())
      ++end;

    result.emplace_back(text.sliced(start, end - start));

    start = end;
  }

  return result;
}

SearchController::SearchController(Matcher *matcher, NotesCache *cache,
                                   QObject *parent)
    : QObject(parent), _matcher(matcher), _cache(cache) {}

void SearchController::search(const QString &prompt) {
  QVector<QStringView> words = splitStringToStringView(prompt);

  const qsizetype notesCount = _cache->allNotes().count();
  const qsizetype threadCount = QThread::idealThreadCount();

  const qsizetype chunkSize =
      std::ceil(static_cast<double>(notesCount) / threadCount);

  for (qsizetype i = 0; i < threadCount; ++i) {
    QVector<LoadedNote> notes;

    const qsizetype begin = i * chunkSize;
    const qsizetype end = std::min(begin + chunkSize, notesCount);

    for (qsizetype j = begin; j < end; ++j)
      notes.emplace_back(_cache->allNotes()[j]);

    if (!notes.isEmpty())
      runSearchThread(std::move(notes), words);
  }
}

bool SearchController::searchInText(const QString &text,
                                    const QStringView &queryWord) {
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

bool SearchController::searchInNote(const Note *note, const QStringView &word) {
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

void SearchController::runSearchThread(QVector<LoadedNote> notes,
                                       QVector<QStringView> words) {
  auto a = QtConcurrent::run([notes, words, this]() {
    QVector<LoadedNote> found;

    for (const auto &note : notes) {
      for (const auto &word : words) {
        if (searchInNote(note.note, word)) {
          found.push_back(note);
          break;
        }
      }
    }

    // DEBUG ONLY

    for (const auto &f : found) {
      qDebug() << "found note: " << f.note->title();
    }

    // emit or smthng
  });
}

} // namespace pad
