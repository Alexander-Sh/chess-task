#include "boardmodel.h"
#include "cellitems/chesspiece.h"
#include "utils.h"
#include "constants.h"

#include <stdexcept>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>
#include <QVariant>
#include <QDebug>

BoardModel::BoardModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_currentPlayer(PieceColor::White)
    , m_isGameOn(false)
    , m_step(-1)
{

}

QVariant BoardModel::data(const QModelIndex &index, int role) const
{
    if( !m_board.contains(index.row()) )
        return QVariant();

    const QSharedPointer<ChessPiece> item = m_board.value(index.row());

    switch(role) {
        case NameRole:
            return QString("Cell %1").arg(index.row());
        case ImageRole:
            return QStringLiteral("qrc:/img/") + item->imageName() + QStringLiteral(".png");
    }

    return QVariant();
}

QHash<int, QByteArray> BoardModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ImageRole] = "image";
    return roles;
}

int BoardModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return Constants::boardTotalSize;
}

bool BoardModel::isMovePossible(int oldCell, int newCell) const
{
    if(!m_isGameOn)
        return false;

    if(isCellEmpty(oldCell))
        return false;

    QSharedPointer<ChessPiece> chessPiece = m_board.value(oldCell);
    if(!isCellEmpty(newCell)) {
        QSharedPointer<ChessPiece> chessPiece2 = m_board.value(newCell);
        if(chessPiece->color() == chessPiece2->color())
            return false;
        else {
            if(chessPiece->isAtackPossible(newCell)) {
                return chessPiece->canJumpOver() || !hasObstaclesOnTheWay(oldCell, newCell);
            } else {
                return false;
            }
        }

    } else if(chessPiece->isMovePossible(newCell))
        return chessPiece->canJumpOver() || !hasObstaclesOnTheWay(oldCell, newCell);
    else
        return false;
}

bool BoardModel::isSelectionPosible(int cellIndex) const
{
    if(!m_isGameOn)
        return false;

    if(isCellEmpty(cellIndex))
        return false;

    QSharedPointer<ChessPiece> chessPiece = m_board.value(cellIndex);
    if(chessPiece->color() != m_currentPlayer)
        return false;

    return true;
}

bool BoardModel::move(int oldCell, int newCell)
{
    if(isCellEmpty(oldCell))
        return false;

    QSharedPointer<ChessPiece> chessPiece = m_board.value(oldCell);
    if(chessPiece->color() != m_currentPlayer)
        return false;

    try {
        QSharedPointer<ChessPiece> chessPiece2 = m_board.value(newCell);
        QJsonObject history;
        history["prev"] = QJsonArray({serializeCell(oldCell),serializeCell(newCell)});
        moveChessPiece(oldCell, newCell);
        history["next"] = QJsonArray({serializeCell(oldCell),serializeCell(newCell)});
        m_moveHistory.append(history);

        if(!chessPiece2.isNull() && chessPiece2->type() == ChessPieceType::King) {
            emit gameFinished(ChessPiece::colorName(m_currentPlayer));
            m_isGameOn = false;
        }
        m_currentPlayer = m_currentPlayer == PieceColor::White ? PieceColor::Black : PieceColor::White;
    } catch(std::exception& e) {
        qDebug() << e.what();
        Q_ASSERT_X(false, "BoardModel", e.what());
        return false;
    }
    return true;
}

void BoardModel::initializeBoard()
{
    QFile loadFile(":/assets/initial_board.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return ;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    loadBoardFromJSON(loadDoc.array());
}

void BoardModel::startGame()
{
    qDebug() << "startgame";
    initializeBoard();
    m_isGameOn = true;
    m_currentPlayer = PieceColor::White;
    refreshBoard();
    clearHistory();
}

void BoardModel::stopGame()
{
    m_board.clear();
    refreshBoard();
    m_isGameOn = false;
}

void BoardModel::loadGame()
{
    QString filePath = QFileDialog::getOpenFileName(0, "ChessGame", QDir::homePath());
    QFile loadFile(filePath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return ;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    m_moveHistory = loadDoc.array();
    if(m_moveHistory.count() > 0) {
        m_step = 0;
        initializeBoard();
        emit gameLoaded();
        refreshBoard();
    }
}

void BoardModel::saveGame()
{
    QString filePath = QFileDialog::getSaveFileName(0, "ChessGame", QDir::homePath(), "json");
    if(!filePath.isEmpty()) {
        filePath += ".json";

        QFile saveFile(filePath);

        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
            return ;
        }

        QJsonDocument saveDoc(m_moveHistory);
        saveFile.write(saveDoc.toJson());
    }
}

void BoardModel::nextStep()
{
    try {
        if(m_step < m_moveHistory.count()) {
            QJsonArray cells = m_moveHistory.at(m_step).toObject()["next"].toArray();
            for(QJsonValue val : cells) {
                deserializeCell(val.toObject());
            }
            ++m_step;
        }
    } catch(std::exception &e) {
        Q_ASSERT_X(false, "nextSetp", e.what());
        qDebug() << e.what();
    }
}

void BoardModel::prevStep()
{
    try {
        if(m_step > 0) {
            --m_step;
            QJsonArray cells = m_moveHistory.at(m_step).toObject()["prev"].toArray();
            for(QJsonValue val : cells) {
                deserializeCell(val.toObject());
            }
        }
    } catch(std::exception &e) {
        Q_ASSERT_X(false, "prevSetp", e.what());
        qDebug() << e.what();
    }
}

bool BoardModel::isCellEmpty(int cellIndex) const
{
    return !m_board.contains(cellIndex);
}

bool BoardModel::hasObstaclesOnTheWay(int firstIndex, int secondIndex) const
{
    const int firstX  = ChessPiece::positionX(firstIndex);
    const int firstY  = ChessPiece::positionY(firstIndex);
    const int secondX = ChessPiece::positionX(secondIndex);
    const int secondY = ChessPiece::positionY(secondIndex);

    int deltaX = secondX - firstX;
    int deltaY = secondY - firstY;

    auto indexByXY = [] (int x, int y) -> int { return x + y * Constants::boardYSize;};

    int absDeltaX = Utils::abs(deltaX);
    int absDeltaY = Utils::abs(deltaY);
    const int signX = absDeltaX > 0 ? deltaX / absDeltaX : 1;
    const int signY = absDeltaY > 0 ? deltaY / absDeltaY : 1;

    while(absDeltaX > 1 || absDeltaY > 1) {
        if(absDeltaX > 1)
            absDeltaX--;
        if(absDeltaY > 1)
            absDeltaY--;

        const int cellIndex = indexByXY(firstX + absDeltaX * signX, firstY + absDeltaY * signY);

        if(m_board.contains(cellIndex))
            return true;
    }

    return false;
}

void BoardModel::loadBoardFromJSON(const QJsonArray &data)
{
    m_board.clear();
    try {
        for(QJsonValue val : data)  {
            deserializeCell(val.toObject());
        }
    } catch(std::exception &e) {
        m_board.clear();
        qDebug() << e.what();
    }

    refreshBoard();
}

QJsonArray BoardModel::saveBoardToJSON() const
{
    QJsonArray result;
    for(auto it = m_board.constBegin(); it != m_board.constEnd(); ++it) {
        result.append(serializeCell(it.key()));
    }
    return result;
}

QJsonObject BoardModel::serializeCell(int cellIndex) const
{
    QJsonObject res;
    if(!isCellEmpty(cellIndex)) {
        QSharedPointer<ChessPiece> piece = m_board.value(cellIndex);
        res["type"] = static_cast<int>(piece->type());
        res["color"] = static_cast<int>(piece->color());
    }
    res["cell"] = cellIndex;

    return res;
}

void BoardModel::deserializeCell(const QJsonObject &obj)
{
    const QStringList keys = obj.keys();
    if(!obj.isEmpty() && keys.contains("cell")) {
        const int cellIndex = obj["cell"].toInt();
        if(keys.contains("type") && keys.contains("color")) {
            ChessPieceType type = static_cast<ChessPieceType>(obj["type"].toInt());
            PieceColor color = static_cast<PieceColor>(obj["color"].toInt());

            m_board.insert(cellIndex, QSharedPointer<ChessPiece>(ChessPiece::createChessPiece(type, color, cellIndex)));
        } else {
            m_board.remove(cellIndex);
        }
        emit dataChanged(index(cellIndex), index(cellIndex));
    } else {
        throw std::logic_error("cannot deserialize cell from json object");
    }
}

void BoardModel::moveChessPiece(int oldCell, int newCell)
{
    QSharedPointer<ChessPiece> chessPiece = m_board.value(oldCell);
    chessPiece->move(newCell);
    m_board.remove(newCell);
    m_board.insert(newCell, chessPiece);
    m_board.remove(oldCell);
    emit dataChanged(index(oldCell), index(oldCell));
    emit dataChanged(index(newCell), index(newCell));
}

void BoardModel::refreshBoard()
{
    emit dataChanged(index(0), index(Constants::boardTotalSize-1));
}

void BoardModel::clearHistory()
{
    m_moveHistory = QJsonArray();
}

