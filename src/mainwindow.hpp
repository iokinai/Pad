#pragma once

#include <QObject>
#include <QScreen>
#include <pages/editor/editorcontroller.hpp>
#include <search/searchcontroller.hpp>

namespace pad {

class MainWindow : public QObject {
  Q_OBJECT

  int _initialWidth, _initialHeight;
  EditorController _editor;

  Q_PROPERTY(int initialWidth MEMBER _initialWidth CONSTANT)
  Q_PROPERTY(int initialHeight MEMBER _initialHeight CONSTANT)
  Q_PROPERTY(EditorController *editor READ editor CONSTANT)
  Q_PROPERTY(CurrentPage currentPage READ currentPage WRITE setCurrentPage
                 NOTIFY currentPageChanged)

public:
  enum CurrentPage {
    Editor,
    Empty,
    Settings,
  };

  Q_ENUM(CurrentPage)

  MainWindow(QScreen *screen, NotesCache *cache, QObject *parent = nullptr);
  EditorController *editor();
  CurrentPage currentPage();
  void setCurrentPage(CurrentPage page);

private:
  CurrentPage _currentPage;

signals:
  void currentPageChanged();
};

} // namespace pad
