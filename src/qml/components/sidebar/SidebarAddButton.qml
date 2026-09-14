import QtQuick
import QtQuick.Controls.Basic

Button {
    id: root

    icon.source: "qrc:/assets/img/plus.svg"
    icon.color: hoverHandler.hovered ? superApp.theme.accent : superApp.theme.textMuted

    HoverHandler {
        id: hoverHandler
        cursorShape: Qt.PointingHandCursor
    }

    background: Rectangle {
        color: hoverHandler.hovered ? superApp.theme.accentSubtle : "transparent"
        border.width: 1
        border.color: hoverHandler.hovered ? superApp.theme.accent : superApp.theme.border
        radius: 7

        Behavior on border.color {
            ColorAnimation {
                duration: 120
            }
        }

        Behavior on color {
            ColorAnimation {
                duration: 120
            }
        }
    }

    Behavior on icon.color {
        ColorAnimation {
            duration: 120
        }
    }
}
