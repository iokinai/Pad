import QtQuick
import QtQuick.Controls.Basic

Button {
    id: root

    hoverEnabled: true

    leftPadding: 12
    rightPadding: 12
    topPadding: 6
    bottomPadding: 6

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    background: Rectangle {
        radius: 7
        color: root.hovered
            ? superApp.theme.accentSubtle
            : "transparent"//superApp.theme.card

        border.width: 1
        border.color: root.hovered
            ? superApp.theme.accent
            : superApp.theme.border

        Behavior on color {
            ColorAnimation {
                duration: 120
            }
        }

        Behavior on border.color {
            ColorAnimation {
                duration: 120
            }
        }
    }

    icon.color: root.hovered
        ? superApp.theme.accent
        : superApp.theme.textMuted

    icon.width: 13
    icon.height: 13

    palette.buttonText: root.hovered
        ? superApp.theme.accent
        : superApp.theme.textMuted

    font.pixelSize: 12
    font.family: superApp.theme.regularFontFamily
    font.weight: Font.Medium
    font.letterSpacing: 0.12

    Behavior on icon.color {
        ColorAnimation {
            duration: 120
        }
    }

    Behavior on palette.buttonText {
        ColorAnimation {
            duration: 120
        }
    }
}
