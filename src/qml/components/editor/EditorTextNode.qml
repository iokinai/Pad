import QtQuick
import QtQuick.Controls.Basic

TextArea {
    id: root
    property var node

    text: node.text
    placeholderText: qsTr("Type here...", "Placeholder for the text block")
    placeholderTextColor: superApp.theme.textMuted
    font.pixelSize: 15
    font.family: superApp.theme.regularFontFamily
    font.weight: Font.Normal
    wrapMode: TextArea.Wrap
    color: superApp.theme.text

    cursorDelegate: Carette {
        cursorVisible: root.cursorVisible
    }

    background: Rectangle {
        color: "transparent"
    }

    onTextEdited: {
        if (!node) return
        node.text = root.text
    }
}
