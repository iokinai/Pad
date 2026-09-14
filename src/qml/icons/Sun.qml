import QtQuick
import QtQuick.Shapes

Item {
    id: root

    property real size: 15
    property color color: superApp.theme.text

    implicitWidth: size
    implicitHeight: size

    Shape {
        id: shape
        anchors.fill: parent

        layer.enabled: true
        layer.samples: 4

        ShapePath {
            id: shapePath

            strokeColor: root.color
            strokeWidth: 1.5 * (root.size / 16)
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap

            property real sx: root.size / 16
            property real sy: root.size / 16

            PathAngleArc {
                centerX: 8 * shapePath.sx
                centerY: 8 * shapePath.sy
                radiusX: 3 * shapePath.sx
                radiusY: 3 * shapePath.sy
                startAngle: 0
                sweepAngle: 360
            }

            PathMove { x: 8 * shapePath.sx; y: 1 * shapePath.sy }
            PathLine { x: 8 * shapePath.sx; y: 3 * shapePath.sy }

            PathMove { x: 8 * shapePath.sx; y: 13 * shapePath.sy }
            PathLine { x: 8 * shapePath.sx; y: 15 * shapePath.sy }

            PathMove { x: 1 * shapePath.sx; y: 8 * shapePath.sy }
            PathLine { x: 3 * shapePath.sx; y: 8 * shapePath.sy }

            PathMove { x: 13 * shapePath.sx; y: 8 * shapePath.sy }
            PathLine { x: 15 * shapePath.sx; y: 8 * shapePath.sy }

            PathMove { x: 3.05 * shapePath.sx; y: 3.05 * shapePath.sy }
            PathLine { x: 4.46 * shapePath.sx; y: 4.46 * shapePath.sy }

            PathMove { x: 11.54 * shapePath.sx; y: 11.54 * shapePath.sy }
            PathLine { x: 12.95 * shapePath.sx; y: 12.95 * shapePath.sy }

            PathMove { x: 3.05 * shapePath.sx; y: 12.95 * shapePath.sy }
            PathLine { x: 4.46 * shapePath.sx; y: 11.54 * shapePath.sy }

            PathMove { x: 11.54 * shapePath.sx; y: 4.46 * shapePath.sy }
            PathLine { x: 12.95 * shapePath.sx; y: 3.05 * shapePath.sy }
        }
    }
}
