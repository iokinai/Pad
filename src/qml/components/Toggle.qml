import QtQuick
import QtQuick.Controls.Basic

Switch {
    id: control

    implicitWidth: 44
    implicitHeight: 24

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: control.toggle()
    }

    background: Rectangle {
        implicitWidth: 44
        implicitHeight: 24
        radius: 12

        color: control.checked ? superApp.theme.accent : superApp.theme.border

        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }

    indicator: Rectangle {
        id: handle

        x: control.checked ? 23 : 3
        y: 3
        width: 18
        height: 18
        radius: 9
        color: superApp.theme.accentButtonText

        Behavior on x {
            NumberAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
    }
}
