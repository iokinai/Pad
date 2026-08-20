#include <theme/lighttheme.hpp>

namespace pad {

LightTheme::LightTheme(QObject *parent) : Theme(parent) {}

QString LightTheme::bg() const { return QStringLiteral("#f5f4f2"); }
QString LightTheme::sidebar() const { return QStringLiteral("#ffffff"); }
QString LightTheme::sidebarHover() const { return QStringLiteral("#f0efed"); }
QString LightTheme::editorBg() const { return QStringLiteral("#f5f4f2"); }
QString LightTheme::card() const { return QStringLiteral("#ffffff"); }
QString LightTheme::border() const { return QStringLiteral("#e6e4e0"); }
QString LightTheme::borderStrong() const { return QStringLiteral("#d0cdc8"); }
QString LightTheme::text() const { return QStringLiteral("#1c1a18"); }
QString LightTheme::textMuted() const { return QStringLiteral("#9b9590"); }
QString LightTheme::accent() const { return QStringLiteral("#b8944a"); }
QString LightTheme::accentSubtle() const { return QStringLiteral("#1ab8944a"); }
QString LightTheme::codeBlock() const { return QStringLiteral("#1a1c1e"); }
QString LightTheme::codeText() const { return QStringLiteral("#c8c8c0"); }
QString LightTheme::insertBar() const { return QStringLiteral("#eceae7"); }
QString LightTheme::placeholder() const { return QStringLiteral("#c8c5c0"); }
QString LightTheme::regularFontFamily() const {
  return QStringLiteral("DM Sans");
}
QString LightTheme::monospaceFontFamily() const {
  return QStringLiteral("JetBrains Mono");
}

QString LightTheme::remove() const { return QStringLiteral("#1fc83c3c"); }
QString LightTheme::removeBorder() const { return QStringLiteral("66c83c3c"); }
QString LightTheme::removeText() const { return QStringLiteral("#e05555"); }
QString LightTheme::accentButtonText() const {
  return QStringLiteral("#ffffff");
}

Theme::ThemeTag LightTheme::themeTag() const { return ThemeTag::Light; }

} // namespace pad
