#ifndef __SQUARE_H__
#define __SQUARE_H__
#include "empty.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

using namespace std;

enum class SquareColour { Burgundy, White };

class Square {
    int row;
    int column;
    SquareColour colour;
    Piece *currPiece;

    public:
        // Ctor
        Square(int row, int column, SquareColour colour);
        // Dtor
        ~Square();
        // Set currPiece to the given piece. Delete the current piece if desired
        void setPiece(Piece* newPiece, bool deleteCurr /*I think we can delete this as well. Refer to square.cc*/);
        // Returns currPiece
        Piece *getPiece() const;
        // Returns the colour of the square
        SquareColour getColour() const;
        // Returns the row of the square
        int getRow() const;
        // Returns the column of the square
        int getCol() const;
};
#endif
