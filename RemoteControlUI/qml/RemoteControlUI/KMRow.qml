import QtQuick 2.0

RowBase {
    id: row
    height: 50

    Rectangle {
        id: line
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 5
        anchors.rightMargin: anchors.leftMargin
        color: "lightgray"
        height: 1
        visible: index != 0
    }
    SmoothText {
        height: parent.height
        anchors.top: line.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: anchors.leftMargin
        text: name
        font.pixelSize: 40
        font.bold: row.highlighted
        font.italic: row.highlighted
        color: "lightgray"
    }
}
