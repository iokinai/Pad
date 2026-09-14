#include <notes/notesmodel.hpp>

namespace pad {

NotesModel::NotesModel(QVector<Note *> &&notes)
    : _loadedNotes(std::move(notes)), _layer2(), _current(&_loadedNotes) {}

int NotesModel::rowCount(const QModelIndex &parent) const {
  return _current->size();
}

QVariant NotesModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return {};

  if (role != Roles::NoteRole)
    return {};

  return QVariant::fromValue((*_current)[index.row()]);
}

QHash<int, QByteArray> NotesModel::roleNames() const {
  return {{Roles::NoteRole, QByteArrayLiteral("note")}};
}

void NotesModel::pushLoadedNoteNoteFront(Note *note) {
  beginInsertRows({}, 0, 0);
  _loadedNotes.push_front(note);
  endInsertRows();

  emit noteAdded();
}

void NotesModel::pushLoadedNoteBack(Note *note) {
  beginInsertRows({}, _loadedNotes.size(), _loadedNotes.size());
  _loadedNotes.push_back(note);
  endInsertRows();

  emit noteAdded();
}

void NotesModel::swapTo(std::optional<QVector<Note *>> set) {
  beginResetModel();

  if (set) {
    _layer2 = std::move(*set);
    _current = &_layer2;
  } else {
    _current = &_loadedNotes;
  }

  endResetModel();
}

// void NotesModel::pushNotes(QVector<Note *> &&notes) {
//   beginInsertRows({}, _notes.size(), _notes.size() + notes.size() - 1);
//   _notes.append(std::move(notes));
//   endInsertRows();

//   emit noteAdded();
// }

} // namespace pad
