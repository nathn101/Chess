#include "empty.h"

Empty::Empty(int row, int column) : Piece{row, column, Colour::NoColour, PieceType::Empty, 0} {}

Empty::Empty(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved) : Piece{row, column, colour, type, value, passed, squaresAttacking, moved} {}

// Empty piece cannot move
bool Empty::canMoveTo(vector<int> from, vector<int> to) {
    return false;
}

// Empty piece cannot move
vector<vector<int>> Empty::calcPath(vector<int> from, vector<int> to) const {
    return vector<vector<int>>{};
}

// Empty piece cannot move
void Empty::updatePseudoLegalMoves(int boardSize) {
    
}

 vector<vector<int>> Empty::getPST(Colour colour) { return  vector<vector<int>>{}; }
