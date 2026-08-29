#pragma once

#include <QCoro/QCoroTask>
#include <QFuture>
#include <QObject>
#include <QTimer>
#include <storage/storagecontroller.hpp>

namespace pad {

class NotesCache : public QObject {
  Q_OBJECT

  StorageController *_storageController;
  QVector<LoadedNote> _notes;
  size_t _totalCount;

public:
  NotesCache(StorageController *storageController, QObject *parent = nullptr);

  QCoro::Task<void> loadNotesAsync();

  const QVector<LoadedNote> &allNotes() const;

  QVector<Note *> fullyLoadedNotes() const;

  size_t totalCount() const;

  QString addMediaFromSystem(const QString &systemPath);

  QString makeNoteName() const noexcept;

  void createEmptyNote();

  QCoro::Task<void> saveNoteAsync(Note *note);

  QCoro::Task<void> requestFullyLoadedNotesAsync(int since, int count);

signals:
  void anyNoteLoaded();
  void fullNotesLoaded(QVector<Note *> notes);
  void noteCreated(Note *note);
  void totalCountChanged();
  void loaded();
};

} // namespace pad
