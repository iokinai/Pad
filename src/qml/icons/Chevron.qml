import QtQuick
import QtQuick.Shapes

Item {
    id: root

    property real size: 12
    property color color: superApp.theme.text
    property bool open: false

    implicitWidth: size
    implicitHeight: size

    rotation: open ? 90 : 0
    Behavior on rotation {
        NumberAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }

    Shape {
        anchors.fill: parent
        asynchronous: true
        antialiasing: true

        ShapePath {
            strokeColor: root.color
            strokeWidth: 1.6 * (root.size / 12)
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap
            joinStyle: ShapePath.RoundJoin

            startX: (4 / 12) * root.size
            startY: (2 / 12) * root.size

            PathLine {
                x: (8 / 12) * root.size
                y: (6 / 12) * root.size
            }
            PathLine {
                x: (4 / 12) * root.size
                y: (10 / 12) * root.size
            }
        }
    }
}
