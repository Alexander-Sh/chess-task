import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    title: qsTr("Chess")
    width: 640
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("OPEN");
            }
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
                    //iconSource: "new.png"
                    text: "New game"
                }
                ToolButton {
                    //iconSource: "open.png"
                    text: "Load game"
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

}
