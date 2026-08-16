#pragma once

#include <QObject>
#include <pages/editor/note.hpp>

namespace pad {

class EditorController : public QObject {
  Q_OBJECT

  Note *_currentNote;

  Q_PROPERTY(Note *currentNote READ currentNote WRITE setCurrentNote NOTIFY
                 currentNoteChanged)
public:
  explicit EditorController(Note *currentNote, QObject *parent = nullptr);

  Note *currentNote();
  void setCurrentNote(Note *note);

signals:
  void currentNoteChanged();
};

} // namespace pad
