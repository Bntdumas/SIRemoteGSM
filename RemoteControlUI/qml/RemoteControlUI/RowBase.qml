import QtQuick 2.0

Rectangle {
    id: row
    color: "transparent"

    Timer {
        interval: 1000
        repeat: true
        running: parent.visible
        triggeredOnStart: true

        onTriggered: {
            parent.highlighted = Util.timeToSeconds(arrivalTime) + parent.secondsHighlighted > Util.timeToSeconds(new Date());
        }
    }

    property bool highlighted: false
    property int secondsHighlighted: 60
}
