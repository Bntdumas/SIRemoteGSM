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
                delegate:
                    Text {
                        text: name + " (" + si + ")"
                    }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        editor.index = parent.indexAt(mouse.x + parent.contentX, mouse.y + parent.contentY);
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

        LockedButton {
            id: invalidateCacheButton
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            text: "Invalidate Cache"
            onClicked: mapper.invalidateCache();
        }
    }

    Rectangle {
        id: editor
        anchors.top: parent.top
        anchors.bottom: back.top
        anchors.bottomMargin: 5
        width: parent.width
        visible: !selector.visible

        property int index: -1

        function clear()
        {
            newSiTextField.placeholderText = mapper.siForIndex(index);
            newNameTextField.placeholderText = mapper.nameForIndex(index);
            newLapTextField.placeholderText = mapper.lapForIndex(index);
            newSiTextField.setText("");
            newNameTextField.setText("");
            newLapTextField.setText("");
        }

        onIndexChanged: clear();
        onVisibleChanged: clear();

        Flickable {
            anchors.fill: parent
            flickableDirection: Flickable.VerticalFlick
            contentWidth: width
            contentHeight: editorColumn.height
            clip: true

            MouseArea {
                anchors.fill: parent
                z: -1
                onClicked: parent.focus = true
            }

            Column {
                id: editorColumn
                spacing: 2
                width: parent.width

                Text {
                    text: "Change SI card:"
                }
                Row {
                    spacing: 10
                    anchors.left: parent.left
                    anchors.right: parent.right
                    TextField {
                        id: newSiTextField
                        width: parent.width - 80 - 10
                    }

                    Button {
                        text: "Save"
                        width: 80
                        onClicked: {
                            if (newSiTextField.text.length > 0) {
                                mapper.changeSINumber(mapper.siForIndex(editor.index),
                                                      newSiTextField.text);
                                selector.visible = true;
                            }
                        }
                    }
                }

                Item {
                    width: parent.width
                    height: 6
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
                    spacing: 10
                    anchors.left: parent.left
                    anchors.right: parent.right
                    TextField {
                        id: newNameTextField
                        width: parent.width - 80 - 10
                    }

                    Button {
                        width: 80
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

                Item {
                    width: parent.width
                    height: 6
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
                    spacing: 10
                    anchors.left: parent.left
                    anchors.right: parent.right
                    TextField {
                        id: newLapTextField
                        width: parent.width - 80 - 10
                    }

                    Button {
                        width: 80
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

                Item {
                    width: parent.width
                    height: 6
                }

                Rectangle {
                    height: 1
                    color: "black"
                    width: parent.width - 10
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Item {
                    width: parent.width
                    height: 6
                }

                LockedButton {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    id: removeButton
                    text: "Remove"
                    onClicked: {
                        mapper.removeRunner(mapper.siForIndex(editor.index));
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
            flickableDirection: Flickable.VerticalFlick
            contentWidth: width
            contentHeight: adderColumn.height
            clip: true

            MouseArea {
                anchors.fill: parent
                z: -1
                onClicked: parent.focus = true
            }

            Column {
                id: adderColumn
                spacing: 2
                width: parent.width

                TextField {
                    id: addSiCard
                    placeholderText: "SI Card"
                    width: parent.width
                }
                TextField {
                    id: addName
                    placeholderText: "Name"
                    width: parent.width
                }
                TextField {
                    id: addTeam
                    placeholderText: "Team"
                    width: parent.width
                }
                TextField {
                    id: addLap
                    placeholderText: "Lap"
                    width: parent.width
                }
                Button {
                    width: parent.width
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
