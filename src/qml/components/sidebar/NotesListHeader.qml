import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

RowLayout {
    id: root
    required property int notesCount
    spacing: 6

    property bool expanded: true

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    TapHandler {
        onTapped: {
            expanded = !expanded
        }
    }

    Chevron {
        size: 10
        color: superApp.theme.textMuted
        open: root.expanded
    }

    Text {
        text: qsTr("All Notes", "Sidebar notes list all notes")
        font.pixelSize: 10
        color: superApp.theme.textMuted
        font.family: superApp.theme.regularFontFamily
        font.weight: Font.Medium
    }

    Item {
        Layout.fillWidth: true
        height: 1
    }

    Text {
        text: notesCount.toString()
        font.pixelSize: 10
        color: superApp.theme.textMuted
        font.family: superApp.theme.regularFontFamily
        font.weight: Font.Medium
    }
}
