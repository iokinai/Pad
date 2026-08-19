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

void NotesModel::pushNote(Note *note) {
  beginInsertRows({}, _notes.size(), _notes.size());
  _notes.push_back(note);
  endInsertRows();

  emit noteAdded();
}

void NotesModel::pushNotes(QVector<Note *> &&notes) {
  beginInsertRows({}, _notes.size(), _notes.size() + notes.size() - 1);
  _notes.append(std::move(notes));
  endInsertRows();

  emit noteAdded();
}

} // namespace pad
