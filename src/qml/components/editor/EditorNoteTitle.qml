import QtQuick
import QtQuick.Controls.Basic

TextField {
    id: root

    required property var note

    text: root.note.title
    font.pixelSize: 28
    font.weight: Font.Bold
    color: superApp.theme.text
    anchors.bottomMargin: 32
    font.family: superApp.theme.regularFontFamily
    height: Math.max(36, root.implicitHeight)
    placeholderText: qsTr("Untitled", "Note title placeholder")
    placeholderTextColor: superApp.theme.textMuted

    background: Rectangle {
        color: "transparent"
    }

    cursorDelegate: Carette {
        cursorVisible: root.cursorVisible
    }

    onAccepted: {
        note.title = root.text
    }
}
