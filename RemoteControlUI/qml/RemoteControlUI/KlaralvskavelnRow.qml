import QtQuick 2.0

RowBase {
    id: row
    height: textHeight + 10

    property int textHeight: offset * Math.pow(base, index) + minHeight
    property int offset: 10
    property double base: 0.6
    property int minHeight: 30

    Behavior on textHeight {
        SmoothedAnimation { }
    }

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
    Rectangle {
        height: parent.height
        anchors.top: line.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: anchors.leftMargin
        color: "transparent"

        Row {
            anchors.fill: parent
            anchors.topMargin: 5
            anchors.bottomMargin: anchors.topMargin

            Rectangle {
                width: 5
                height: 5
                color: "transparent"
            }

            property color textColor: "lightgray"
            property int effectiveWidth: width - 15
            SmoothText {
                height: parent.height
                width: parent.effectiveWidth * 0.4
                font.pixelSize: height
                color: parent.textColor
                text: name
                font.bold: row.highlighted
                font.italic: row.highlighted
            }
            SmoothText {
                height: parent.height
                width: parent.effectiveWidth * 0.4
                font.pixelSize: height
                color: parent.textColor
                text: team
                font.bold: row.highlighted
                font.italic: row.highlighted
            }
            SmoothText {
                height: parent.height
                width: parent.effectiveWidth * 0.1
                font.pixelSize: height
                color: parent.textColor
                text: time
                horizontalAlignment: Text.AlignLeft
                font.bold: row.highlighted
                font.italic: row.highlighted
            }
            SmoothText {
                height: parent.height
                width: parent.effectiveWidth * 0.1
                font.pixelSize: height
                color: parent.textColor
                text: lap
                horizontalAlignment: Text.AlignHCenter
                font.bold: row.highlighted
                font.italic: row.highlighted
            }
        }
    }
}
