import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Column {
    spacing: 8

    Text {
        font.pixelSize: 22
        font.weight: Font.Bold
        color: superApp.theme.text
        text: qsTr("Settings", "Settings page title")
    }

    Text {
        font.pixelSize: 13
        color: superApp.theme.textMuted
        text: qsTr("Configure your workspace preferences", "Settings page subtitle")
    }
}
