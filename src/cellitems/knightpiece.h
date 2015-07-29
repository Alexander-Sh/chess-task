#ifndef KNIGHTPIECE_H
#define KNIGHTPIECE_H

#include "chesspiece.h"

class KnightPiece : public ChessPiece
{
public:
    KnightPiece(PieceColor color, int cellIndex);

    QString imageName() const override;
    bool isMovePossible(int newCell) const override;
    bool isAtackPossible(int newCell) const override;
    bool canJumpOver() const override;
};

#endif // KNIGHTPIECE_H
