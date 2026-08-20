#pragma once

#include <QObject>
#include <QString>

namespace pad {

class Language : public QObject {
  Q_OBJECT

  QString _name;
  QString _localizationFile;

  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QString localizationFile READ localizationFile CONSTANT)

public:
  Language(const QString &name, const QString &localizationFile,
           QObject *parent = nullptr);

  const QString &name() const;
  const QString &localizationFile() const;
};

} // namespace pad
