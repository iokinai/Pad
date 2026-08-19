#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <mainwindow.hpp>
#include <padimageprovider.hpp>
#include <pages/editor/node.hpp>
#include <storage/mediastorage.hpp>
#include <superapp.hpp>
#include <theme/darktheme.hpp>

constexpr const char *PAD_IMAGE_PROVIDER_PATH = "pad-images";

void loadFonts() {
  QFontDatabase::addApplicationFont(":/assets/fonts/DMSans.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/JetBrainsMono.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/JetBrainsMono-Medium.ttf");
}

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  pad::Theme *defaultTheme = new pad::DarkTheme();
  pad::MainWindow mainWindow{QGuiApplication::primaryScreen()};
  pad::MediaStorage mediaStorage{};
  pad::NotesController notesController{&mediaStorage};
  pad::PadImageProvider *imageProvider =
      new pad::PadImageProvider{&mediaStorage};

  pad::SuperApp superApp{defaultTheme, &mainWindow, &notesController,
                         PAD_IMAGE_PROVIDER_PATH};

  engine.addImageProvider(PAD_IMAGE_PROVIDER_PATH, imageProvider);

  qmlRegisterUncreatableType<pad::Node>("PadUi", 1, 0, "Node", "Used for Enum");
  qmlRegisterUncreatableType<pad::MainWindow>("PadUi", 1, 0, "CxxMainWindow",
                                              "Used for Enum");
  engine.rootContext()->setContextProperty("superApp", &superApp);
  engine.loadFromModule("PadUi", "MainWindow");

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  auto code = app.exec();

  delete defaultTheme;

  return code;
}
