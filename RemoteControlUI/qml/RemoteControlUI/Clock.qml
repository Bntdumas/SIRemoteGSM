import QtQuick 1.0

Item {
    id: clock
    height: 50

    property string hours1: ""
    property string hours2: "0"
    property string minutes1: "0"
    property string minutes2: "0"
    property string seconds1: "0"
    property string seconds2: "0"
    property color textColor: "lightgray"
    property int textSize: 60

    Timer {
        interval: 1000
        repeat: true
        running: true
        triggeredOnStart: true

        onTriggered: {
            var time = new Date();
            var hours = time.getHours();
            var minutes = time.getMinutes();
            var seconds = time.getSeconds();
            parent.hours1 = Math.floor(hours / 10);
            parent.hours2 = hours - parent.hours1 * 10;
            if(parent.hours1 == "0")
                parent.hours1 = "";
            parent.minutes1 = Math.floor(minutes / 10);
            parent.minutes2 = minutes - parent.minutes1 * 10;
            parent.seconds1 = Math.floor(seconds / 10);
            parent.seconds2 = seconds - parent.seconds1 * 10;
        }
    }

    Row {
        id : rowLayout
        anchors.horizontalCenter: parent.horizontalCenter

        ClockNumber {
            id: hoursText1
            text: clock.hours1
        }
        ClockNumber {
            id: hoursText2
            text: clock.hours2
        }
        SmoothText {
            text: ":"
            color: clock.textColor
            font.pixelSize: clock.textSize
        }
        ClockNumber {
            id: minutesText1
            text: clock.minutes1
        }
        ClockNumber {
            id: minutesText2
            text: clock.minutes2
        }
        SmoothText {
            text: ":"
            color: clock.textColor
            font.pixelSize: clock.textSize
        }
        ClockNumber {
            id: secondsText1
            text: clock.seconds1
        }
        ClockNumber {
            id: secondsText2
            text: clock.seconds2
        }
    }
}
