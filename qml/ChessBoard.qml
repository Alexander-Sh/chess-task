import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    GridView {
        id: chessGrid
        anchors.fill: parent

        boundsBehavior: Flickable.StopAtBounds
        model: boardModel
        delegate: CellDelegate {}
        currentIndex: -1
        cellWidth: chessBoard.width / 8
        cellHeight: cellWidth
    }

}
