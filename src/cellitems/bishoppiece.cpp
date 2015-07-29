#include "bishoppiece.h"
#include "../utils.h"

BishopPiece::BishopPiece(PieceColor color, int cellIndex)
    : ChessPiece(ChessPieceType::Bishop, color, cellIndex)
{

}

QString BishopPiece::imageName() const
{
    return QStringLiteral("bishop_") + ChessPiece::colorName(color());
}

bool BishopPiece::isMovePossible(int newCell) const
{
    const int newX = ChessPiece::positionX(newCell);
    const int newY = ChessPiece::positionY(newCell);
    const int deltaX = Utils::abs(newX - positionX());
    const int deltaY = Utils::abs(newY - positionY());
    return deltaX == deltaY;
}

bool BishopPiece::isAtackPossible(int newCell) const
{
    return isMovePossible(newCell);
}

bool BishopPiece::canJumpOver() const
{
    return false;
}

