#include <superapp.hpp>

namespace pad {

SuperApp::SuperApp(Theme *defaultTheme, MainWindow *mainWindow,
                   NotesController *notesController, QObject *parent)
    : QObject(parent), _theme(defaultTheme), _mainWindow(mainWindow),
      _notesController(notesController) {}

Theme *SuperApp::theme() const { return _theme; }

void SuperApp::setTheme(Theme *theme) {
  _theme = theme;
  emit themeChanged();
}

NotesController *SuperApp::notesController() { return _notesController; }

} // namespace pad
