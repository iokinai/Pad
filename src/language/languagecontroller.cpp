#include <QGuiApplication>
#include <QVector>
#include <language/languagecontroller.hpp>

namespace pad {

LanguageController::LanguageController(QVector<Language *> &&languages,
                                       Language *currentLanguage,
                                       QTranslator *translator,
                                       QQmlApplicationEngine *engine,
                                       QLocale *locale, QObject *parent)
    : QObject(parent), _languages(std::move(languages)),
      _currentLanguage(currentLanguage), _translator(translator),
      _engine(engine), _locale(locale) {}

LanguageController *LanguageController::create(QVector<Language *> &&languages,
                                               QTranslator *translator,
                                               QQmlApplicationEngine *engine,
                                               QLocale *locale,
                                               QObject *parent) {
  Language *initialLanguage = detectInitialLanguage(*locale, languages);
  auto lc = new LanguageController(std::move(languages), initialLanguage,
                                   translator, engine, locale, parent);
  lc->setLanguage(initialLanguage);
  return lc;
}

Language *LanguageController::detectInitialLanguage(
    const QLocale &locale, const QVector<Language *> &languages) {
  switch (locale.language()) {
  case QLocale::Language::Russian:
    return languages[0];
    break;
  case QLocale::Language::English:
  default:
    return languages[1];
    break;
  }
}

void LanguageController::switchLanguage(Language *language) {
  if (!language)
    return;

  _currentLanguage = language;

  qApp->removeTranslator(_translator);

  if (_translator->load(language->localizationFile())) {
    qApp->installTranslator(_translator);

    _engine->retranslate();
  } else {
    qDebug() << "Failed to load language file:" << language->localizationFile();
  }
}

void LanguageController::setLanguage(Language *language) {
  if (!language || _currentLanguage == language)
    return;

  switchLanguage(language);
  emit currentLanguageChanged();
}

LanguageModel *LanguageController::languages() { return &_languages; }

Language *LanguageController::currentLanguage() { return _currentLanguage; }

} // namespace pad
