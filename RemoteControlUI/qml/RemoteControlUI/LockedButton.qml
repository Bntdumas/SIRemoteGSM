import QtQuick 1.1

Item {
    signal clicked

    property int numClicks: 10
    property string text: "Button"

    height: button.height

    Button {
        z: 10
        id: button
        anchors.fill: parent
        text: parent.text + " (click " + (parent.numClicks - clicks) + " more times)"
        property int clicks: 0

        Timer {
            id: timer
            interval: 2000
            running: false
            repeat: false
            triggeredOnStart: false

            onTriggered: {
                if (parent.clicks > 0) {
                    parent.clicks--;
                }
                if (parent.clicks > 0) {
                    restart();
                }
            }
        }

        onClicked: {
            clicks++;
            timer.restart();
            if (clicks >= parent.numClicks) {
                parent.clicked();
                clicks = 0;
                timer.stop();
            }
        }
    }
}
