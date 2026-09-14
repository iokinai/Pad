import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ColumnLayout {
    default property alias content: container.data

    id: root

    required property string title
    spacing: 16

    implicitWidth: card.implicitWidth
    implicitHeight: card.implicitHeight + spacing + ttl.implicitHeight

    Text {
        id: ttl
        text: root.title
        font.pixelSize: 10
        font.weight: Font.Bold
        color: superApp.theme.textMuted
    }

    Rectangle {
        id: card
        color: superApp.theme.sidebar
        border.color: superApp.theme.border
        border.width: 1
        radius: 12
        implicitWidth: container.width + 20 * 2
        implicitHeight: container.height + 16 * 2
        Layout.fillWidth: true
        Layout.preferredWidth: implicitWidth
        Layout.preferredHeight: implicitHeight

        ColumnLayout {
            id: container
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            anchors.topMargin: 16
            anchors.bottomMargin: 16
        }
    }
}
