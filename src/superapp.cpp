#include <superapp.hpp>

namespace pad {

SuperApp::SuperApp(Theme *defaultTheme, MainWindow *mainWindow, QObject *parent)
    : QObject(parent), _theme(defaultTheme), _mainWindow(mainWindow) {}

Theme *SuperApp::theme() const { return _theme; }

void SuperApp::setTheme(Theme *theme) {
  _theme = theme;
  emit themeChanged();
}

} // namespace pad
