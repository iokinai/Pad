import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ColumnLayout {
    id: root
    required property string title
    required property string description

    spacing: 3

    Text {
        text: root.title
        font.pixelSize: 14
        font.weight: Font.Bold
        color: superApp.theme.text
    }

    Text {
        text: root.description
        font.pixelSize: 12
        color: superApp.theme.textMuted
    }
}
