#include <mainwindow.hpp>
#include <pages/editor/codenode.hpp>
#include <pages/editor/editorcontroller.hpp>
#include <pages/editor/imagenode.hpp>
#include <pages/editor/textnode.hpp>

namespace pad {

static QVector<Node *> getTestNodes() {
  QVector<Node *> nodes;

  nodes.push_back(new TextNode("123"));
  nodes.push_back(new TextNode("456"));
  nodes.push_back(new ImageNode(
      "file:///C:\\Users\\MegaD\\Downloads\\Новий проєкт (1).png"));
  nodes.push_back(
      new CodeNode("#include <iostream>\n\nint main() {\n\tstd::cout << "
                   "\"Hello World\" << std::endl;\n}"));

  return nodes;
}

MainWindow::MainWindow(QScreen *screen, QObject *parent)
    : QObject(parent), _editor(new Note("Test", std::move(getTestNodes()),
                                        QDateTime::currentDateTime(), this)) {
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
