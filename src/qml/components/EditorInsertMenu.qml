import QtQuick
import QtQuick.Controls.Basic

Popup {
    id: root

    property int leftRightMargin: 4
    property int topBottomMargin: 6
    required property var note
    required property var currentNode
    required property bool above

    OpenImageDialog {
        id: openImageDialog
        note: root.note
        node: root.currentNode
        above: root.above
    }

    signal anyButtonClicked()

    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    padding: topBottomMargin
    leftPadding: leftRightMargin
    rightPadding: leftRightMargin

    background: Rectangle {
        color: superApp.theme.card
        radius: 8
        border.color: superApp.theme.border
        border.width: 1
    }

    contentItem: Row {
        id: insertMenuRow
        spacing: 4

        ToolbarButton {
            text: "TEXT"
            height: 26
            onClicked: {
                if (root.above)
                    root.note.insertTextAbove(root.currentNode)
                else
                    root.note.insertTextBelow(root.currentNode)

                root.anyButtonClicked()
                root.close()
            }
        }

        ToolbarButton {
            text: "IMAGE"
            height: 26
            onClicked: {
                openImageDialog.open()

                root.anyButtonClicked()
                root.close()
            }
        }

        ToolbarButton {
            text: "CODE"
            height: 26
            onClicked: {
                root.anyButtonClicked()
                root.close()
            }
        }
    }
}
