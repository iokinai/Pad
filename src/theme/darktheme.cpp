#include <theme/darktheme.hpp>

namespace pad {

DarkTheme::DarkTheme(QObject *parent) : Theme(parent) {}

QString DarkTheme::bg() const { return QStringLiteral("#0e0f10"); }
QString DarkTheme::sidebar() const { return QStringLiteral("#141516"); }
QString DarkTheme::sidebarHover() const { return QStringLiteral("#1d1f21"); }
QString DarkTheme::editorBg() const { return QStringLiteral("#0e0f10"); }
QString DarkTheme::card() const { return QStringLiteral("#1a1c1e"); }
QString DarkTheme::border() const { return QStringLiteral("#252729"); }
QString DarkTheme::borderStrong() const { return QStringLiteral("#35383b"); }
QString DarkTheme::text() const { return QStringLiteral("#ddddd8"); }
QString DarkTheme::textMuted() const { return QStringLiteral("#636669"); }
QString DarkTheme::accent() const { return QStringLiteral("#c8a96e"); }
QString DarkTheme::accentSubtle() const { return QStringLiteral("#1fc8a96e"); }
QString DarkTheme::codeBlock() const { return QStringLiteral("#141618"); }
QString DarkTheme::codeText() const { return QStringLiteral("#c8c8c0"); }
QString DarkTheme::insertBar() const { return QStringLiteral("#1d1f21"); }
QString DarkTheme::placeholder() const { return QStringLiteral("#3a3d40"); }
QString DarkTheme::regularFontFamily() const {
  return QStringLiteral("DM Sans");
}
QString DarkTheme::monospaceFontFamily() const {
  return QStringLiteral("JetBrains Mono");
}

QString DarkTheme::remove() const { return QStringLiteral("#1fc83c3c"); }
QString DarkTheme::removeBorder() const { return QStringLiteral("#66c83c3c"); }
QString DarkTheme::removeText() const { return QStringLiteral("#e05555"); }
QString DarkTheme::accentButtonText() const {
  return QStringLiteral("#ffffff");
}
Theme::ThemeTag DarkTheme::themeTag() const { return ThemeTag::Dark; }

} // namespace pad
