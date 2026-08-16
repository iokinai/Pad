import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ColumnLayout {
    id: root

    required property var editor
    required property var notes
    required property int notesCount

    property int currentIndex: 0

    NotesListHeader {
        id: header
        notesCount: root.notesCount
        Layout.topMargin: 4
        Layout.bottomMargin: 4
        Layout.leftMargin: 8
        Layout.rightMargin: 8
    }

    ListView {
        id: listView
        model: root.notes
        spacing: 2
        Layout.fillHeight: true
        Layout.fillWidth: true
        clip: true

        delegate: NoteListItem {

            visible: header.expanded
            height: 56
            width: parent.width
            required property int index
            active: index === root.currentIndex
            onClicked: {
                root.currentIndex = index
                editor.currentNote = note
            }
        }
    }
}
