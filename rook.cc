#include "rook.h"

Rook::Rook(int row, int column, Colour colour) : Piece{row, column, colour, PieceType::Rook, 500} {}

Rook::Rook(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved) : Piece{row, column, colour, type, value, passed, squaresAttacking, moved} {}

bool Rook::canMoveTo(vector<int> from, vector<int> to) {
    // Horizontal or vertical movement
    // Row or column must be the same, but not both
    return !( (from[0] == to[0]) && (from[1] == to[1]) ) &&
            ( (from[0] == to[0]) || (from[1] == to[1]) );
}

vector<vector<int>> Rook::calcPath(vector<int> from, vector<int> to) const {
    vector<vector<int>> squaresPassed;

    // Checks if the current square is not equal to the destination square
    while (from != to) {
        if (from[0] > to[0]) { // if the current row is greater than the destination row then we decrement the current row
            --from[0]; // move from bottom to top
        }
        if (from[0] < to[0]) { // if the current row is less than the destination row then we increment the current row
            ++from[0]; // move from top to bottom
        }
        if (from[1] > to[1]) { // if the current column is greater than the destination column then we decrement the current column
            --from[1]; // move from right to left
        }
        if (from[1] < to[1]) { // if the current column is less than the destination column then we increment the current column
            ++from[1]; // move from left to right
        }
        // Checks if we've reached the destination square, if so don't add to passed squares
        if (from == to) break;
        // If we haven't reached the destination square, we will add the squares along the path travelled
        squaresPassed.emplace_back(vector<int>{from[0], from[1]});
    }
    return squaresPassed;
}

void Rook::updatePseudoLegalMoves(int boardSize) {
    int row = getPos()[0];
    int col = getPos()[1];

    int tmpRow = row;
    int tmpCol = col;

    vector<vector<vector<int>>> pseudoLegal(4);

    // attacked squares towards the bottom
    while (tmpRow + 1 <= boardSize - 1) {
        ++tmpRow;
        pseudoLegal[0].emplace_back(vector<int>{tmpRow, tmpCol});
    }
    tmpRow = row;
    // attacked squares towards the top
    while (tmpRow - 1 >= 0) {
        --tmpRow;
        pseudoLegal[1].emplace_back(vector<int>{tmpRow, tmpCol});
    }
    tmpRow = row;
    // attacked squares towards the left
    while(tmpCol - 1 >= 0) {
        --tmpCol;
        pseudoLegal[2].emplace_back(vector<int>{tmpRow, tmpCol});
    }
    tmpCol = col;
    // attacked squares towards the right
    while(tmpCol + 1 <= boardSize - 1) {
        ++tmpCol;
        pseudoLegal[3].emplace_back(vector<int>{tmpRow, tmpCol});
    }

    pseudoLegalMoves = pseudoLegal;
}


vector<vector<int>> Rook::getPST(Colour colour) {
    return (colour == Colour::White)? WRPST : BRPST;
}
