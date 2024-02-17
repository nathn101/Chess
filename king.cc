#include "king.h"

King::King(int row, int column, Colour colour) : Piece{row, column, colour, PieceType::King, 1000} {}

King::King(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved) : Piece{row, column, colour, type, value, passed, squaresAttacking, moved} {}

// NOTE: Castling returns false
bool King::canMoveTo(vector<int> from, vector<int> to) {
    // One move in any direction
    if (absVal(from[0] - to[0]) == 1) {
        // Moving down or up the board
        return absVal(from[1] - to[1]) <= 1;
    } else if (from[0] - to[0] == 0) {
        // Moving horizontally
        return absVal(from[1] - to[1]) == 1;
    }

    return false;
}

// Don't need to calculate path because next square is destination
vector<vector<int>> King::calcPath(vector<int> from, vector<int> to) const {
    return vector<vector<int>>{};
}

// Calculates squares being attacked
void King::updatePseudoLegalMoves(int boardSize) {
    int row = getPos()[0];
    int col = getPos()[1];

    vector<vector<vector<int>>> pseudoLegal(10);

    // bottom square
    if (row + 1 <= boardSize - 1) pseudoLegal[0].emplace_back(vector<int>{row + 1, col});
    // top square
    if (row - 1 >= 0) pseudoLegal[1].emplace_back(vector<int>{row - 1, col});
    // right square
    if (col + 1 <= boardSize - 1) pseudoLegal[2].emplace_back(vector<int>{row, col + 1});
    // left square
    if (col - 1 >= 0) pseudoLegal[3].emplace_back(vector<int>{row, col - 1});
    // top left
    if (row - 1 >= 0 && col - 1 >= 0) pseudoLegal[4].emplace_back(vector<int>{row - 1, col - 1});
    // top right
    if (row - 1 >= 0 && col + 1 <= boardSize - 1) pseudoLegal[5].emplace_back(vector<int>{row - 1, col + 1});
    // bottom left
    if (row + 1 <= boardSize - 1 && col - 1 >= 0) pseudoLegal[6].emplace_back(vector<int>{row + 1, col - 1});
    // bottom right
    if (row + 1 <= boardSize - 1 && col + 1 <= boardSize - 1) pseudoLegal[7].emplace_back(vector<int>{row + 1, col + 1});

    // Castling left and right
    if ( (row == 0 || row == boardSize - 1) && (col == 4) ) {
        pseudoLegal[8].emplace_back(vector<int>{row, col + 2});
        pseudoLegal[9].emplace_back(vector<int>{row, col - 2});
    }

    pseudoLegalMoves = pseudoLegal;
}

vector<vector<int>> King::getPST(Colour colour) {
    return (colour == Colour::White)? WKPST : BKPST;
}
