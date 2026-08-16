import QtQuick
import QtQuick.Controls.Basic

Rectangle {
    id: root
    height: 400
    required property var node
    radius: 8
    border.width: 1
    border.color: superApp.theme.border
    color: superApp.theme.codeBlock

    Image {
        anchors.fill: parent
        source: root.node.src
        fillMode: Image.PreserveAspectFit
    }
}
