import QtQuick
import QtQuick.Controls.Basic

Button {
    id: root

    required property var notesController

    text: notesController.loadingNotes ? qsTr("Loading...") : qsTr("Load more")

    HoverHandler {
        id: hoverHandler
        cursorShape: Qt.PointingHandCursor
    }

    topPadding: 7
    bottomPadding: 7
    leftPadding: 10
    rightPadding: 10
    palette.buttonText: hoverHandler.hovered ? superApp.theme.accent : superApp.theme.textMuted
    font.pixelSize: 12
    font.family: superApp.theme.regularFontFamily
    font.weight: Font.Bold

    Behavior on palette.buttonText {
        ColorAnimation {
            duration: 120
        }
    }

    background: Rectangle {
        color: "transparent"
        radius: 8
        border.width: 1
        border.color: hoverHandler.hovered ? superApp.theme.accent : superApp.theme.border

        Behavior on border.color {
            ColorAnimation {
                duration: 120
            }
        }
    }
}
