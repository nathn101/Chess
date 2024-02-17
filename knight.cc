#include "knight.h"

Knight::Knight(int row, int column, Colour colour) : Piece{row, column, colour, PieceType::Knight, 300} {}

Knight::Knight(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved) : Piece{row, column, colour, type, value, passed, squaresAttacking, moved} {}

bool Knight::canMoveTo(vector<int> from, vector<int> to) {
    // Vertical L (think L)
    if (absVal(from[0] - to[0]) == 2) {
        return absVal(from[1] - to[1]) == 1;
    // Horizontal L (think negation symbol)
    } else if (absVal(from[0] - to[0]) == 1) {
        return absVal(from[1] - to[1]) == 2;
    } else {
        return false;
    }
}

// Don't need to calculate path because it jumps over any piece
vector<vector<int>> Knight::calcPath(vector<int> from, vector<int> to) const {
    return vector<vector<int>>{};
}

void Knight::updatePseudoLegalMoves(int boardSize) {
    int row = getPos()[0];
    int col = getPos()[1];

    vector<vector<vector<int>>> pseudoLegal(8);

    // down right
    if (row + 2 <= boardSize - 1 && col + 1 <= boardSize - 1) pseudoLegal[0].emplace_back(vector<int>{row + 2, col + 1});
    // down left
    if (row + 2 <= boardSize - 1 && col - 1 >= 0) pseudoLegal[1].emplace_back(vector<int>{row + 2, col - 1});
    // up right
    if (row - 2 >= 0 && col + 1 <= boardSize - 1) pseudoLegal[2].emplace_back(vector<int>{row - 2, col + 1});
    // up left
    if (row - 2 >= 0 && col - 1 >= 0) pseudoLegal[3].emplace_back(vector<int>{row - 2, col - 1});
    // left up
    if (row - 1 >= 0 && col - 2 >= 0) pseudoLegal[4].emplace_back(vector<int>{row - 1, col - 2});
    // left down
    if (row + 1 <= boardSize - 1 && col - 2 >= 0) pseudoLegal[5].emplace_back(vector<int>{row + 1, col - 2});
    // right up
    if (row - 1 >= 0 && col + 2 <= boardSize - 1) pseudoLegal[6].emplace_back(vector<int>{row - 1, col + 2});
    // right down
    if (row + 1 <= boardSize - 1 && col + 2 <= boardSize - 1) pseudoLegal[7].emplace_back(vector<int>{row + 1, col + 2});

    pseudoLegalMoves = pseudoLegal;
}

vector<vector<int>> Knight::getPST(Colour colour) {
    return (colour == Colour::White)? WNPST : BNPST;
}
