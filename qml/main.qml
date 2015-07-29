import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQml.StateMachine 1.0

ApplicationWindow {
    title: qsTr("Chess")
    width: 640
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }
    toolBar:ToolBar {
            RowLayout {
                anchors.fill: parent
                ToolButton {
                    id: newGameToolButton
                    text: "New game"
                    onClicked: boardModel.startGame()
                }
                ToolButton {
                    id: stopGameToolButton
                    text: "Stop game"
                    onClicked: boardModel.stopGame()
                    visible: false
                }
                ToolButton {
                    id: loadGameToolButton
                    text: "Load game"
                    onClicked: boardModel.loadGame()
                }
                ToolButton {
                    id: saveGameToolButton
                    text: "Save game"
                    visible: false
                    onClicked: boardModel.saveGame()
                }
                ToolButton {
                    id: prevGameToolButton
                    text: "Prev"
                    visible: false
                    onClicked: boardModel.prevStep()
                }
                ToolButton {
                    id: nextGameToolButton
                    text: "Next"
                    visible: false
                    onClicked: boardModel.nextStep()
                }

                Item { Layout.fillWidth: true }
            }
        }
    Rectangle {
        anchors.fill: parent
        color: "#559"
    }

    ChessBoard {
        id: chessBoard
        anchors {
            centerIn: parent
        }
        height: parent.height > parent.width ? parent.width : parent.height
        width: chessBoard.height
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("Chess game")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

    Connections {
        target: boardModel

        onGameFinished: {
            messageDialog.show("Player " + player + " won!");
        }
    }

    StateMachine {
        id: stateMachine
        initialState: s1
        running: true
        State {
            id: s1

            SignalTransition {
                targetState: s2
                signal: newGameToolButton.clicked
            }
            SignalTransition {
                targetState: s3
                signal: boardModel.gameLoaded
            }

            onEntered: {
                newGameToolButton.visible = true;
                loadGameToolButton.visible = true;
            }
            onExited: {
                newGameToolButton.visible = false;
                loadGameToolButton.visible = false;
            }
        }

        State {
            id: s2

            SignalTransition {
                targetState: s1
                signal: stopGameToolButton.clicked
            }
            onEntered: {
                stopGameToolButton.visible = true;
                saveGameToolButton.visible = true;
            }

            onExited: {
                stopGameToolButton.visible = false;
                saveGameToolButton.visible = false;
            }
        }
        State {
            id: s3
            SignalTransition {
                targetState: s2
                signal: newGameToolButton.clicked
            }
            onEntered: {
                newGameToolButton.visible = true;
                loadGameToolButton.visible = true;
                prevGameToolButton.visible = true;
                nextGameToolButton.visible = true;
            }

            onExited: {
                newGameToolButton.visible = false;
                loadGameToolButton.visible = false;
                prevGameToolButton.visible = false;
                nextGameToolButton.visible = false;
            }
        }
    }
}
