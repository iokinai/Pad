#include <mainwindow.hpp>
#include <pages/editor/codenode.hpp>
#include <pages/editor/editorcontroller.hpp>
#include <pages/editor/imagenode.hpp>
#include <pages/editor/textnode.hpp>

namespace pad {

MainWindow::MainWindow(QScreen *screen, SearchController *searchController,
                       QObject *parent)
    : QObject(parent), _editor(nullptr), _searchController(searchController) {
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

MainWindow::CurrentPage MainWindow::currentPage() { return _currentPage; }

void MainWindow::setCurrentPage(CurrentPage page) {
  _currentPage = page;
  emit currentPageChanged();
}

} // namespace pad
