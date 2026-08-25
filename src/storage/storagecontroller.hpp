#pragma once

#include <QJsonObject>
#include <QStandardPaths>
#include <QString>
#include <pages/editor/note.hpp>
#include <storage/manifest.hpp>

namespace pad {

struct LoadNoteResult {
  Note *loadedNote;
  QVector<QPair<QString, QByteArray>> medias;
  QString name;
};

struct LoadNotesResult {
  QVector<LoadNoteResult> notes;
  size_t totalCount;
};

struct NoteSaveData {
  QString path;
  QJsonObject manifest;
  QJsonObject markup;
  QVector<QPair<QString, QByteArray>> mediaData;
};

class StorageController : public QObject {
  Q_OBJECT

  MediaStorage *_storage;

  QString _basicPath =
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
      "/Pad/";

  std::optional<LoadNoteResult> loadSingleNote(const QString &path);

  QVector<QPair<QString, QByteArray>>
  extractMediaWithDataFromArchive(void *reader);

  Note *loadAndParseContent(void *reader, const Manifest &manifest);

  Manifest loadAndParseManifest(void *reader);

  QStringList extractMediaFromNote(Note *note);

  QVector<QPair<QString, QByteArray>>
  loadMediaWithDataFromStorage(const QStringList &mediaPaths);

public:
  explicit StorageController(MediaStorage *storage, QObject *parent = nullptr);

  LoadNotesResult loadNotes(const int maxLoadCount);

  void saveNoteToPad(NoteSaveData data);

  NoteSaveData prepareNoteSaveData(Note *note, const QString &path);
};

} // namespace pad
