#include "kingpiece.h"
#include "../utils.h"

KingPiece::KingPiece(PieceColor color, int cellIndex)
    : ChessPiece(ChessPieceType::King, color, cellIndex)
{

}

QString KingPiece::imageName() const
{
    return QStringLiteral("king_") + ChessPiece::colorName(color());
}

bool KingPiece::isMovePossible(int newCell) const
{
    const int newX = ChessPiece::positionX(newCell);
    const int newY = ChessPiece::positionY(newCell);
    const int deltaX = Utils::abs(newX - positionX());
    const int deltaY = Utils::abs(newY - positionY());
    return deltaY <= 1 || deltaX <= 1;
}

bool KingPiece::isAtackPossible(int newCell) const
{
    return isMovePossible(newCell);
}

bool KingPiece::canJumpOver() const
{
    return false;
}

