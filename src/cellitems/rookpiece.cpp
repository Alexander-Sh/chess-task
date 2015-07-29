#include "rookpiece.h"

RookPiece::RookPiece(PieceColor color, int cellIndex)
    : ChessPiece(ChessPieceType::Rook, color, cellIndex)
{

}

QString RookPiece::imageName() const
{
    return QStringLiteral("rook_") + ChessPiece::colorName(color());
}

bool RookPiece::isMovePossible(int newCell) const
{
    const int newX = ChessPiece::positionX(newCell);
    const int newY = ChessPiece::positionY(newCell);

    return (newX == positionX() || newY == positionY());
}

bool RookPiece::isAtackPossible(int newCell) const
{
    return isMovePossible(newCell);
}

bool RookPiece::canJumpOver() const
{
    return false;
}

