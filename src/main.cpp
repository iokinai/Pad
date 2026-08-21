#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <language/language.hpp>
#include <language/languagecontroller.hpp>
#include <mainwindow.hpp>
#include <padimageprovider.hpp>
#include <pages/editor/node.hpp>
#include <storage/mediastorage.hpp>
#include <superapp.hpp>
#include <theme/darktheme.hpp>

constexpr const char *PAD_IMAGE_PROVIDER_PATH = "pad-images";

QVector<pad::Language *> loadLanguages() {
  return {
      new pad::Language("Русский", ":/lang/ru_RU.qm"),
      new pad::Language("English", ":/lang/en_US.qm"),
  };
}

void loadFonts() {
  QFontDatabase::addApplicationFont(":/assets/fonts/DMSans.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/JetBrainsMono.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/JetBrainsMono-Medium.ttf");
}

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  QTranslator translator;
  QLocale locale = QLocale::system();
  QVector<pad::Language *> languages = loadLanguages();

  QQmlApplicationEngine engine;

  app.installTranslator(&translator);

  pad::LanguageController *languageController = pad::LanguageController::create(
      std::move(languages), &translator, &engine, &locale);

  pad::Theme::ThemeTag defaultTheme = pad::Theme::Dark;
  pad::MainWindow mainWindow{QGuiApplication::primaryScreen()};
  pad::MediaStorage mediaStorage{};
  pad::NotesController notesController{&mediaStorage};
  pad::PadImageProvider *imageProvider =
      new pad::PadImageProvider{&mediaStorage};

  pad::SuperApp superApp{defaultTheme, &mainWindow, &notesController,
                         PAD_IMAGE_PROVIDER_PATH, languageController};

  engine.addImageProvider(PAD_IMAGE_PROVIDER_PATH, imageProvider);

  qmlRegisterUncreatableType<pad::Node>("PadUi", 1, 0, "Node", "Used for Enum");
  qmlRegisterUncreatableType<pad::MainWindow>("PadUi", 1, 0, "CxxMainWindow",
                                              "Used for Enum");
  qmlRegisterUncreatableType<pad::Theme>("PadUi", 1, 0, "Theme",
                                         "Used for Enum");

  engine.rootContext()->setContextProperty("superApp", &superApp);
  engine.loadFromModule("PadUi", "MainWindow");

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  auto code = app.exec();

  delete languageController;

  return code;
}
