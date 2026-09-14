#include <configure.hpp>
#include <superapp.hpp>
#include <theme/darktheme.hpp>
#include <theme/lighttheme.hpp>

namespace pad {

SuperApp::SuperApp(Theme::ThemeTag defaultTheme, MainWindow *mainWindow,
                   NotesController *notesController,
                   const QString &imageProviderPath,
                   LanguageController *languageController, QObject *parent)
    : QObject(parent), _mainWindow(mainWindow),
      _notesController(notesController), _theme(nullptr),
      _imageProviderPath(imageProviderPath),
      _languageController(languageController) {
  setTheme(defaultTheme);
}

Theme *SuperApp::theme() const { return _theme; }

NotesController *SuperApp::notesController() { return _notesController; }

void SuperApp::setTheme(Theme::ThemeTag themeTag) {
  if (_theme && themeTag == _theme->themeTag())
    return;

  delete _theme;

  switch (themeTag) {
  case Theme::ThemeTag::Light:
    _theme = new LightTheme();
    break;
  case Theme::ThemeTag::Dark:
    _theme = new DarkTheme();
    break;
  }

  emit themeChanged();
}

LanguageController *SuperApp::languageController() {
  return _languageController;
}

QString SuperApp::applicationName() const { return PAD_PROJECT; }

QString SuperApp::applicationVersion() const { return PAD_VERSION; }

QString SuperApp::applicationGitHub() const { return PAD_GITHUB; }

} // namespace pad
