#include "bishop.h"

Bishop::Bishop(int row, int column, Colour colour) : Piece{row, column, colour, PieceType::Bishop, 350} {}

Bishop::Bishop(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved) : Piece{row, column, colour, type, value, passed, squaresAttacking, moved} {}

bool Bishop::canMoveTo(vector<int> from, vector<int> to) {
    // Diagonal movement
    return absVal(from[0] - to[0]) == absVal(from[1] - to[1]);
}

vector<vector<int>> Bishop::calcPath(vector<int> from, vector<int> to) const {
    vector<vector<int>> squaresPassed;
    // Checks if the current square is not equal to the destination square
    while (from != to) {
        if (from[0] > to[0]) { // if the starting row is greater than the ending row
            // if the starting column is greater than the ending column
            if (from[1] > to[1]) {
                // decrement both row and column of the current square (move from down right to up left)
                --from[0];
                --from[1];
            } else {
                // decrement the row of the current square but increment the column of the starting square (move from down left to up right)
                --from[0];
                ++from[1];
            }
        } else { // if the starting row is less than the ending row
            // if the starting colum is greater than the ending column
            if (from[1] > to[1]) {
                // increment the row of the current square but decrement the column of the starting square (move from up right to down left)
                ++from[0];
                --from[1];
            } else {
                // increment both row and column of the current square (move from up left to down right)
                ++from[0];
                ++from[1];
            }
        }
        // Checks if we've reached the destination square, if so don't add to passed squares
        if (from == to) break;
        // If we haven't reached the destination square, we will add the squares along the path travelled
        squaresPassed.emplace_back(vector<int>{from[0], from[1]});
    }

    return squaresPassed;
}

void Bishop::updatePseudoLegalMoves(int boardSize) {
    int row = getPos()[0];
    int col = getPos()[1];

    vector<vector<vector<int>>> pseudoLegal(4);

    int tmpRow = row;
    int tmpCol = col;
    // attacked squares towards top left
    while(tmpRow - 1 >= 0 && tmpCol - 1 >= 0) {
        --tmpRow;
        --tmpCol;
        pseudoLegal[0].emplace_back(vector<int>{tmpRow, tmpCol});
    }

    tmpRow = row;
    tmpCol = col;
    // attacked squares towards top right
    while (tmpRow - 1 >= 0 && tmpCol + 1 <= boardSize - 1) {
        --tmpRow;
        ++tmpCol;
        pseudoLegal[1].emplace_back(vector<int>{tmpRow, tmpCol});
    }

    tmpRow = row;
    tmpCol = col;
    // attacked squares towards bottom left
    while(tmpRow + 1 <= 7 && tmpCol - 1 >= 0) {
        ++tmpRow;
        --tmpCol;
        pseudoLegal[2].emplace_back(vector<int>{tmpRow, tmpCol});
    }

    tmpRow = row;
    tmpCol = col;
    //attacked squares towards bottom right
    while (tmpRow + 1 <= 7 && tmpCol + 1 <= boardSize - 1) {
        ++tmpRow;
        ++tmpCol;
        pseudoLegal[3].emplace_back(vector<int>{tmpRow, tmpCol});
    }   

    pseudoLegalMoves = pseudoLegal;
}

vector<vector<int>> Bishop::getPST(Colour colour) {
    return (colour == Colour::White)? WBPST : BBPST;
}
