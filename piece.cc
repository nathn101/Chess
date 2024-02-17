#include "piece.h"

// Ctor
Piece::Piece(int row, int column, Colour colour, PieceType type, int value) : row{row}, column{column}, 
                                                                                colour{colour}, type{type}, 
                                                                                value{value},  moved{false} {
    passed = vector<vector<int>>{};
    squaresAttacking = vector<vector<int>>{};
}

Piece::Piece(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved) : row{row}, column{column}, colour{colour}, type{type}, value{value}, moved{moved} {
    passed = vector<vector<int>>{};
    squaresAttacking = vector<vector<int>>{};
}

// Return the colour of the current piece
Colour Piece::getColour() const {
    return colour;
}

// Returns the position of the current piece
vector<int> Piece::getPos() const {
    vector<int> v{row, column};
    return v;
}

void Piece::setPos(int row, int column) {
    this->row = row;
    this->column = column;
}

// Returns the point value of the current piece
int Piece::getValue() const {
    return value;
}

// Returns the piece type of the current piece
PieceType Piece::getType() const {
    return type;
}

// Returns the vector of passed squares
vector<vector<int>> Piece::getPassed() {
    return passed;
}

// Returns the number of squares that have been passed
int Piece::getNumPassed() {
    return passed.size();
}

// Clears passed
void Piece::clearPassed() {
    passed.clear();
}

// Determines whether a piece has moved or not
bool Piece::hasMoved() const {
    return moved;
}

// Sets pieces to moved
void Piece::setMoved() {
    moved = true;
}

void Piece::unsetMoved() {
    moved = false;
}

vector<vector<vector<int>>> Piece::getPseudoLegalMoves() {
    return pseudoLegalMoves;
}

void Piece::setPseudoLegalMoves(vector<vector<vector<int>>> pseudoLegalMoves) {
    this->pseudoLegalMoves = pseudoLegalMoves;
}

vector<vector<int>> Piece::getSquaresAttacking() {
    return squaresAttacking;
}

void Piece::updateAttacking(vector<vector<int>> attacking) {
    squaresAttacking = attacking;
}

void Piece::updatePassed(vector<vector<int>> passed) {
    this->passed = passed;
}

// Dtor
Piece::~Piece() {}
