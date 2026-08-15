import QtQuick
import QtQuick.Controls.Basic

Rectangle {
    id: root
    required property bool cursorVisible

    color: superApp.theme.accent
    width: 1
    visible: root.cursorVisible
    radius: 2

    SequentialAnimation on opacity {
        running: root.cursorVisible
        loops: Animation.Infinite

        PropertyAction {
            value: 1
        }

        PauseAnimation {
            duration: 500
        }

        PropertyAction {
            value: 0
        }

        PauseAnimation {
            duration: 500
        }
    }
}
