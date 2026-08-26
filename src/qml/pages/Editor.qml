import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs
import PadUi

ColumnLayout {
    id: root

    required property var editor
    property var notesController: superApp.notesController
    property var note: root.editor.currentNote

    MessageDialog {
        id: errorDialog
        buttons: MessageDialog.Ok
    }

    Connections {
        target: root.notesController

        function onAddImageError(path) {
            errorDialog.title = qsTr("Error adding image")
            errorDialog.text = qsTr("Could not add image from %1").arg(path)
            errorDialog.open()
        }
    }

    Shortcut {
        sequences:  ["Ctrl+S", "StandardKey.Save"]
        enabled: root.note.hasUnsavedChanges
        onActivated: {
            superApp.notesController.qmlSaveNoteAsync(root.note)
        }
    }

    EditorHeader {
        Layout.preferredWidth: parent.width
        Layout.preferredHeight: 57
        createdAt: new Date()
        note: root.note
        notesController: superApp.notesController
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
                model: root.note

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
                        item.noteValue = root.note
                        item.notesControllerValue = root.notesController
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
                    property var notesControllerValue

                    note: baseNode.noteValue
                    currentNode: baseNode.nodeValue
                    notesController: baseNode.notesControllerValue

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
                    property var notesControllerValue

                    note: baseNode.noteValue
                    currentNode: baseNode.nodeValue
                    notesController: baseNode.notesControllerValue

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
                    property var notesControllerValue

                    note: baseNode.noteValue
                    currentNode: baseNode.nodeValue
                    notesController: baseNode.notesControllerValue

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
