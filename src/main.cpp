#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <mainwindow.hpp>
#include <pages/editor/node.hpp>
#include <superapp.hpp>
#include <theme/darktheme.hpp>

void loadFonts() {
  QFontDatabase::addApplicationFont(":/assets/fonts/DMSans.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/JetBrainsMono.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/JetBrainsMono-Medium.ttf");
}

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  pad::Theme *defaultTheme = new pad::DarkTheme();
  pad::MainWindow mainWindow{QGuiApplication::primaryScreen()};

  pad::SuperApp superApp{defaultTheme, &mainWindow};

  QQmlApplicationEngine engine;

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
