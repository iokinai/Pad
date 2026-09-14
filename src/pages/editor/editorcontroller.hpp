#pragma once

#include <QCoro/QCoroFuture>
#include <QObject>
#include <pages/editor/note.hpp>
#include <storage/notescache.hpp>

namespace pad {

class EditorController : public QObject {
  Q_OBJECT

  Note *_currentNote;
  NotesCache *_cache;

  Q_PROPERTY(Note *currentNote READ currentNote WRITE setCurrentNote NOTIFY
                 currentNoteChanged)

public:
  explicit EditorController(Note *currentNote, NotesCache *cache,
                            QObject *parent = nullptr);

  Note *currentNote();
  void setCurrentNote(Note *note);

signals:
  void currentNoteChanged();
  void imageLoaded();
};

} // namespace pad
