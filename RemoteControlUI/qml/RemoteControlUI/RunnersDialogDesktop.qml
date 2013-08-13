import QtQuick 1.1

Rectangle {
    id: root

    signal done

    function setup() {
    }

    Rectangle {
        id: selector
        visible: true
        anchors.top: parent.top
        anchors.bottom: back.top
        anchors.bottomMargin: 5
        width: parent.width

        Rectangle {
            anchors.bottomMargin: 5
            anchors.bottom: addButton.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            clip: true

            ListView {
                id: view
                anchors.fill: parent
                currentIndex: -1
                model: runnersModel
                delegate: Item {
                    Text {
                        anchors.leftMargin: 5
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: name + " (" + si + ")"
                    }
                    height: 20
                    width: view.width
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        editor.index = parent.indexAt(mouse.x, mouse.y);
                        if (editor.index !== -1 && mapper.supportsEdit) {
                            selector.visible = false;
                        }
                    }
                }
            }
        }

        Button {
            id: addButton
            text: "Add"
            anchors.bottom: invalidateCacheButton.top
            anchors.bottomMargin: 5
            anchors.left: parent.left
            anchors.right: parent.right
            onClicked: adder.visible = true
        }

        Button {
            id: invalidateCacheButton
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            text: "Invalidate Cache (click " + (10-clicks) + " times)"
            property int clicks: 0
            onClicked: {
                clicks++;
                if (clicks > 9) {
                    mapper.invalidateCache();
                    clicks = 0;
                }
            }
        }
    }

    Rectangle {
        id: editor
        anchors.top: parent.top
        anchors.bottom: back.top
        anchors.bottomMargin: 5
        width: parent.width
        visible: !selector.visible

        onVisibleChanged: removeButton.clicks = 0;

        property int index: -1

        onIndexChanged: {
            newSiTextField.placeholderText = mapper.siForIndex(index);
            newNameTextField.placeholderText = mapper.nameForIndex(index)
            newLapTextField.placeholderText = mapper.lapForIndex(index)
        }

        Column {
            anchors.fill: parent
            spacing: 2
            Text {
                text: "Change SI card:"
            }
            Row {
                spacing: 2
                TextField {
                    id: newSiTextField
                }

                Button {
                    text: "Save"
                    onClicked: {
                        if (newSiTextField.text.length > 0) {
                            mapper.changeSINumber(mapper.siForIndex(editor.index),
                                                  newSiTextField.text);
                            selector.visible = true;
                        }
                    }
                }
            }

            Rectangle {
                height: 1
                color: "black"
                width: parent.width - 10
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "Change Name:"
            }
            Row {
                spacing: 2
                TextField {
                    id: newNameTextField
                }

                Button {
                    text: "Save"
                    onClicked: {
                        if (newNameTextField.text.length > 0) {
                            mapper.changeName(mapper.siForIndex(editor.index),
                                                  newNameTextField.text);
                            selector.visible = true;
                        }
                    }
                }
            }

            Rectangle {
                height: 1
                color: "black"
                width: parent.width - 10
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "Change Lap:"
            }
            Row {
                spacing: 2
                TextField {
                    id: newLapTextField
                }

                Button {
                    text: "Save"
                    onClicked: {
                        if (newLapTextField.text.length > 0) {
                            mapper.changeLap(mapper.siForIndex(editor.index),
                                                  newLapTextField.text);
                            selector.visible = true;
                        }
                    }
                }
            }

            Rectangle {
                height: 1
                color: "black"
                width: parent.width - 10
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                id: removeButton
                text: "Remove (click " + (5-clicks) + " times)"
                property int clicks: 0
                onClicked: {
                    clicks++;
                    if (clicks > 4) {
                        mapper.removeRunner(mapper.siForIndex(editor.index));
                        clicks = 0;
                        selector.visible = true;
                    }
                }
            }
        }
    }

    Rectangle {
        id: adder
        anchors.top: parent.top
        anchors.bottom: back.top
        anchors.bottomMargin: 5
        width: parent.width
        visible: false
        z: 2

        onVisibleChanged: {
            addSiCard.text = "";
            addName.text = "";
            addTeam.text = "";
            addLap.text = "";
        }

        Flickable {
            anchors.fill: parent
            Column {
                anchors.fill: parent

                TextField {
                    id: addSiCard
                    placeholderText: "SI Card"
                }
                TextField {
                    id: addName
                    placeholderText: "Name"
                }
                TextField {
                    id: addTeam
                    placeholderText: "Team"
                }
                TextField {
                    id: addLap
                    placeholderText: "Lap"
                }
                Button {
                    text: "Save"
                    onClicked: {
                        mapper.addRunner(addSiCard.text, addName.text, addTeam.text, addLap.text);
                        adder.visible = false;
                    }
                }
            }
        }
    }

    Rectangle {
        id: errorMessage
        anchors.top: parent.top
        anchors.bottom: back.top
        anchors.bottomMargin: 5
        width: parent.width
        visible: false
        z: 2

        Rectangle {
            anchors.centerIn: parent
            width: parent.width / 2
            height: parent.height / 2
            color: "red"
            radius: 10

            Text {
                id: errorHeader
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 10
                font.pixelSize: parent.width / 12
                font.bold: true
                color: "yellow"
                text: "Error"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: errorHeader.bottom
                anchors.bottom: parent.bottom
                anchors.topMargin: 10
                wrapMode: Text.Wrap
                width: parent.width - 20
                font.pixelSize: parent.width / 16
                horizontalAlignment: Text.AlignHCenter
                color: "yellow"
                text: mapper.errorMessage
                onTextChanged: errorMessage.visible = true
            }
        }
    }

    Button {
        id: back
        text: "Back"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        onClicked: {
            if (adder.visible) {
                adder.visible = false;
            } else if (errorMessage.visible) {
                errorMessage.visible = false;
            } else if (selector.visible) {
                root.done();
            } else if (editor.visible) {
                selector.visible = true;
            }
        }
    }
}
