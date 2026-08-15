#pragma once

#include <QObject>
#include <mainwindow.hpp>
#include <theme/theme.hpp>

namespace pad {

class SuperApp : public QObject {
  Q_OBJECT

  Theme *_theme;
  MainWindow *_mainWindow;

  Q_PROPERTY(Theme *theme READ theme WRITE setTheme NOTIFY themeChanged)
  Q_PROPERTY(MainWindow *mainWindow MEMBER _mainWindow CONSTANT)

public:
  SuperApp(Theme *defaultTheme, MainWindow *mainWindow,
           QObject *parent = nullptr);

  Theme *theme() const;
  void setTheme(Theme *theme);

signals:
  void themeChanged();
};

} // namespace pad
