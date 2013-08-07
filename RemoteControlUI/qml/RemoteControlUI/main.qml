import QtQuick 1.0

Rectangle {
    id: root
    width: height * ratio
    height: 480

    property double ratio: 1920 / 1080

    property int secondsHighlighted: 10

    Image {
        id: image1
        anchors.fill: parent
        source: "images/background.jpg"
        fillMode: Image.PreserveAspectFit

        Image {
            anchors.centerIn: parent
            source: "images/viking-logo.png"
            fillMode: Image.PreserveAspectFit
            width: 200
            height: width
            opacity: 0.5
        }

        Clock {
            id: clock
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.right: parent.right
        }

        ListView {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: clock.bottom
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

                property int textHeight: maxHeight * Math.pow(base, index) + buffer
                property int maxHeight: 10
                property double base: 0.6
                property int buffer: 20

                Behavior on textHeight {
                    SmoothedAnimation { }
                }

                Timer {
                    interval: 1000
                    repeat: true
                    running: parent.visible
                    triggeredOnStart: true

                    function timeToSeconds(time) {
                        return time.getHours() * 60 * 60 + time.getMinutes() * 60 + time.getSeconds();
                    }

                    onTriggered: {
                        parent.highlighted = timeToSeconds(realTime) + root.secondsHighlighted > timeToSeconds(new Date());
                    }
                }

                property bool highlighted: false

                Rectangle {
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
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 10
                    anchors.rightMargin: anchors.leftMargin
                    color: "transparent"

                    Row {
                        id: row
                        anchors.fill: parent
                        anchors.topMargin: parent.parent.buffer / 2
                        anchors.bottomMargin: anchors.topMargin

                        Rectangle {
                            width: 10
                            height: width
                            radius: height/2
                            anchors.verticalCenter: parent.verticalCenter
                            color: "green"
                            opacity: parent.parent.parent.highlighted ? 1.0 : 0.0
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

