import QtQuick 1.1

Rectangle {
    id: button

    radius: 5
    height: 80
    z: 5
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
    property string text: "Button"
    property bool pressed: mouseArea.pressed

    Text {
        id: label
        anchors.centerIn: parent
        text: parent.text
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            parent.clicked();
        }
    }
}
