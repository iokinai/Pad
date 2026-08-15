import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import PadUi

ColumnLayout {
    id: root

    required property var editor

    property var note: root.editor.note

    EditorHeader {
        Layout.preferredWidth: parent.width
        Layout.preferredHeight: 57
        createdAt: new Date()
        note: root.note
    }

    Item {
        Layout.preferredWidth: parent.width
        Layout.preferredHeight: 40
    }

    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        ColumnLayout {
            width: Math.min(720, parent.width - 80)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            EditorNoteTitle {
                Layout.fillWidth: true
                note: root.note
            }

            Item {
                Layout.preferredWidth: 1
                Layout.preferredHeight: 32
            }

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                clip: true
                model: root.editor.note

                delegate: Loader {
                    width: ListView.view.width

                    property var node: model.node

                    sourceComponent: {
                        switch (node.type) {
                        case Node.Text:
                            return textComponent
                        case Node.Image:
                            return imageComponent
                        case Node.Code:
                            return codeComponent
                        default:
                            return null
                        }
                    }

                    onLoaded: {
                        item.nodeValue = node
                        item.noteValue = root.editor.note
                    }
                }
            }

            Component {
                id: textComponent

                EditorBaseNode {
                    id: baseNode
                    width: parent.width
                    property var noteValue
                    property var nodeValue

                    note: baseNode.noteValue
                    currentNode: baseNode.nodeValue

                    EditorTextNode {
                        node: baseNode.nodeValue
                        width: parent.width
                    }
                }
            }

            Component {
                id: imageComponent

                EditorBaseNode {
                    id: baseNode
                    width: parent.width
                    property var noteValue
                    property var nodeValue

                    note: baseNode.noteValue
                    currentNode: baseNode.nodeValue

                    EditorImageNode {
                        node: baseNode.nodeValue
                        width: parent.width
                    }
                }
            }

            Component {
                id: codeComponent

                EditorBaseNode {
                    id: baseNode
                    width: parent.width
                    property var noteValue
                    property var nodeValue

                    note: baseNode.noteValue
                    currentNode: baseNode.nodeValue

                    EditorCodeNode {
                        node: baseNode.nodeValue
                        width: parent.width
                    }
                }
            }
        }
    }

    Item {
        Layout.preferredHeight: 80
    }
}
