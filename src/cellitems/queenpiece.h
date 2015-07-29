#ifndef QUEENPIECE_H
#define QUEENPIECE_H

#include "chesspiece.h"
#include "bishoppiece.h"
#include "rookpiece.h"

class QueenPiece : public ChessPiece
{
public:
    QueenPiece(PieceColor color, int cellIndex);

    QString imageName() const override;
    bool isMovePossible(int newCell) const override;
    bool isAtackPossible(int newCell) const override;
    bool canJumpOver() const override;
};

#endif // QUEENPIECE_H
