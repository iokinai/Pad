#pragma once

#include <QString>
#include <pages/editor/note.hpp>

namespace pad {

enum class LoadedNoteType {
  FullyLoaded,
  OnlyText,
};

struct LoadedNote {
  QString path;
  Note *note;
  LoadedNoteType type;
};

} // namespace pad
