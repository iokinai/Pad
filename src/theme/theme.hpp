#pragma once

#include <QObject>
#include <QString>
#include <qtmetamacros.h>

namespace pad {

class Theme : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString bg READ bg CONSTANT)
  Q_PROPERTY(QString sidebar READ sidebar CONSTANT)
  Q_PROPERTY(QString sidebarHover READ sidebarHover CONSTANT)
  Q_PROPERTY(QString editorBg READ editorBg CONSTANT)
  Q_PROPERTY(QString card READ card CONSTANT)
  Q_PROPERTY(QString border READ border CONSTANT)
  Q_PROPERTY(QString borderStrong READ borderStrong CONSTANT)
  Q_PROPERTY(QString text READ text CONSTANT)
  Q_PROPERTY(QString textMuted READ textMuted CONSTANT)
  Q_PROPERTY(QString accent READ accent CONSTANT)
  Q_PROPERTY(QString accentSubtle READ accentSubtle CONSTANT)
  Q_PROPERTY(QString codeBlock READ codeBlock CONSTANT)
  Q_PROPERTY(QString codeText READ codeText CONSTANT)
  Q_PROPERTY(QString insertBar READ insertBar CONSTANT)
  Q_PROPERTY(QString placeholder READ placeholder CONSTANT)
  Q_PROPERTY(QString regularFontFamily READ regularFontFamily CONSTANT)
  Q_PROPERTY(QString monospaceFontFamily READ monospaceFontFamily CONSTANT)
  Q_PROPERTY(QString remove READ remove CONSTANT)
  Q_PROPERTY(QString removeBorder READ removeBorder CONSTANT)
  Q_PROPERTY(QString removeText READ removeText CONSTANT)
  Q_PROPERTY(QString accentButtonText READ accentButtonText CONSTANT)

public:
  explicit Theme(QObject *parent = nullptr);
  ~Theme() = default;

  virtual QString bg() const = 0;
  virtual QString sidebar() const = 0;
  virtual QString sidebarHover() const = 0;
  virtual QString editorBg() const = 0;
  virtual QString card() const = 0;
  virtual QString border() const = 0;
  virtual QString borderStrong() const = 0;
  virtual QString text() const = 0;
  virtual QString textMuted() const = 0;
  virtual QString accent() const = 0;
  virtual QString accentSubtle() const = 0;
  virtual QString codeBlock() const = 0;
  virtual QString codeText() const = 0;
  virtual QString insertBar() const = 0;
  virtual QString placeholder() const = 0;
  virtual QString regularFontFamily() const = 0;
  virtual QString remove() const = 0;
  virtual QString removeBorder() const = 0;
  virtual QString removeText() const = 0;
  virtual QString monospaceFontFamily() const = 0;
  virtual QString accentButtonText() const = 0;
};

} // namespace pad
