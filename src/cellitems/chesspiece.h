#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QString>


enum class ChessPieceType {
    King = 0,
    Queen = 1,
    Knight = 2,
    Bishop = 3,
    Rook = 4,
    Pawn = 5
};

enum class PieceColor {
    White = 0,
    Black
};


class ChessPiece
{
public:
    ChessPiece(ChessPieceType type, PieceColor color, int cellIndex);
    virtual ~ChessPiece();

    virtual QString imageName() const = 0;
    virtual bool isMovePossible(int newCell) const = 0;
    virtual bool isAtackPossible(int newCell) const = 0;
    virtual bool canJumpOver() const = 0;
    virtual void move(int newCellIndex);
    PieceColor color() const {return m_color;}
    ChessPieceType type() const {return m_type;}
    int currentCellIndex() const {return m_cellIndex;}
    int positionX() const {return positionX(m_cellIndex);}
    int positionY() const {return positionY(m_cellIndex);}

    static int positionX(int cellIndex);
    static int positionY(int cellIndex);

    static QString colorName(PieceColor);
    static ChessPiece* createChessPiece(ChessPieceType type, PieceColor color, int cellIndex);

private:
    const PieceColor m_color;
    const ChessPieceType m_type;
    int m_cellIndex;
};

#endif // CHESSPIECE_H
