import QtQuick
import QtQuick.Controls.Basic

Rectangle {
    id: root
    required property var node

    color: superApp.theme.codeBlock
    border.width: 1
    border.color: superApp.theme.border
    radius: 8
    height: Math.max(textArea.height, 24)

    TextArea {
        id: textArea

        topPadding: 12
        bottomPadding: 12
        leftPadding: 16
        rightPadding: 16
        width: root.width
        text: root.node.code
        color: superApp.theme.codeText
        font.pixelSize: 13
        font.family: superApp.theme.monospaceFontFamily
        font.weight: Font.Normal
        placeholderText: qsTr("// write code", "Write code placeholder")
        placeholderTextColor: superApp.theme.textMuted

        cursorDelegate: Carette {
            cursorVisible: textArea.cursorVisible
        }

        background: Rectangle {
            color: "transparent"
        }

        Text {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.rightMargin: 12

            text: qsTr("CODE", "Code text on the code node")
            font.pixelSize: 10
            font.family: superApp.theme.regularFontFamily
            font.weight: Font.Medium
            color: superApp.theme.textMuted
        }

        onTextEdited: {
            if (!node) return
            node.code = textArea.text
        }
    }
}
