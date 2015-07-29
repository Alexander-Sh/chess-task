#ifndef PAWNPIECE_H
#define PAWNPIECE_H

#include "chesspiece.h"

class PawnPiece : public ChessPiece
{
public:
    PawnPiece(PieceColor color, int cellIndex);
    ~PawnPiece();

    QString imageName() const override;
    bool isMovePossible(int newCell) const override;
    bool isAtackPossible(int newCell) const override;
    void move(int newCellIndex) override;
    bool canJumpOver() const override;

private:
    bool m_firstStep;
};

#endif // PAWNPIECE_H
