import QtQuick 1.0

SmoothText {
    id : clockNumber
    color: "lightgray"
    font.pixelSize: 60
    Behavior on text {
        SequentialAnimation {
            ParallelAnimation {
                NumberAnimation { target: clockNumber; property: "opacity"; to: 0.1; duration: 200 }
                NumberAnimation { target: clockNumber; property: "y"; to: clock.height/4; duration: 300 }
            }
            PropertyAction {  }
            NumberAnimation { target: clockNumber; property: "y"; to: -clock.height/4; duration: 0 }
            ParallelAnimation {
                NumberAnimation { target: clockNumber; property: "opacity"; to: 1; duration: 200 }
                NumberAnimation { target: clockNumber; property: "y"; to: 0; duration: 400 }
            }
        }
    }
}
