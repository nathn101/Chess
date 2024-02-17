#ifndef __ROOK_H__
#define __ROOK_H__
#include "piece.h"

class Rook : public Piece {
    // White Rook Piece Square Table
    vector<vector<int>> WRPST = {
        {0,  0,  0,  0,  0,  0,  0,  0},
        {5, 10, 10, 10, 10, 10, 10,  5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {0,  0,  0,  5,  5,  0,  0,  0}
    };
    // Black Rook Piece Square Table
    vector<vector<int>> BRPST = {
        {0,  0,  0,  5,  5,  0,  0,  0},
        {-5,  0,  0,  0,  0,  0,  0,  -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {5, 10, 10, 10, 10, 10, 10, 5},
        {0,  0,  0,  5,  5,  0,  0,  0}
    };
    public:
        Rook(int row, int column, Colour colour);
        Rook(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved);
        bool canMoveTo(vector<int> from, vector<int> to) override;
        vector<vector<int>> calcPath(vector<int> from, vector<int> to) const override;
        void updatePseudoLegalMoves(int boardSize) override;
        vector<vector<int>> getPST(Colour colour) override;
        ~Rook() = default;
};
#endif
