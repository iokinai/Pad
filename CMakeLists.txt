cmake_minimum_required(VERSION 3.15)
set(PROJECT Pad)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

qt_add_executable(${PROJECT}
  src/main.cpp
  src/superapp.hpp
  src/superapp.cpp
  src/theme/darktheme.hpp
  src/theme/darktheme.cpp
  src/theme/lighttheme.hpp
  src/theme/lighttheme.cpp
  src/theme/theme.hpp
  src/theme/theme.cpp
  src/mainwindow.hpp
  src/mainwindow.cpp
  src/pages/editor/note.hpp
  src/pages/editor/note.cpp
  src/pages/editor/node.hpp
  src/pages/editor/node.cpp
  src/pages/editor/textnode.hpp
  src/pages/editor/textnode.cpp
  src/pages/editor/editorcontroller.hpp
  src/pages/editor/editorcontroller.cpp
  assets.qrc
)

qt_add_qml_module(${PROJECT}
  URI PadUi
  VERSION 1.0
  QML_FILES
    src/qml/MainWindow.qml
    src/qml/pages/Editor.qml
    src/qml/components/ToolbarButton.qml
    src/qml/components/EditorNoteHeader.qml
    src/qml/components/EditorNoteTitle.qml
    src/qml/components/EditorTextNode.qml
    src/qml/components/Carette.qml
    src/qml/components/EditorBaseNode.qml
    src/qml/components/EditorInsertMenu.qml
    src/qml/components/EditorInsertTrigger.qml
    src/qml/components/EditorInsert.qml
    src/qml/components/NodeRemoveButton.qml
)

target_link_libraries(${PROJECT} PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Qml
  Qt6::Quick
)

target_include_directories(${PROJECT} PRIVATE
  src
)
