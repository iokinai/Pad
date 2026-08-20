#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <language/language.hpp>
#include <language/languagecontroller.hpp>
#include <mainwindow.hpp>
#include <notes/notescontroller.hpp>
#include <theme/theme.hpp>

namespace pad {

class SuperApp : public QObject {
  Q_OBJECT

  Theme *_theme;
  MainWindow *_mainWindow;
  NotesController *_notesController;
  QString _imageProviderPath;
  LanguageController *_languageController;

  Q_PROPERTY(Theme *theme READ theme NOTIFY themeChanged)
  Q_PROPERTY(MainWindow *mainWindow MEMBER _mainWindow CONSTANT)
  Q_PROPERTY(NotesController *notesController READ notesController CONSTANT)
  Q_PROPERTY(QString imageProviderPath MEMBER _imageProviderPath CONSTANT)
  Q_PROPERTY(LanguageController *languageController MEMBER _languageController
                 CONSTANT)

  Q_PROPERTY(QString applicationName READ applicationName CONSTANT)
  Q_PROPERTY(QString applicationVersion READ applicationVersion CONSTANT)
  Q_PROPERTY(QString applicationGitHub READ applicationGitHub CONSTANT)

public:
  SuperApp(Theme *defaultTheme, MainWindow *mainWindow,
           NotesController *notesController, const QString &imageProviderPath,
           LanguageController *languageController, QObject *parent = nullptr);

  Theme *theme() const;
  // void setTheme(Theme *theme);
  Q_INVOKABLE void setTheme(Theme::ThemeTag themeTag);
  NotesController *notesController();
  LanguageController *languageController();

  QString applicationName() const;
  QString applicationVersion() const;
  QString applicationGitHub() const;

signals:
  void themeChanged();
};

} // namespace pad
