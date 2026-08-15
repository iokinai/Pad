import QtQuick
import QtQuick.Controls.Basic

import QtQuick
import QtQuick.Controls.Basic

Popup {
    id: root

    required property bool above
    property bool isOpen: false

    signal buttonHoveredChanged(bool buttonHovered)

    padding: 0
    background: null

    closePolicy: Popup.NoAutoClose

    contentItem: Button {
        id: button

        font.family: superApp.theme.regularFontFamily
        font.pixelSize: 10
        font.weight: Font.Normal

        palette.buttonText: root.isOpen ? superApp.theme.accent : superApp.theme.textMuted
        text: root.above ? qsTr("+ Add Above", "Add root above button") : qsTr("+ Add Below", "Add root below button")

        HoverHandler {
            cursorShape: Qt.PointingHandCursor
            onHoveredChanged: {
                root.buttonHoveredChanged(hovered)
            }
        }

        background: Rectangle {
            color: root.isOpen ? superApp.theme.accentSubtle : superApp.theme.insertBar
            border.width: 1
            border.color: root.isOpen ? superApp.theme.accent : superApp.theme.border
            radius: 5
        }

        onClicked: {
            root.isOpen = !root.isOpen
        }
    }
}
