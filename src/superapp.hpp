#pragma once

#include <QObject>
#include <mainwindow.hpp>
#include <notes/notescontroller.hpp>
#include <theme/theme.hpp>

namespace pad {

class SuperApp : public QObject {
  Q_OBJECT

  Theme *_theme;
  MainWindow *_mainWindow;
  NotesController *_notesController;

  Q_PROPERTY(Theme *theme READ theme WRITE setTheme NOTIFY themeChanged)
  Q_PROPERTY(MainWindow *mainWindow MEMBER _mainWindow CONSTANT)
  Q_PROPERTY(NotesController *notesController READ notesController CONSTANT)

public:
  SuperApp(Theme *defaultTheme, MainWindow *mainWindow,
           NotesController *notesController, QObject *parent = nullptr);

  Theme *theme() const;
  void setTheme(Theme *theme);
  NotesController *notesController();

signals:
  void themeChanged();
};

} // namespace pad
