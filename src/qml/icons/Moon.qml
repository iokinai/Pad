import QtQuick
import QtQuick.Shapes

Item {
    id: root

    property real size: 15
    property color color: superApp.theme.text

    implicitWidth: size
    implicitHeight: size

    Shape {
        anchors.fill: parent
        layer.enabled: true
        layer.samples: 4

        ShapePath {
            id: sp
            strokeColor: root.color
            strokeWidth: 1.5 * (root.size / 16)
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap

            PathSvg {
                path: "M13.5 10.5A6 6 0 0 1 5.5 2.5a6 6 0 1 0 8 8z"
            }
        }

        transform: Scale {
            xScale: root.size / 16
            yScale: root.size / 16
        }
    }
}
