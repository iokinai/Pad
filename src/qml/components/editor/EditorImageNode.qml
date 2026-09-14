import QtQuick
import QtQuick.Controls.Basic

Rectangle {
    id: root
    height: 400
    required property var node
    required property var editor
    radius: 8
    border.width: 1
    border.color: superApp.theme.border
    color: superApp.theme.codeBlock

    function formatPath() {
        return "image://%1/%2".arg(superApp.imageProviderPath).arg(root.node.src);
    }

    Connections {
        target: root.editor
        function onImageLoaded() {
            image.source = ""
            image.source = formatPath()
        }
    }

    Image {
        id: image
        anchors.fill: parent
        source: root.formatPath()
        fillMode: Image.PreserveAspectFit
    }
}
