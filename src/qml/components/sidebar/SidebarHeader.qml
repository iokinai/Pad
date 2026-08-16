import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

RowLayout {
    id: root

    required property var notes

    SidebarAppName {}

    Item {
        Layout.fillWidth: true
    }

    SidebarAddButton {
        Layout.preferredWidth: 28
        Layout.preferredHeight: 28

        onClicked: {
            notes.addEmptyNote()
        }
    }
}
