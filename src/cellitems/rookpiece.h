#ifndef ROOKPIECE_H
#define ROOKPIECE_H

#include "chesspiece.h"

class RookPiece : public ChessPiece
{
public:
    RookPiece(PieceColor color, int cellIndex);

    QString imageName() const override;
    bool isMovePossible(int newCell) const override;
    bool isAtackPossible(int newCell) const override;
    bool canJumpOver() const override;
};

#endif // ROOKPIECE_H
