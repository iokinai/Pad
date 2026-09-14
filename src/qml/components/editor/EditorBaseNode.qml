import QtQuick
import QtQuick.Controls.Basic

Item {
    id: root
    required property var note
    required property var currentNode
    required property var notesController

    default property alias content: container.data
    width: parent.width
    height: column.height

    HoverHandler {
        id: hoverHandler
    }

    EditorInsert {
        note: root.note
        above: true
        currentNode: root.currentNode
        hovered: hoverHandler.hovered
        notesController: root.notesController
    }

    EditorInsert {
        note: root.note
        above: false
        currentNode: root.currentNode
        hovered: hoverHandler.hovered
        notesController: root.notesController
    }

    Column {
        id: column
        width: parent.width
        height: childrenRect.height

        Item {
            width: parent.width
            height: 10
        }

        NodeRemoveButton {
            node: root.currentNode
            nodeHovered: hoverHandler.hovered
            anchors.right: parent.right
            anchors.rightMargin: 5
        }

        Item {
            id: container
            width: parent.width
            height: childrenRect.height
        }

        Item {
            width: parent.width
            height: 10
        }

        Rectangle {
            width: parent.width
            height: 1
            color: superApp.theme.border
        }
    }
}
