#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>

#include "cellitems/chesspiece.h"

class BoardModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BoardModel(QObject *parent = nullptr);

    enum DataRoles {
        NameRole = Qt::UserRole + 1,
        ImageRole,
        BackgroundRole
    };

     QVariant data(const QModelIndex & index, int role) const override;
     QHash<int,QByteArray> roleNames() const override;
     int rowCount(const QModelIndex & parent = QModelIndex()) const override;

     Q_INVOKABLE bool isMovePossible(int oldCell, int newCell) const;
     Q_INVOKABLE bool isSelectionPosible(int cellIndex) const;
     Q_INVOKABLE bool move(int oldCell, int newCell);

     void initializeBoard();

     void startGame();
     void stopGame();

signals:
     void gameFinished(QString player);

private:
     bool isCellEmpty(int cellIndex) const;
     bool hasObstaclesOnTheWay(int firstIndex, int secondIndex) const;
     void loadBoardFromJSON(const QJsonObject& data);
     void moveChessPiece(int oldCell, int newCell);

private:
    QMap<int, QSharedPointer<ChessPiece>> m_board;
    PieceColor m_currentPlayer;
    bool m_isGameOn;
};

#endif // BOARDMODEL_H
