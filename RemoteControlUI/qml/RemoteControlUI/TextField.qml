import QtQuick 1.1

Rectangle {
    property string text: edit.text
    property string placeholderText: ""

    radius: 5
    border.width: 1
    border.color: "gray"
    color: "#EEE"
    width: 150
    height: 17
    clip: true

    Text {
        id: placeholder
        text: parent.placeholderText
        visible: !edit.focus && edit.text.length === 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 3
        color: "#999"
    }

    TextEdit {
        id: edit
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 3
        anchors.verticalCenter: parent.verticalCenter
    }
}
