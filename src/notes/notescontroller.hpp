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

  static constexpr const int LOAD_FIRST_COUNT = 10;

  size_t _loaded = 0;
  size_t _totalCount = 0;
  NotesModel _notesModel;
  MediaStorage *_storage;
  QHash<QString, Note *> _notePathMap;
  QSet<Note *> _unsavedNotes;
  StorageController *_storageController;

  Q_PROPERTY(size_t notesCount READ notesCount NOTIFY notesCountChanged)
  Q_PROPERTY(NotesModel *notes READ notes CONSTANT)

  void loadNotesAsync();

  QVector<QPair<QString, QByteArray>>
  loadMediaWithDataFromStorage(const QStringList &mediaPaths);

  QString makeNoteName();

  void prepareAndPushCreatedNote(Note *note);

public:
  NotesController(MediaStorage *storage, StorageController *storageController,
                  QObject *parent = nullptr);
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
