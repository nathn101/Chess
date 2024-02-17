#ifndef __KNIGHT_H__
#define __KNIGHT_H__
#include "piece.h"

class Knight : public Piece {
    // White Knight Piece Square Table
    vector<vector<int>> WNPST = {
        {-50,-40,-30,-30,-30,-30,-40,-50},
        {-40,-20,  0,  0,  0,  0,-20,-40},
        {-30,  0, 10, 15, 15, 10,  0,-30},
        {-30,  5, 15, 20, 20, 15,  5,-30},
        {-30,  0, 15, 20, 20, 15,  0,-30},
        {-30,  5, 10, 15, 15, 10,  5,-30},
        {-40,-20,  0,  5,  5,  0,-20,-40},
        {-50,-40,-30,-30,-30,-30,-40,-50}
    };
    // Black Knight Piece Square Table
    vector<vector<int>> BNPST = {
        {-50,-40,-30,-30,-30,-30,-40,-50},
        {-40,-20,  0,  0,  0,  0,-20,-40},
        {-30,  0, 10, 15, 15, 10,  0,-30},
        {-30,  5, 15, 20, 20, 15,  5,-30},
        {-30,  0, 15, 20, 20, 15,  0,-30},
        {-30,  5, 10, 15, 15, 10,  5,-30},
        {-40,-20,  0,  5,  5,  0,-20,-40},
        {-50,-40,-30,-30,-30,-30,-40,-50}
    };
    public:
        Knight(int row, int column, Colour colour);
        Knight(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved);
        bool canMoveTo(vector<int> from, vector<int> to) override;
        vector<vector<int>> calcPath(vector<int> from, vector<int> to) const override;
        void updatePseudoLegalMoves(int boardSize) override;
        vector<vector<int>> getPST(Colour colour) override;
        ~Knight() = default;
};
#endif
