import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import PadUi

Rectangle {
    id: root

    required property var note
    required property var notesController
    required property date createdAt

    color: superApp.theme.sidebar
    border.color: superApp.theme.border
    border.width: 1

    RowLayout {
        anchors.fill: parent
        spacing: 8
        anchors.leftMargin: 40
        anchors.rightMargin: 40
        anchors.topMargin: 12
        anchors.bottomMargin: 12

        OpenImageDialog {
            id: openImageDialog
            note: root.note
            node: null
            above: false
        }

        Text {
            font.pixelSize: 11
            font.weight: Font.Bold
            color: superApp.theme.textMuted
            rightPadding: 4
            text: qsTr("ADD", "Add node text")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        ToolbarButton {
            Layout.preferredHeight: 32
            icon.source: "qrc:/assets/image/text.svg"
            text: qsTr("Text", "Append text button")
            onClicked: {
                note.insertTextBelow(null)
            }
        }

        ToolbarButton {
            Layout.preferredHeight: 32
            icon.source: "qrc:/assets/image/image.svg"
            text: qsTr("Image", "Append image button")
            onClicked: {
                openImageDialog.open()
            }
        }

        ToolbarButton {
            Layout.preferredHeight: 32
            icon.source: "qrc:/assets/image/code.svg"
            text: qsTr("Code", "Append code button")
            onClicked: {
                note.insertCodeBelow(null)
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
        }

        Button {
            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }

            background: Rectangle {
                color: superApp.theme.accent
                radius: 7
            }

            visible: root.note.hasUnsavedChanges
            text: qsTr("Save", "Save note button")
            font.pixelSize: 12
            font.weight: Font.Bold
            palette.buttonText: superApp.theme.accentButtonText

            onClicked: {
                root.notesController.saveNoteAsync(root.note)
            }
        }

        Text {
            id: dateText
            font.pixelSize: 11
            color: superApp.theme.textMuted
            text: Qt.formatDate(root.createdAt, "MMM d")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
    }
}
