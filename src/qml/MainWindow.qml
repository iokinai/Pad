import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import PadUi

ApplicationWindow {
    id: root
    visible: true
    property var mainWindow: superApp.mainWindow
    color: superApp.theme.editorBg

    width: root.mainWindow.initialWidth
    height: root.mainWindow.initialHeight

    MessageDialog {
        id: errorDialog
        buttons: MessageDialog.Ok
    }

    Connections {
        target: root.mainWindow
        function onCurrentPageChanged() {
            switch (root.mainWindow.currentPage) {
                case CxxMainWindow.Empty:
                    stackView.replace(emptyStatePage)
                    break
                case CxxMainWindow.Editor:
                    stackView.replace(editorPage)
                    break
            }
        }
    }

    Connections {
        target: superApp.notesController
        function onCouldNotSaveNote() {
            errorDialog.title = qsTr("Error saving note")
            errorDialog.text = qsTr("Could not save note")
            errorDialog.open()
        }
    }

    RowLayout {
        spacing: 0

        anchors.fill: parent

        Sidebar {
            editor: root.mainWindow.editor
            width: 248
            Layout.fillHeight: true
        }

        StackView {
            id: stackView
            initialItem: emptyStatePage
            Layout.fillHeight: true
            Layout.fillWidth: true

            pushEnter: Transition {}
            pushExit: Transition {}
            popEnter: Transition {}
            popExit: Transition {}
            replaceEnter: Transition {}
            replaceExit: Transition {}
        }

        Component {
            id: editorPage
            Editor {
                editor: root.mainWindow.editor
            }
        }

        Component {
            id: emptyStatePage
            EmptyState {
                notesController: superApp.notesController
            }
        }
    }
}
