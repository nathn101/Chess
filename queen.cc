#include "queen.h"
#include "rook.h"
#include "bishop.h"

Queen::Queen(int row, int column, Colour colour) : Piece{row, column, colour, PieceType::Queen, 900} {}

Queen::Queen(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved) : Piece{row, column, colour, type, value, passed, squaresAttacking, moved} {}

bool Queen::canMoveTo(vector<int> from, vector<int> to) {
    Rook r{from[0], from[1], getColour()};
    Bishop b{from[0], from[1], getColour()};

    // A queen can move like a bishop or a rook
    return r.canMoveTo(from, to) || b.canMoveTo(from, to);
}

vector<vector<int>> Queen::calcPath(vector<int> from, vector<int> to) const {
    vector<vector<int>> squaresPassed;

    Rook r{from[0], from[1], getColour()};
    Bishop b{from[0], from[1], getColour()};

    vector<vector<int>> bishopPassed;
    vector<vector<int>> rookPassed;

    if (absVal(from[0] - to[0]) == absVal(from[1] - to[1])) {
        bishopPassed = b.calcPath(from, to);
    } else {
        rookPassed = r.calcPath(from, to);
    }

    // Add the two vectors together
    squaresPassed.reserve(rookPassed.size() + bishopPassed.size());
    squaresPassed.insert(squaresPassed.end(), rookPassed.begin(), rookPassed.end());
    squaresPassed.insert(squaresPassed.end(), bishopPassed.begin(), bishopPassed.end());

    return squaresPassed;
}

void Queen::updatePseudoLegalMoves(int boardSize) {
    Rook r{getPos()[0], getPos()[1], getColour()};
    Bishop b{getPos()[0], getPos()[1], getColour()};
    
    vector<vector<vector<int>>> pseudoLegal(8);

    // Get the Pseudo legal moves of the rook and bishop
    r.updatePseudoLegalMoves(boardSize);
    b.updatePseudoLegalMoves(boardSize);

    auto rookPseudo = r.getPseudoLegalMoves();
    auto bishopPseudo = b.getPseudoLegalMoves();

    // Add the two vectors together
    pseudoLegal.reserve(rookPseudo.size() + bishopPseudo.size());
    pseudoLegal.insert(pseudoLegal.end(), rookPseudo.begin(), rookPseudo.end());
    pseudoLegal.insert(pseudoLegal.end(), bishopPseudo.begin(), bishopPseudo.end());
    
    pseudoLegalMoves = pseudoLegal;
}

vector<vector<int>> Queen::getPST(Colour colour) {
    return (colour == Colour::White)? WQPST : BQPST;
}
