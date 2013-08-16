import QtQuick 1.1

Rectangle {
    id: root

    // needed to display anything at all on desktop
    width: 1
    height: 1

    property int secondsHighlighted: 10

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



//                Image {
//                    anchors.right: parent.right
//                    anchors.rightMargin: 10
//                    anchors.verticalCenter: parent.verticalCenter
//                    source: "images/viking-logo.png"
//                    fillMode: Image.PreserveAspectFit
//                    width: height
//                    height: parent.height
//                    opacity: 0.5
//                }

//                Image {
//                    anchors.left: parent.left
//                    anchors.leftMargin: 10
//                    anchors.verticalCenter: parent.verticalCenter
//                    source: "images/viking-logo.png"
//                    fillMode: Image.PreserveAspectFit
//                    width: height
//                    height: parent.height
//                    opacity: 0.5
//                }

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

                delegate: Rectangle {
                    width: root.width
                    height: textHeight + 10
                    color: "transparent"

                    property int textHeight: offset * Math.pow(base, index) + minHeight
                    property int offset: 10
                    property double base: 0.6
                    property int minHeight: 30

                    Behavior on textHeight {
                        SmoothedAnimation { }
                    }

                    Timer {
                        interval: 1000
                        repeat: true
                        running: parent.visible
                        triggeredOnStart: true

                        function timeToSeconds(t) {
                            return t.getHours() * 60 * 60 + t.getMinutes() * 60 + t.getSeconds();
                        }

                        onTriggered: {
                            parent.highlighted = Util.timeToString(arrivalTime) + root.secondsHighlighted > Util.timeToString(new Date());
                        }
                    }

                    property bool highlighted: false

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
                            id: row
                            anchors.fill: parent
                            anchors.topMargin: 5
                            anchors.bottomMargin: anchors.topMargin

                            Rectangle {
                                width: 10
                                height: width
                                radius: height/2
                                anchors.verticalCenter: parent.verticalCenter
                                color: "green"
                                opacity: parent.parent.parent.highlighted ? 1.0 : 0.001
                            }
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
                            }
                            SmoothText {
                                height: parent.height
                                width: parent.effectiveWidth * 0.4
                                font.pixelSize: height
                                color: parent.textColor
                                text: team
                            }
                            SmoothText {
                                height: parent.height
                                width: parent.effectiveWidth * 0.1
                                font.pixelSize: height
                                color: parent.textColor
                                text: time
                                horizontalAlignment: Text.AlignLeft
                            }
                            SmoothText {
                                height: parent.height
                                width: parent.effectiveWidth * 0.1
                                font.pixelSize: height
                                color: parent.textColor
                                text: lap
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }
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

