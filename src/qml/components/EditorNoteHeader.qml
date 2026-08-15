import QtQuick
import QtQuick.Controls.Basic
import PadUi

Rectangle {
    id: root

    required property var note
    required property date createdAt

    color: superApp.theme.sidebar
    border.color: superApp.theme.border
    border.width: 1

    Row {
        anchors.fill: parent
        spacing: 8
        anchors.leftMargin: 40
        anchors.rightMargin: 40
        anchors.topMargin: 12
        anchors.bottomMargin: 12

        Text {
            font.pixelSize: 11
            font.weight: Font.Bold
            color: superApp.theme.textMuted
            rightPadding: 4
            text: qsTr("ADD", "Add note text")
            anchors.verticalCenter: parent.verticalCenter
        }

        ToolbarButton {
            icon.source: "qrc:/assets/image/text.svg"
            text: qsTr("Text", "Append text button")
            onClicked: {
                note.insertTextBelow(null)
            }
        }

        ToolbarButton {
            icon.source: "qrc:/assets/image/image.svg"
            text: qsTr("Image", "Append image button")
        }

        ToolbarButton {
            icon.source: "qrc:/assets/image/code.svg"
            text: qsTr("Code", "Append code button")
        }

        Item {
            width: parent.width - x - dateText.width
            height: 1
        }

        Text {
            id: dateText
            font.pixelSize: 11
            color: superApp.theme.textMuted
            text: Qt.formatDate(root.createdAt, "MMM d")
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
