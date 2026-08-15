import QtQuick
import QtQuick.Controls.Basic

TextArea {
    required property var textNode

    background: Rectangle {
        color: "white"
        border.color: "black"
        border.width: 2
        radius: 4
    }

    color: "#000"
    placeholderText: "Enter text..."
    placeholderTextColor: "#888888"
    text: textNode ? textNode.text : ""

    onTextEdited: {
        textNode.text = text
    }
}
