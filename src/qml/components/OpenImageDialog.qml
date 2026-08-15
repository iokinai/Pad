import QtQuick
import QtQuick.Dialogs

FileDialog {
    id: root

    required property var note
    required property var node
    required property bool above

    nameFilters: [
        "Images (*.png *.jpg *.jpeg *.webp *.bmp)"
    ]

    onAccepted: {
        if (above) {
            note.insertImageAbove(root.node, root.selectedFile)
        } else {
            note.insertImageBelow(root.node, root.selectedFile)
        }
    }
}
