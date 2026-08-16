import QtQuick
import QtQuick.Controls.Basic

Row {
    spacing: 8

    Rectangle {
        width: 26
        height: 26
        radius: 7
        color: superApp.theme.accent

        Image {
            anchors.centerIn: parent
            width: 13
            height: 13
            source: "qrc:/assets/img/doc.svg"
        }
    }

    Text {
        font.pixelSize: 14
        font.weight: Font.Bold
        color: superApp.theme.text
        anchors.verticalCenter: parent.verticalCenter
        text: "Pad"
    }
}
