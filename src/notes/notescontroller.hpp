#pragma once

#include <QObject>
#include <notes/notesmodel.hpp>
#include <pages/editor/note.hpp>

namespace pad {

class NotesController : public QObject {
  Q_OBJECT

  size_t _loaded = 0;
  size_t _totalCount = 0;
  NotesModel _notesModel;

  Q_PROPERTY(size_t notesCount READ notesCount NOTIFY notesCountChanged)
  Q_PROPERTY(NotesModel *notes READ notes CONSTANT)
public:
  NotesController(QObject *parent = nullptr);
  size_t notesCount() const;
  NotesModel *notes();

signals:
  void notesCountChanged();

private slots:
  void onNoteAdded();
};

} // namespace pad
