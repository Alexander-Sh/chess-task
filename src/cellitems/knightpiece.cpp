#include "knightpiece.h"
#include "../utils.h"

KnightPiece::KnightPiece(PieceColor color, int cellIndex)
    : ChessPiece(ChessPieceType::Knight, color, cellIndex)
{

}

QString KnightPiece::imageName() const
{
    return QStringLiteral("knight_") + ChessPiece::colorName(color());
}

bool KnightPiece::isMovePossible(int newCell) const
{
    const int newX = ChessPiece::positionX(newCell);
    const int newY = ChessPiece::positionY(newCell);
    const int deltaX = Utils::abs(newX - positionX());
    const int deltaY = Utils::abs(newY - positionY());

    return (deltaX == 2 && deltaY == 1) || (deltaX == 1 && deltaY == 2);
}

bool KnightPiece::isAtackPossible(int newCell) const
{
    return isMovePossible(newCell);
}

bool KnightPiece::canJumpOver() const
{
    return true;
}

