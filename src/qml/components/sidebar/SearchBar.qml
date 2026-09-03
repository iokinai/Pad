import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Effects

Rectangle {
    width: parent.width
    height: 48

    required property var notesController

    color: "transparent"
    border.color: superApp.theme.border
    border.width: 1

    Column {
        anchors.fill: parent
        anchors.margins: 10

        Rectangle {
            width: parent.width
            height: 32

            color: superApp.theme.bg
            border.color: superApp.theme.border
            border.width: 1
            radius: 7

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: 7

                Item {
                    width: iconImage.width
                    height: iconImage.height
                    Layout.alignment: Qt.AlignHCenter

                    Image {
                        id: iconImage
                        source: "qrc:/assets/img/search.svg"
                        sourceSize: Qt.size(13, 13)
                        opacity: 0
                    }

                    MultiEffect {
                        anchors.fill: iconImage
                        source: iconImage
                        colorization: 1.0
                        colorizationColor: superApp.theme.textMuted
                    }
                }

                TextField {
                    id: searchField

                    Layout.fillWidth: true

                    placeholderText: qsTr("Search notes…")

                    color: superApp.theme.text
                    font.pixelSize: 13
                    font.family: superApp.theme.regularFontFamily

                    background: null

                    cursorDelegate: Carette {
                        cursorVisible: searchField.cursorVisible
                    }

                    onTextChanged: {
                        notesController.searchQuery = text
                    }
                }

                Button {
                    visible: searchField.text.length > 0

                    HoverHandler {
                        cursorShape: Qt.PointingHandCursor
                    }

                    background: Item {}

                    text: "×"

                    onClicked: {
                        searchField.clear()
                    }
                }
            }
        }
    }
}
