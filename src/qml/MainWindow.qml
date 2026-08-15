import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import PadUi

ApplicationWindow {
    id: root
    visible: true
    property var mainWindow: superApp.mainWindow
    color: superApp.theme.editorBg

    width: root.mainWindow.initialWidth
    height: root.mainWindow.initialHeight

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: editorPage
    }

    Component {
        id: editorPage
        Editor {
            editor: root.mainWindow.editor
        }
    }
}
