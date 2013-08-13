import QtQuick 1.1

Rectangle {
    id: button

    radius: 5
    height: Math.max(label.contentHeight + 4, 15)
    width: Math.max(label.contentWidth + 10, 100)
    gradient: Gradient {
        GradientStop {
            color: "gray"
            position: button.pressed ? 0.0 : 1.0
        }
        GradientStop {
            color: "lightgray"
            position: button.pressed ? 1.0 : 0.0
        }
    }

    signal clicked
    property string text: ""
    property bool pressed: mouseArea.pressed

    Text {
        id: label
        anchors.centerIn: parent
        text: parent.text
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: parent.clicked()
    }
}
