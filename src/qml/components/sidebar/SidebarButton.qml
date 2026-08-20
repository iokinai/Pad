import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Effects

Button {
    id: root

    required property bool active

    property color contentColor: root.active
                            ? superApp.theme.accent
                            : hoverHandler.hovered
                                ? superApp.theme.text
                                : superApp.theme.textMuted

    hoverEnabled: true
    icon.width: 14
    icon.height: 14

    topPadding: 8
    bottomPadding: 8
    leftPadding: 12
    rightPadding: 12
    palette.buttonText: root.contentColor
    font.pixelSize: 13
    font.family: superApp.theme.regularFontFamily
    font.weight: Font.Normal

    background: Rectangle {
        color: root.active
                ? superApp.theme.accentSubtle
                : hoverHandler.hovered
                    ? superApp.theme.sidebarHover
                    : "transparent"

        Behavior on color {
            ColorAnimation {
                duration: 120
            }
        }

        radius: 7
    }

    contentItem: Row {
        leftPadding: 12
        spacing: 9
        anchors.fill: parent

        Item {
            width: iconImage.width
            height: iconImage.height
            anchors.verticalCenter: parent.verticalCenter

            Image {
                id: iconImage

                source: root.icon.source
                width: root.icon.width
                fillMode: Image.PreserveAspectFit
                opacity: 0
            }

            MultiEffect {
                anchors.fill: iconImage

                source: iconImage
                colorization: 1.0
                colorizationColor: root.contentColor
            }
        }

        Text {
            text: root.text
            font: root.font
            color: root.contentColor

            anchors.verticalCenter: parent.verticalCenter

            elide: Text.ElideRight
        }
    }

    HoverHandler {
        id: hoverHandler
        cursorShape: Qt.PointingHandCursor
    }

    Behavior on contentColor {
        ColorAnimation {
            duration: 120
        }
    }
}
