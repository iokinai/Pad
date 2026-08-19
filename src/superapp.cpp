#include <superapp.hpp>

namespace pad {

SuperApp::SuperApp(Theme *defaultTheme, MainWindow *mainWindow,
                   NotesController *notesController,
                   const QString &imageProviderPath, QObject *parent)
    : QObject(parent), _theme(defaultTheme), _mainWindow(mainWindow),
      _notesController(notesController), _imageProviderPath(imageProviderPath) {
}

Theme *SuperApp::theme() const { return _theme; }

void SuperApp::setTheme(Theme *theme) {
  _theme = theme;
  emit themeChanged();
}

NotesController *SuperApp::notesController() { return _notesController; }

} // namespace pad
