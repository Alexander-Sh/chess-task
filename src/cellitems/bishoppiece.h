#ifndef BISHOPPIECE_H
#define BISHOPPIECE_H

#include "chesspiece.h"

class BishopPiece : public ChessPiece
{
public:
    BishopPiece(PieceColor color, int cellIndex);
    QString imageName() const override;
    bool isMovePossible(int newCell) const override;
    bool isAtackPossible(int newCell) const override;
    bool canJumpOver() const override;
};

#endif // BISHOPPIECE_H
