#pragma once

#include <QCoro/QCoroTask>
#include <QJsonObject>
#include <QStandardPaths>
#include <QString>
#include <memory>
#include <pages/editor/note.hpp>
#include <storage/loadednote.hpp>
#include <storage/manifest.hpp>

namespace pad {

// REFACTOR: move async I/O operations here with QCoro

struct LoadNotesResult {
  QVector<LoadedNote> notes;
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

  struct ZipDeleter {
    void *r;
    ~ZipDeleter();
  };

  using ZipDeleterPtr = std::unique_ptr<ZipDeleter>;

  MediaStorage *_storage;

  QString _basicPath =
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
      "/Pad/";

  QVector<QPair<QString, QByteArray>>
  extractMediaWithDataFromArchive(void *reader);

  Note *loadAndParseContent(void *reader, const Manifest &manifest);

  Manifest loadAndParseManifest(void *reader);

  QStringList extractMediaFromNote(Note *note);

  QVector<QPair<QString, QByteArray>>
  loadMediaWithDataFromStorage(const QStringList &mediaPaths);

  std::optional<ZipDeleterPtr> prepareReader(const QString &path);
  void registerMediaForNote(const QVector<QPair<QString, QByteArray>> &media);

  LoadNotesResult *loadNotes(
      std::optional<LoadedNote> (StorageController::*loadSingleNoteDelegate)(
          const QString &path),
      const int start = 0, const int maxLoadCount = INT32_MAX);

public:
  explicit StorageController(MediaStorage *storage, QObject *parent = nullptr);

  LoadNotesResult *loadFullNotes(const int start = 0,
                                 const int maxLoadCount = INT32_MAX);

  void saveNoteToPad(NoteSaveData data);

  NoteSaveData prepareNoteSaveData(Note *note, const QString &path);

  std::optional<LoadedNote> loadSingleNoteWithoutMedia(const QString &path);
  std::optional<LoadedNote> loadSingleNoteWithMedia(const QString &path);

  LoadNotesResult *loadNotesWithoutMedia(const int start = 0,
                                         const int maxLoadCount = INT32_MAX);

  QCoro::Task<LoadNotesResult *>
  loadFullNotesAsync(const int start = 0, const int maxLoadCount = INT32_MAX);

  QString addMediaFromSystem(const QString &systemPath);

  void loadMediaFor(const QString &path);

  QCoro::Task<void> saveNoteAsync(NoteSaveData data);
};

} // namespace pad
