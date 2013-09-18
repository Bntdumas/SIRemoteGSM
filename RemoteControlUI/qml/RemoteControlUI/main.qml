import QtQuick 2.0

Rectangle {
    id: root

    // needed to display anything at all on desktop
    width: 1
    height: 1

    property int secondsHighlighted: 60

    Rectangle {
        id: dialog
        anchors.fill: parent
        anchors.margins: 10
        visible: false

        onVisibleChanged: {
            if (visible) {
                runnersDialog.setup();
            }
        }

        RunnersDialog {
            id: runnersDialog
            anchors.fill: parent
            onDone: parent.visible = false
        }
    }

    Image {
        id: background
        anchors.fill: parent
        source: "images/background.jpeg"
        visible: !dialog.visible

        Image {
            anchors.horizontalCenter: background.horizontalCenter
            source: "images/viking-logo.png"
            fillMode: Image.PreserveAspectFit
            height: background.height
            opacity: 0.05
        }

        Rectangle {
            id: container
            anchors.fill: parent
            anchors.margins: 10
            color: "transparent"

            Rectangle {
                id: header
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: 75
                color: "transparent"

                Clock {
                    id: clock
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: -9
                    timeOffset: Util.competitionTime
                }
            }

            ListView {
                id: mainView
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: header.bottom
                anchors.topMargin: 10

                ////// Workaround because ListView doesn't support add and displaced transitions in QQ1
                /*add: Transition {
                    NumberAnimation { properties: "x"; from: -root.width; duration: 1000; easing.type: Easing.OutQuart }
                }
                displaced: Transition {
                    NumberAnimation { duration: 250 }
                }*/

                model: incommingRunnersModel

                interactive: false

                delegate: KMRow {
                    width: root.width
                    secondsHighlighted: root.secondsHighlighted
                }
            }
        }
    }

    LockedButton {
        opacity: 0.001
        anchors.fill: parent
        numClicks: 5
        onClicked: dialog.visible = true
        z: dialog.visible ? -10 : 10
    }
}

