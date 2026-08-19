import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Effects

ColumnLayout {
    id: root
    required property var notesController

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignCenter

        spacing: 16

        Item {
            Layout.preferredWidth: 32
            Layout.preferredHeight: 32
            Layout.alignment: Qt.AlignCenter

            Image {
                id: icon
                sourceSize: Qt.size(32, 32)
                width: 32
                height: 32
                Layout.alignment: Qt.AlignHCenter
                source: "qrc:/assets/img/doc.svg"
            }

            MultiEffect {
                anchors.fill: icon
                source: icon
                colorization: 1.0
                colorizationColor: superApp.theme.textMuted
            }
        }

        Text {
            font.pixelSize: 14
            color: superApp.theme.textMuted
            text: qsTr("No note selected", "No note selected empty state")
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            text: qsTr("New note", "Create note empty state")
            onClicked: notesController.addEmptyNote()
            Layout.alignment: Qt.AlignHCenter

            topPadding: 8
            bottomPadding: 8
            leftPadding: 20
            rightPadding: 20

            background: Rectangle {
                color: superApp.theme.accent
                radius: 8
            }

            font.pixelSize: 13
            font.weight: Font.Bold
            font.family: superApp.theme.regularFontFamily
            palette.buttonText: superApp.theme.accentButtonText

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }
        }
    }
}
