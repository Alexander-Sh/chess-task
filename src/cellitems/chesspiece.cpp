#include "chesspiece.h"
#include "kingpiece.h"
#include "knightpiece.h"
#include "bishoppiece.h"
#include "queenpiece.h"
#include "rookpiece.h"
#include "pawnpiece.h"

#include "../constants.h"

ChessPiece::ChessPiece(ChessPieceType type, PieceColor color, int cellIndex)
    : m_color(color)
    , m_type(type)
    , m_cellIndex(cellIndex)
{

}

ChessPiece::~ChessPiece()
{

}

void ChessPiece::move(int newCellIndex)
{
    if(newCellIndex < 0 || newCellIndex > 63)
        throw std::logic_error("Out of range movement");

    if(isMovePossible(newCellIndex) || isAtackPossible(newCellIndex)) {
        m_cellIndex = newCellIndex;
    } else {
        throw std::logic_error("Wrong move pattern");
    }
}

int ChessPiece::positionX(int cellIndex)
{
    return cellIndex % Constants::boardXSize;
}

int ChessPiece::positionY(int cellIndex)
{
    return cellIndex / Constants::boardYSize;
}

QString ChessPiece::colorName(PieceColor color)
{
    if(color == PieceColor::White)
        return QStringLiteral("white");
    else
        return QStringLiteral("black");
}

ChessPiece *ChessPiece::createChessPiece(ChessPieceType type, PieceColor color, int cellIndex)
{
    switch(type) {
    case ChessPieceType::King:
        return new KingPiece(color, cellIndex);
    case ChessPieceType::Knight:
        return new KnightPiece(color, cellIndex);
    case ChessPieceType::Bishop:
        return new BishopPiece(color, cellIndex);
    case ChessPieceType::Queen:
        return new QueenPiece(color, cellIndex);
    case ChessPieceType::Rook:
        return new RookPiece(color, cellIndex);
    case ChessPieceType::Pawn:
        return new PawnPiece(color, cellIndex);
    }
    Q_ASSERT_X(false, Q_FUNC_INFO, "Missing chesspiece");
    return nullptr;
}
