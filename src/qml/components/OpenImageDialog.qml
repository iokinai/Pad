import QtQuick
import QtQuick.Dialogs

FileDialog {
    id: root

    required property var note
    required property var node
    required property bool above
    required property var notesController

    nameFilters: [
        "Images (*.png *.jpg *.jpeg *.webp *.bmp)"
    ]

    onAccepted: {
        const filePath = root.selectedFile
        const newPath = root.notesController.loadImageFromSystem(filePath);

        if (!newPath) {
            return
        }

        if (above) {
            note.insertImageAbove(root.node, newPath)
        } else {
            note.insertImageBelow(root.node, newPath)
        }
    }
}
