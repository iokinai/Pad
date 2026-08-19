#pragma once

#include <QObject>
#include <QStandardPaths>
#include <notes/notesmodel.hpp>
#include <pages/editor/note.hpp>
#include <storage/manifest.hpp>
#include <storage/mediastorage.hpp>

namespace pad {

namespace detail {

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

} // namespace detail

class NotesController : public QObject {
  Q_OBJECT

  static constexpr const int LOAD_FIRST_COUNT = 10;

  size_t _loaded = 0;
  size_t _totalCount = 0;
  NotesModel _notesModel;
  MediaStorage *_storage;
  QHash<QString, Note *> _notePathMap;
  QSet<Note *> _unsavedNotes;

  QString basicPath =
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
      "/Pad/";

  Q_PROPERTY(size_t notesCount READ notesCount NOTIFY notesCountChanged)
  Q_PROPERTY(NotesModel *notes READ notes CONSTANT)

  detail::LoadNotesResult loadNotes();
  void loadNotesAsync();
  std::optional<detail::LoadNoteResult> loadSingleNote(const QString &path);

  Manifest loadAndParseManifest(void *reader);
  Note *loadAndParseContent(void *reader, const Manifest &manifest);
  QVector<QPair<QString, QByteArray>>
  extractMediaWithDataFromArchive(void *reader, MediaStorage *storage);
  QStringList extractMediaFromNote(Note *note);
  void saveNoteToPad(const QString &path, const QJsonObject &manifest,
                     const QJsonObject &markup,
                     const QVector<QPair<QString, QByteArray>> &medias);
  detail::NoteSaveData prepareNoteSaveData(Note *note);

  QVector<QPair<QString, QByteArray>>
  loadMediaWithDataFromStorage(const QStringList &mediaPaths);

  QString makeNoteName();

  void prepareAndPushCreatedNote(Note *note);

public:
  NotesController(MediaStorage *storage, QObject *parent = nullptr);
  size_t notesCount() const;
  NotesModel *notes();
  Q_INVOKABLE void addEmptyNote();
  Q_INVOKABLE void saveNoteAsync(Note *note);

signals:
  void notesCountChanged();
  void couldNotSaveNote();

private slots:
  void onNoteAdded();
  void onNoteEdited();
};

} // namespace pad
