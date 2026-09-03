import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import PadUi

Rectangle {
    id: root

    required property var editor
    required property var mainWindow

    property var notesController: superApp.notesController

    color: superApp.theme.sidebar
    border.width: 1
    border.color: superApp.theme.border

    property var selectedPage: root.mainWindow.currentPage
    property bool shouldShowSeachBar: false

    ColumnLayout {
        anchors.fill: parent

        SidebarHeader {
            notesController: root.notesController

            Layout.preferredWidth: root.width
            Layout.topMargin: 18
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            Layout.bottomMargin: 12
        }

        SearchBar {
            visible: root.shouldShowSeachBar
            notesController: root.notesController
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
            notes: root.notesController.notes
            notesCount: root.notesController.notesCount
            editor: root.editor
            notesController: root.notesController
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

        ColumnLayout {
            spacing: 2
            Layout.topMargin: 10
            Layout.bottomMargin: 10

            SidebarButton {
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                Layout.fillWidth: true
                Layout.preferredHeight: 36
                active: root.shouldShowSeachBar
                onClicked: {
                    root.shouldShowSeachBar = !root.shouldShowSeachBar
                }

                icon.source: "qrc:/assets/img/search.svg"
                icon.width: 14
                icon.height: 14
                text: qsTr("Search", "Search sidebar button")
            }

            SidebarButton {
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
}
