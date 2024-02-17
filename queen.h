#ifndef __QUEEN_H__
#define __QUEEN_H__
#include "piece.h"

class Queen : public Piece {
    // White Queen Piece Square Table
    vector<vector<int>> WQPST = {
        {-20,-10,-10, -5, -5,-10,-10,-20},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-10,  0,  5,  5,  5,  5,  0,-10},
        {-5,  0,  5,  5,  5,  5,  0, -5},
        {0,  0,  5,  5,  5,  5,  0, -5},
        {-10,  5,  5,  5,  5,  5,  0,-10},
        {-10,  0,  5,  0,  0,  0,  0,-10},
        {-20,-10,-10, -5, -5,-10,-10,-20}
    };
    // Black Queen Piece Square Table
    vector<vector<int>> BQPST = {
        {-20,-10,-10, -5, -5,-10,-10,-20},
        {-10,  0,  0,  0,  0,  5,  0,-10},
        {-10,  0,  5,  5,  5,  5,  5,-10},
        {-5,  0,  5,  5,  5,  5,  0, -5},
        {0,  0,  5,  5,  5,  5,  0, -5},
        {-10,  0,  5,  5,  5,  5,  0,-10},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-20,-10,-10, -5, -5,-10,-10,-20}
    };
    public:
        Queen(int row, int column, Colour colour);
        Queen(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved);
        bool canMoveTo(vector<int> from, vector<int> to) override;
        vector<vector<int>> calcPath(vector<int> from, vector<int> to) const override;
        void updatePseudoLegalMoves(int boardSize) override;
        vector<vector<int>> getPST(Colour colour) override;
        ~Queen() = default;
};
#endif
