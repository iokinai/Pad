import QtQuick
import QtQuick.Controls.Basic

Item {
    id: root

    required property var note
    required property var currentNode
    required property bool above
    required property bool hovered

    property bool isMenuOpened: false
    property bool buttonHovered: false
    property bool shouldShow: root.buttonHovered || root.hovered || root.isMenuOpened

    y: root.above ? -height + 8 : parent.height

    width: insertTrigger.width
    height: insertTrigger.height

    onShouldShowChanged: {
        if (!root.shouldShow) {
            insertTrigger.isOpen = false
        }
    }

    EditorInsertTrigger {
        id: insertTrigger
        visible: root.shouldShow
        above: root.above

        onButtonHoveredChanged: (v) => {
            root.buttonHovered = v
        }

        onIsOpenChanged: {
            root.isMenuOpened = !root.isMenuOpened
            if (root.isMenuOpened) {
                insertMenu.open()
            } else {
                insertMenu.close()
            }
        }
    }

    EditorInsertMenu {
        id: insertMenu

        x: 0
        y: root.above ? -height - 4 : insertTrigger.height

        note: root.note
        currentNode: root.currentNode
        above: root.above

        onAnyButtonClicked: {
            insertTrigger.isOpen = false
        }

        onClosed: {
            insertTrigger.isOpen = false
            root.isMenuOpened = false
        }
    }
}
