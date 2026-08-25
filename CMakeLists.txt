cmake_minimum_required(VERSION 3.15)
set(PROJECT Pad)
project(${PROJECT} VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick LinguistTools)
find_package(minizip-ng CONFIG REQUIRED)

qt_standard_project_setup(REQUIRES 6.10)

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
  src/pages/editor/imagenode.hpp
  src/pages/editor/imagenode.cpp
  src/pages/editor/codenode.hpp
  src/pages/editor/codenode.cpp
  src/notes/notescontroller.hpp
  src/notes/notescontroller.cpp
  src/notes/notesmodel.hpp
  src/notes/notesmodel.cpp
  src/storage/manifest.hpp
  src/storage/manifest.cpp
  src/storage/markup.hpp
  src/storage/markup.cpp
  src/storage/mediastorage.hpp
  src/storage/mediastorage.cpp
  src/storage/storagecontroller.hpp
  src/storage/storagecontroller.cpp
  src/exceptions/invalidmanifest.hpp
  src/exceptions/invalidjson.hpp
  src/language/languagecontroller.hpp
  src/language/languagecontroller.cpp
  src/language/language.hpp
  src/language/language.cpp
  src/language/languagemodel.hpp
  src/language/languagemodel.cpp
  src/padimageprovider.hpp
  src/padimageprovider.cpp
  src/utils.hpp
  src/utils.cpp
  assets.qrc
)

qt_add_qml_module(${PROJECT}
  URI PadUi
  VERSION 1.0
  QML_FILES
    src/qml/MainWindow.qml
    src/qml/pages/Editor.qml
    src/qml/pages/EmptyState.qml
    src/qml/pages/Settings.qml
    src/qml/icons/Chevron.qml
    src/qml/icons/Sun.qml
    src/qml/icons/Moon.qml
    src/qml/components/Toggle.qml
    src/qml/components/Select.qml
    src/qml/components/editor/ToolbarButton.qml
    src/qml/components/editor/EditorHeader.qml
    src/qml/components/editor/EditorNoteTitle.qml
    src/qml/components/editor/EditorTextNode.qml
    src/qml/components/editor/EditorBaseNode.qml
    src/qml/components/editor/EditorInsertMenu.qml
    src/qml/components/editor/EditorInsertTrigger.qml
    src/qml/components/editor/EditorInsert.qml
    src/qml/components/editor/NodeRemoveButton.qml
    src/qml/components/editor/EditorImageNode.qml
    src/qml/components/editor/EditorCodeNode.qml
    src/qml/components/sidebar/SidebarAddButton.qml
    src/qml/components/sidebar/SidebarAppName.qml
    src/qml/components/sidebar/SidebarHeader.qml
    src/qml/components/sidebar/Sidebar.qml
    src/qml/components/sidebar/NotesListHeader.qml
    src/qml/components/sidebar/NotesList.qml
    src/qml/components/sidebar/NoteListItem.qml
    src/qml/components/sidebar/SidebarButton.qml
    src/qml/components/settings/SettingsHeader.qml
    src/qml/components/settings/AppearanceSection.qml
    src/qml/components/settings/LanguageSection.qml
    src/qml/components/settings/AboutSection.qml
    src/qml/components/settings/SettingsBasicSection.qml
    src/qml/components/settings/SectionTitleWithDescription.qml
    src/qml/components/OpenImageDialog.qml
    src/qml/components/Carette.qml
)

qt_add_translations(${PROJECT}
  TS_FILES
    lang/ru_RU.ts
    lang/en_US.ts
  RESOURCE_PREFIX
    "/lang"
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/configure.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/configure.hpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-g -gcodeview)
endif()

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  set_target_properties(${PROJECT} PROPERTIES
      MACOSX_BUNDLE_GUI_IDENTIFIER com.pad.Pad
      MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
      MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
      MACOSX_BUNDLE TRUE
      WIN32_EXECUTABLE TRUE
  )

  add_link_options(-Wl,-s)
  add_compile_options(-O2)
endif()

install(TARGETS ${PROJECT}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(DIRECTORY "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-mingw-dynamic/bin/"
    DESTINATION bin
    FILES_MATCHING PATTERN "*.dll"
)

target_link_libraries(${PROJECT} PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Qml
  Qt6::Quick
  MINIZIP::minizip-ng
)

target_include_directories(${PROJECT} PRIVATE
  src
  ${CMAKE_CURRENT_BINARY_DIR}
)
