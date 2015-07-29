#include "boardmodel.h"
#include "cellitems/chesspiece.h"
#include "constants.h"

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
        return false ; // TODO if jump

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
        moveChessPiece(oldCell, newCell);

        if(!chessPiece2.isNull() && chessPiece2->type() == ChessPieceType::King) {
            emit gameFinished(ChessPiece::colorName(m_currentPlayer));
            m_isGameOn = false;
        }
        m_currentPlayer = m_currentPlayer == PieceColor::White ? PieceColor::Black : PieceColor::White;
        rememberBoard();
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
    loadBoardFromJSON(loadDoc.object());
}

void BoardModel::startGame()
{
    qDebug() << "startgame";
    initializeBoard();
    m_isGameOn = true;
    refreshBoard();
    clearHistory();
    rememberBoard();
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
    m_gameHistory = loadDoc.array();
    if(m_gameHistory.count() > 0) {
        m_step = 0;
        loadBoardFromJSON(m_gameHistory.at(m_step).toObject());
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

        QJsonDocument saveDoc(m_gameHistory);
        saveFile.write(saveDoc.toJson());
    }
    qDebug() << filePath;
}

void BoardModel::nextStep()
{
    if(m_step < m_gameHistory.count() - 1) {
        ++m_step;
        loadBoardFromJSON(m_gameHistory.at(m_step).toObject());
    }
}

void BoardModel::prevStep()
{
    if(m_step > 0) {
        --m_step;
        loadBoardFromJSON(m_gameHistory.at(m_step).toObject());
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

    auto abs = [] (int val) -> int { return val > 0 ? val : val * -1; };
    auto indexByXY = [] (int x, int y) -> int { return x + y * Constants::boardYSize;};

    int absDeltaX = abs(deltaX);
    int absDeltaY = abs(deltaY);
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

void BoardModel::loadBoardFromJSON(const QJsonObject &data)
{
    m_board.clear();
    for(auto it = data.constBegin(); it != data.constEnd(); ++it) {
        Q_ASSERT_X(!m_board.contains(it.key().toInt()), "loadingJSON", "Duplicated data");
        ChessPieceType type = static_cast<ChessPieceType>(it.value().toObject()["piece"].toInt());
        PieceColor color = static_cast<PieceColor>(it.value().toObject()["color"].toInt());
        it.value().toObject()["color"].toInt();

        const int cellIndex = it.key().toInt();
        m_board.insert(cellIndex, QSharedPointer<ChessPiece>(ChessPiece::createChessPiece(type, color, cellIndex)));
    }
    refreshBoard();
}

QJsonObject BoardModel::saveBoardToJSON() const
{
    QJsonObject result;
    for(auto it = m_board.constBegin(); it != m_board.constEnd(); ++it) {
        QJsonObject piece;
        piece["piece"] = static_cast<int>(it.value()->type());
        piece["color"] = static_cast<int>(it.value()->color());
        result[QString::number(it.key())] = piece;
    }
    return result;
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

void BoardModel::rememberBoard()
{
    m_gameHistory.append(saveBoardToJSON());
}

void BoardModel::refreshBoard()
{
    emit dataChanged(index(0), index(Constants::boardTotalSize-1));
}

void BoardModel::clearHistory()
{
    m_gameHistory = QJsonArray();
}

