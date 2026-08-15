import QtQuick
import QtQuick.Controls.Basic

Button {
    id: root
    width: 22
    height: 22
    opacity: nodeHovered ? 1 : 0
    enabled: nodeHovered

    required property bool nodeHovered
    required property var node

    palette.buttonText: hoverHandler.hovered ? superApp.theme.removeText : superApp.theme.textMuted
    font.pixelSize: 14

    text: "×"

    background: Rectangle {
        border.width: 1
        border.color: hoverHandler.hovered ? superApp.theme.removeBorder : superApp.theme.border
        radius: 5
        color: hoverHandler.hovered ? superApp.theme.remove : superApp.theme.sidebar
    }

    HoverHandler {
        id: hoverHandler
        cursorShape: Qt.PointingHandCursor
    }

    onClicked: {
        node.removeRequested()
    }

    Behavior on opacity {
        NumberAnimation { duration: 120 }
    }
}
