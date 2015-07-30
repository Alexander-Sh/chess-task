#include "pawnpiece.h"
#include "../utils.h"

PawnPiece::PawnPiece(PieceColor color, int cellIndex)
    : ChessPiece(ChessPieceType::Pawn, color, cellIndex)
    , m_firstStep(true)
{

}

PawnPiece::~PawnPiece()
{
}

QString PawnPiece::imageName() const
{
    return QStringLiteral("pawn_") + ChessPiece::colorName(color());
}

bool PawnPiece::isMovePossible(int newCell) const
{
    const int newX = ChessPiece::positionX(newCell);
    const int newY = ChessPiece::positionY(newCell);
    const int allowStep = m_firstStep ? 2 : 1;
    const int delta = positionY()- newY;

    if(newX != positionX())
        return false;

    if(Utils::abs(delta) > allowStep)
        return false;

    if(color() == PieceColor::Black && delta > 0)
        return false;

    if(color() == PieceColor::White && delta < 0)
        return false;

    return true;
}

bool PawnPiece::isAtackPossible(int newCell) const
{
    const int newX = ChessPiece::positionX(newCell);
    const int newY = ChessPiece::positionY(newCell);
    const int delta = positionY()- newY;

    if(newX == positionX())
        return false;

    if(Utils::abs(delta) > 1 || Utils::abs(positionX()- newX) > 1)
        return false;

    if(color() == PieceColor::Black && delta > 0)
        return false;

    if(color() == PieceColor::White && delta < 0)
        return false;

    return true;
}

void PawnPiece::move(int newCellIndex)
{
    ChessPiece::move(newCellIndex);
    m_firstStep = false;
}

bool PawnPiece::canJumpOver() const
{
    return false;
}

