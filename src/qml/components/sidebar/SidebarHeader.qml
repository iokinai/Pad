import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import PadUi

RowLayout {
    id: root

    required property var notesController

    SidebarAppName {}

    Item {
        Layout.fillWidth: true
    }

    SidebarAddButton {
        Layout.preferredWidth: 28
        Layout.preferredHeight: 28

        onClicked: {
            notesController.addEmptyNote()
        }
    }
}
