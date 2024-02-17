#include "pawn.h"

Pawn::Pawn(int row, int column, Colour colour) : Piece{row, column, colour, PieceType::Pawn, 100}, justMoved{false}, justMovedBuffer{true} {}

Pawn::Pawn(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved, bool justMoved, bool justMovedBuffer) : Piece{row, column, colour, type, value, passed, squaresAttacking, moved}, justMoved{justMoved}, justMovedBuffer{justMovedBuffer} {}

Pawn::Pawn(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved) : Piece{row, column, colour, type, value, passed, squaresAttacking, moved} {}

bool Pawn::canMoveTo(vector<int> from, vector<int> to) {
    // White pawns can only move up the board (indexes lower in the number of vectors)
    //  Black pawns can only move down the board (indexes higher in the number of vectors)
    // First move
    if ( (absVal(from[0] - to[0]) == 2) && (to[1] == from[1]) && (!hasMoved())) {
        return (getColour() == Colour::White) ? (to[0] - from[0] == -2) : (to[0] - from[0] == 2) ;
    // Capture
    } else if ( (absVal(from[0] - to[0]) == 1) && (absVal(from[1] - to[1]) == 1) ) {
        return true;
    // Normal move
    } else { 
        return (getColour() == Colour::White) ? 
            (to[0] - from[0] == -1) && (to[1] == from[1]) :
            (to[0] - from[0] == 1) && (to[1] == from[1]);
    }

    
}

vector<vector<int>> Pawn::calcPath(vector<int> from, vector<int> to) const {
    if (absVal(from[0] - to[0]) == 2) { 
        return ((from[0] > to[0]) ? 
                    vector<vector<int>>{vector<int>{from[0] - 1, from[1]}} : 
                    vector<vector<int>>{vector<int>{to[0] - 1, to[1]}} ); 
    } else {
        return vector<vector<int>>{}; 
    }
}

void Pawn::setMoved() {
    if (!moved) {
        moved = true;
        justMoved = true;
        justMovedBuffer = false;
    }
}

void Pawn::unsetMoved() {
    moved = false;
    justMoved = false;
    justMovedBuffer = true;
}

void Pawn::updateJustMoved() {
    if (moved && justMoved && !justMovedBuffer) {
        justMovedBuffer = true;
    } else if (moved && justMoved) {
        justMoved = false;
    }
}

bool Pawn::hasJustMoved() const {
    return justMoved;
}

bool Pawn::getJustMovedBuffer() const {
    return justMovedBuffer;
}

void Pawn::updatePseudoLegalMoves(int boardSize) {
    int row = getPos()[0];
    int col = getPos()[1];

    vector<vector<vector<int>>> pseudoLegal(4);

    if (getColour() == Colour::White) {
        // attack to the top left
        if (row - 1 >= 0 && col - 1 >= 0) {
            pseudoLegal[0].emplace_back(vector<int>{row - 1, col - 1});
        }
        // attack to the top right
        if (row - 1 >= 0 && col + 1 <= 7) {
            pseudoLegal[1].emplace_back(vector<int>{row - 1, col + 1});
        }
        // Move one space up
        if (row - 1 >= 0) {
            pseudoLegal[2].emplace_back(vector<int>{row - 1, col});
        }
        // Move two spaces up
        if (row - 2 >= 0 && !hasMoved()) {
            pseudoLegal[3].emplace_back(vector<int>{row - 2, col});
        }
    } else {
        // attack to the bottom left
        if (row + 1 <= 7 && col - 1 >= 0) {
            pseudoLegal[0].emplace_back(vector<int>{row + 1, col - 1});
        }
        // attack to the bottom right
        if (row + 1 <= 7 && col + 1 <= 7) {
            pseudoLegal[1].emplace_back(vector<int>{row + 1, col + 1});
        }
        // Move one space down
        if (row + 1 >= 0) {
            pseudoLegal[2].emplace_back(vector<int>{row + 1, col});
        }
        // Move two spaces down
        if (row + 2 >= 0 && !hasMoved()) {
            pseudoLegal[3].emplace_back(vector<int>{row + 2, col});
        }
    }

    pseudoLegalMoves = pseudoLegal;
}

vector<vector<int>> Pawn::getPST(Colour colour) {
    return (colour == Colour::White)? WPPST : BPPST;
}
