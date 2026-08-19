import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

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

        RowLayout {
            width: parent.width
            Text {
                font.pixelSize: 13
                font.weight: Font.Bold
                color: root.active
                    ? superApp.theme.accent
                    : superApp.theme.text
                text: note.title ? note.title : qsTr("Untitled", "Note title placeholder")
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
            }

            Rectangle {
                visible: root.note.hasUnsavedChanges
                width: 6
                height: 6
                color: superApp.theme.accent
                radius: width / 2
            }
        }

        Text {
            font.pixelSize: 11
            color: superApp.theme.textMuted
            text: "%1 · %2 %3".arg(Qt.formatDate(root.note.createdAt, "MMM d")).arg(root.note.nodesCount).arg(qsTr("blocks", "blocks count suffix"))
        }
    }
}
