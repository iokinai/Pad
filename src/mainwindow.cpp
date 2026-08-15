#include <mainwindow.hpp>
#include <pages/editor/editorcontroller.hpp>

namespace pad {

MainWindow::MainWindow(QScreen *screen, QObject *parent)
    : QObject(parent), _editor(this) {
  int w, h;

  if (screen == nullptr) {
    w = 1280;
    h = 720;
  } else {
    w = screen->size().width() * 0.7;
    h = screen->size().height() * 0.7;
  }

  _initialWidth = w;
  _initialHeight = h;
}

EditorController *MainWindow::editor() { return &_editor; }

} // namespace pad
