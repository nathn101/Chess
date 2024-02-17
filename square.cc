#include "square.h"

// Ctor
Square::Square(int row, int column, SquareColour colour) : row{row}, column{column}, colour{colour}, currPiece{new Empty{row, column}} {
}                                                                

// Dtor
Square::~Square() {
    // delete currPiece;
}

// Sets the current square to the piece specified by newPiece
void Square::setPiece(Piece* newPiece, bool deleteCurr) {
    Piece *temp = currPiece;
    currPiece = newPiece;
    currPiece->setPos(temp->getPos()[0], temp->getPos()[1]);
    //if (deleteCurr) delete temp;
}

// Gets the current piece on the square
Piece* Square::getPiece() const {
    return currPiece;
}

// Gets the colour of the square
SquareColour Square::getColour() const {
    return colour;
}

// Gets the row of the square
int Square::getRow() const {
    return row;
}

// Gets the column of the square
int Square::getCol() const {
    return column;
}
