#pragma once

#include <QObject>
#include <QStandardPaths>
#include <notes/notesmodel.hpp>
#include <pages/editor/note.hpp>
#include <storage/manifest.hpp>
#include <storage/mediastorage.hpp>
#include <storage/storagecontroller.hpp>

namespace pad {

class NotesController : public QObject {
  Q_OBJECT

  static constexpr const int LOAD_PACKET_LENGTH = 10;

  size_t _loaded = 0;
  size_t _totalCount = 0;
  NotesModel _notesModel;
  MediaStorage *_storage;
  QHash<QString, Note *> _notePathMap;
  QSet<Note *> _unsavedNotes;
  StorageController *_storageController;
  bool _loadingNotes = false;

  Q_PROPERTY(size_t notesCount READ notesCount NOTIFY notesCountChanged)
  Q_PROPERTY(size_t loadedNotesCount READ loadedNotesCount NOTIFY
                 loadedNotesCountChanged)
  Q_PROPERTY(NotesModel *notes READ notes CONSTANT)
  Q_PROPERTY(bool loadingNotes READ loadingNotes NOTIFY loadingNotesChanged)

  QVector<QPair<QString, QByteArray>>
  loadMediaWithDataFromStorage(const QStringList &mediaPaths);

  void prepareAndPushCreatedNote(Note *note, bool front = false);

  void setLoadingNotes(bool v) noexcept;

public:
  NotesController(MediaStorage *storage, StorageController *storageController,
                  QObject *parent = nullptr);
  size_t notesCount() const;
  NotesModel *notes();
  Q_INVOKABLE void addEmptyNote();
  Q_INVOKABLE void saveNoteAsync(Note *note);
  Q_INVOKABLE void loadNotesAsync();
  bool loadingNotes() const noexcept;
  size_t loadedNotesCount() const noexcept;

signals:
  void notesCountChanged();
  void couldNotSaveNote();
  void loadingNotesChanged();
  void loadedNotesCountChanged();

private slots:
  void onNoteAdded();
  void onNoteEdited();
};

} // namespace pad
