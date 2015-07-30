#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>
#include <QJsonArray>

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

     Q_INVOKABLE void startGame();
     Q_INVOKABLE void stopGame();
     Q_INVOKABLE void loadGame();
     Q_INVOKABLE void saveGame();
     Q_INVOKABLE void nextStep();
     Q_INVOKABLE void prevStep();

signals:
     void gameFinished(QString player);
     void gameLoaded();

private:
     void initializeBoard();
     bool isCellEmpty(int cellIndex) const;
     bool hasObstaclesOnTheWay(int firstIndex, int secondIndex) const;
     void loadBoardFromJSON(const QJsonArray &data);
     QJsonArray saveBoardToJSON() const;
     QJsonObject serializeCell(int cellIndex) const;
     void deserializeCell(const QJsonObject &obj);
     void moveChessPiece(int oldCell, int newCell);
     void refreshBoard();
     void clearHistory();

private:
    QMap<int, QSharedPointer<ChessPiece>> m_board;
    PieceColor m_currentPlayer;
    bool m_isGameOn;
    QJsonArray m_moveHistory;
    int m_step;
};

#endif // BOARDMODEL_H
