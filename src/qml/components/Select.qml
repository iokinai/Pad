import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ComboBox {
    id: control

    implicitWidth: Math.max(140, contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: 34

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.NoButton
    }

    contentItem: Text {
        leftPadding: 12
        rightPadding: 32

        text: control.displayText
        font.pixelSize: 13
        font.weight: Font.Medium
        color: superApp.theme.text
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        color: superApp.theme.bg
        radius: 8
        border.width: 1
        border.color: control.activeFocus || control.popup.visible
                      ? superApp.theme.accent
                      : superApp.theme.border

        Behavior on border.color {
            ColorAnimation { duration: 150 }
        }
    }

    indicator: Canvas {
        x: control.width - width - 12
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 10
        height: 6

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            ctx.moveTo(0, 0);
            ctx.lineTo(5, 6);
            ctx.lineTo(10, 0);
            ctx.strokeStyle = superApp.theme.textMuted;
            ctx.lineWidth = 1.5;
            ctx.stroke();
        }
    }

    popup: Popup {
        y: control.height + 4
        width: control.width
        implicitHeight: Math.min(200, contentItem.implicitHeight + padding * 2)
        padding: 4

        background: Rectangle {
            color: superApp.theme.bg
            border.color: superApp.theme.border
            border.width: 1
            radius: 8
        }

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    delegate: ItemDelegate {
        id: delegate
        width: ListView.view ? ListView.view.width : control.width
        height: 32

        contentItem: Text {
            text: (control.textRole && model[control.textRole] !== undefined)
                  ? model[control.textRole]
                  : (typeof modelData !== "undefined" ? modelData : model)
            font.pixelSize: 13
            font.weight: Font.Medium
            color: delegate.highlighted ? superApp.theme.accent : superApp.theme.text
            verticalAlignment: Text.AlignVCenter
            leftPadding: 8
            elide: Text.ElideRight
        }

        background: Rectangle {
            x: 2
            y: 1
            width: parent.width - 4
            height: parent.height - 2
            color: delegate.highlighted ? superApp.theme.sidebar : "transparent"
            radius: 6
        }

        highlighted: control.highlightedIndex === index
    }
}
