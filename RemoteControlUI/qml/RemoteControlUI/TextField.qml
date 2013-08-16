import QtQuick 1.1

Rectangle {
    property string text: ""
    property string placeholderText: ""

    radius: 5
    border.width: 1
    border.color: "gray"
    color: "#EEE"
    width: 150
    height: 80
    clip: true

    function setText(str) {
        edit.text = str;
    }

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
        anchors.fill: parent
        anchors.leftMargin: 3
        verticalAlignment: TextEdit.AlignVCenter

        onTextChanged: parent.text = text
    }
}
