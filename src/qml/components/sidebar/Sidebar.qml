import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import PadUi

Rectangle {
    id: root

    required property var editor
    required property var mainWindow

    color: superApp.theme.sidebar
    border.width: 1
    border.color: superApp.theme.border

    property var selectedPage: root.mainWindow.currentPage

    ColumnLayout {
        anchors.fill: parent

        SidebarHeader {
            notesController: superApp.notesController

            Layout.preferredWidth: root.width
            Layout.topMargin: 18
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            Layout.bottomMargin: 12
        }

        Rectangle {
            Layout.preferredHeight: 1
            Layout.fillWidth: true
            color: superApp.theme.border
        }

        NotesList {
            id: notesList
            Layout.preferredWidth: root.width
            Layout.fillHeight: true
            notes: superApp.notesController.notes
            notesCount: superApp.notesController.notesCount
            editor: root.editor
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            Layout.leftMargin: 8
            Layout.rightMargin: 8

            onAnyButtonClicked: {
                root.selectedPage = CxxMainWindow.Editor
            }
        }

        Item {
            Layout.fillHeight: true
        }

        SidebarButton {
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            Layout.leftMargin: 8
            Layout.rightMargin: 8
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            active: root.selectedPage === CxxMainWindow.Settings
            onClicked: {
                root.selectedPage = CxxMainWindow.Settings
                root.mainWindow.currentPage = CxxMainWindow.Settings
                notesList.currentIndex = -1
            }

            icon.source: "qrc:/assets/img/settings.svg"
            icon.width: 14
            icon.height: 14
            text: qsTr("Settings", "Sidebar settings button")
        }
    }
}
