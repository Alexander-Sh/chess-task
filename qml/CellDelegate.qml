import QtQuick 2.4

Item {
    id: delegate
    width: chessGrid.cellWidth; height: chessGrid.cellHeight;

    Rectangle {
        anchors.fill: parent
        antialiasing : true
        color: (Math.floor(model.index / 8) + model.index)%2 == 0 ? "#eee" : "#555"
        anchors.horizontalCenter: parent.horizontalCenter
//        rotation: chessGrid.currentIndex === index ? 25 : 0

//        Behavior on rotation {
//            NumberAnimation {
//                duration: 600
//                easing.type: Easing.OutBounce
//            }
//        }
    }
    Image {
        anchors.fill: parent
        source: image ? image : ""
        rotation: chessGrid.currentIndex === index ? 27 : 0
        Behavior on rotation {
            NumberAnimation {
                duration: 600
                easing.type: Easing.OutBounce
            }
        }
    }

    MouseArea {
        id: frameMouseArea
        anchors {
            fill: parent
        }
        onClicked: {

            if(chessGrid.currentIndex === index) {
                chessGrid.currentIndex = -1;
            } else if(chessGrid.currentIndex === -1 && boardModel.isSelectionPosible(index)) {
                chessGrid.currentIndex = index;
            } else if(chessGrid.currentIndex !== -1 && boardModel.isMovePossible(chessGrid.currentIndex, index)) {
                boardModel.move(chessGrid.currentIndex, index);
                chessGrid.currentIndex = -1;
            }
        }
    }
}
