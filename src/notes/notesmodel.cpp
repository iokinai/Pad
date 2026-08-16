#include <notes/notesmodel.hpp>

namespace pad {

NotesModel::NotesModel(QVector<Note *> &&notes) : _notes(std::move(notes)) {}

int NotesModel::rowCount(const QModelIndex &parent) const {
  return _notes.size();
}

QVariant NotesModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return {};

  if (role != Roles::NoteRole)
    return {};

  return QVariant::fromValue(_notes[index.row()]);
}

QHash<int, QByteArray> NotesModel::roleNames() const {
  return {{Roles::NoteRole, QByteArrayLiteral("note")}};
}

void NotesModel::addEmptyNote() {
  beginInsertRows({}, 0, 0);
  _notes.insert(0, new Note("", {}, QDateTime::currentDateTime(), this));
  endInsertRows();

  emit noteAdded();
}

} // namespace pad
