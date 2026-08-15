import QtQuick
import QtQuick.Controls
import PadUi

Rectangle {
    id: root
    property var editor

    Column {
        width: root.width

        Button {
            id: addButton

            width: 70
            height: 50
            text: "Add Text Node"
            onClicked: {
                editor.note.insertTextNode(null, "")
            }
        }

        ListView {
            clip: true

            width: parent.width
            height: root.height - addButton.height
            model: editor.note

            delegate: Loader {
                property var node: model.node

                width: ListView.view.width

                sourceComponent: {
                    switch (node.type) {
                    case Node.Text:
                        return textComponent
                    default:
                        return null
                    }
                }

                onLoaded: {
                    item.node = node
                }
            }
        }

        Component {
            id: textComponent

            TextNode {
                id: tn
                property var node
                anchors.topMargin: 8

                textNode: tn.node
            }
        }
    }
}
