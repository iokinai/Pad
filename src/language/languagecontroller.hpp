#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <language/language.hpp>
#include <language/languagemodel.hpp>

namespace pad {

class LanguageController : public QObject {
  Q_OBJECT

  LanguageModel _languages;
  Language *_currentLanguage;

  QTranslator *_translator;
  QQmlApplicationEngine *_engine;
  QLocale *_locale;

  Q_PROPERTY(LanguageModel *languages READ languages CONSTANT)
  Q_PROPERTY(Language *currentLanguage READ currentLanguage NOTIFY
                 currentLanguageChanged)

  void switchLanguage(Language *language);
  static Language *detectInitialLanguage(const QLocale &locale,
                                         const QVector<Language *> &languages);

  LanguageController(QVector<Language *> &&languages, Language *currentLanguage,
                     QTranslator *translator, QQmlApplicationEngine *engine,
                     QLocale *locale, QObject *parent = nullptr);

public:
  Q_INVOKABLE void setLanguage(Language *language);

  LanguageModel *languages();
  Language *currentLanguage();

  static LanguageController *create(QVector<Language *> &&languages,
                                    QTranslator *translator,
                                    QQmlApplicationEngine *engine,
                                    QLocale *locale, QObject *parent = nullptr);

signals:
  void currentLanguageChanged();
};

} // namespace pad
