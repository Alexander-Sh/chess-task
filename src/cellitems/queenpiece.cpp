#include "queenpiece.h"
#include "../utils.h"

QueenPiece::QueenPiece(PieceColor color, int cellIndex)
    : ChessPiece(ChessPieceType::Queen, color, cellIndex)
{

}

QString QueenPiece::imageName() const
{
    return QStringLiteral("queen_") + ChessPiece::colorName(color());
}

bool QueenPiece::isMovePossible(int newCell) const
{
    const int newX = ChessPiece::positionX(newCell);
    const int newY = ChessPiece::positionY(newCell);
    const int deltaX = Utils::abs(newX - positionX());
    const int deltaY = Utils::abs(newY - positionY());

    return (deltaX == deltaY) || deltaX == 0 || deltaY == 0;
}

bool QueenPiece::isAtackPossible(int newCell) const
{
    return isMovePossible(newCell);
}

bool QueenPiece::canJumpOver() const
{
    return false;
}

