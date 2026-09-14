#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QVector>
#include <pages/editor/note.hpp>

namespace pad {

class NotesModel : public QAbstractListModel {
  Q_OBJECT

  QVector<Note *> _loadedNotes;
  QVector<Note *> _layer2;
  QVector<Note *> *_current;

public:
  enum Roles { NoteRole = Qt::UserRole + 1 };

  Q_ENUM(Roles)

  NotesModel(QVector<Note *> &&notes);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const override;

  void swapTo(std::optional<QVector<Note *>> set);

public:
  void pushLoadedNoteNoteFront(Note *note);
  void pushLoadedNoteBack(Note *note);
  // void pushNotes(QVector<Note *> &&notes);

signals:
  void noteAdded();
};

} // namespace pad
