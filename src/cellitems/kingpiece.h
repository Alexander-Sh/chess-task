#ifndef KINGPIECE_H
#define KINGPIECE_H

#include "chesspiece.h"

class KingPiece : public ChessPiece
{
public:
    KingPiece(PieceColor color, int cellIndex);

    QString imageName() const override;
    bool isMovePossible(int newCell) const override;
    bool isAtackPossible(int newCell) const override;
    bool canJumpOver() const override;
};

#endif // KINGPIECE_H
