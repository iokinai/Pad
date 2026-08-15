#pragma once

#include <QObject>
#include <QScreen>
#include <pages/editor/editorcontroller.hpp>

namespace pad {

class MainWindow : public QObject {
  Q_OBJECT

  int _initialWidth, _initialHeight;
  EditorController _editor;

  Q_PROPERTY(int initialWidth MEMBER _initialWidth CONSTANT)
  Q_PROPERTY(int initialHeight MEMBER _initialHeight CONSTANT)
  Q_PROPERTY(EditorController *editor READ editor CONSTANT)

public:
  enum CurrentPage {
    Editor,
  };

  Q_ENUM(CurrentPage)

  MainWindow(QScreen *screen, QObject *parent = nullptr);

  EditorController *editor();
};

} // namespace pad
