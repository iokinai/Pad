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
  QVector<Note *> _notes;
  QTimer *_timer;
  bool _loaded = false;

  QFuture<void> loadNotesToCacheAsync();

public:
  NotesCache(StorageController *storageController, QObject *parent = nullptr);
  QCoro::Task<QVector<Note *>> notesAsync();

public slots:
  void clearCache();
};

} // namespace pad
