import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: root

    required property var editor

    color: superApp.theme.sidebar
    width: 248
    border.width: 1
    border.color: superApp.theme.border

    ColumnLayout {
        anchors.fill: parent

        SidebarHeader {
            notes: superApp.notesController.notes

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
            Layout.preferredWidth: root.width
            Layout.fillHeight: true
            notes: superApp.notesController.notes
            notesCount: superApp.notesController.notesCount
            editor: root.editor
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            Layout.leftMargin: 8
            Layout.rightMargin: 8
        }
    }
}
