import QtQuick
import QtQuick.Controls.Basic

Button {
    id: root
    required property var note
    required property bool active

    HoverHandler {
        id: hoverHandler
        cursorShape: Qt.PointingHandCursor
    }

    background: Rectangle {
        color: root.active
            ? superApp.theme.accentSubtle
            : hoverHandler.hovered
                ? superApp.theme.sidebarHover
                : "transparent"

        border.color: root.active
            ? superApp.theme.accent
            : "transparent"

        border.width: 1
        radius: 8

        Behavior on color {
            ColorAnimation { duration: 120 }
        }
    }

    Column {
        spacing: 2
        anchors.topMargin: 8
        anchors.bottomMargin: 8
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.fill: parent

        Text {
            font.pixelSize: 13
            font.weight: Font.Bold
            color: root.active
                ? superApp.theme.accent
                : superApp.theme.text
            text: note.title ? note.title : qsTr("Untitled", "Note title placeholder")
        }

        Text {
            font.pixelSize: 11
            color: superApp.theme.textMuted
            text: "%1 · %2 %3".arg(Qt.formatDate(root.note.createdAt, "MMM d")).arg(root.note.nodesCount).arg(qsTr("blocks", "blocks count suffix"))
        }
    }
}
