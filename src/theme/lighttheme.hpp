#pragma once

#include <theme/theme.hpp>

namespace pad {

class LightTheme final : public Theme {
  Q_OBJECT

public:
  explicit LightTheme(QObject *parent = nullptr);

  QString bg() const override;
  QString sidebar() const override;
  QString sidebarHover() const override;
  QString editorBg() const override;
  QString card() const override;
  QString border() const override;
  QString borderStrong() const override;
  QString text() const override;
  QString textMuted() const override;
  QString accent() const override;
  QString accentSubtle() const override;
  QString codeBlock() const override;
  QString codeText() const override;
  QString insertBar() const override;
  QString placeholder() const override;
  QString regularFontFamily() const override;
  QString remove() const override;
  QString removeBorder() const override;
  QString removeText() const override;
  QString monospaceFontFamily() const override;
  QString accentButtonText() const override;
};

} // namespace pad
