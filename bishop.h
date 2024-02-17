#ifndef __BISHOP_H__
#define __BISHOP_H__
#include "piece.h"

class Bishop : public Piece {
    // White Bishop Piece Square Table
    vector<vector<int>> WBPST = {
        {-20,-10,-10,-10,-10,-10,-10,-20},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-10,  0,  5, 10, 10,  5,  0,-10},
        {-10,  5,  5, 10, 10,  5,  5,-10},
        {-10,  0, 10, 10, 10, 10,  0,-10},
        {-10, 10, 10, 10, 10, 10, 10,-10},
        {-10,  5,  0,  0,  0,  0,  5,-10},
        {-20,-10,-10,-10,-10,-10,-10,-20}
    };

    // Black Bishop Piece Square Table NOT REVERSED YET
    vector<vector<int>> BBPST = {
        {-20,-10,-10,-10,-10,-10,-10,-20},
        {-10,  5,  0,  0,  0,  0,  5,-10},
        {-10, 10, 10, 10, 10, 10, 10,-10},
        {-10,  0, 10, 10, 10, 10,  0,-10},
        {-10,  5,  5, 10, 10,  5,  5,-10},
        {-10,  0,  5, 10, 10,  5,  0,-10},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-20,-10,-10,-10,-10,-10,-10,-20}
    };

    public:
        Bishop(int row, int column, Colour colour);
        Bishop(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved);
        bool canMoveTo(vector<int> from, vector<int> to) override;
        vector<vector<int>> calcPath(vector<int> from, vector<int> to) const override;
        void updatePseudoLegalMoves(int boardSize) override;
        vector<vector<int>> getPST(Colour colour) override;
        ~Bishop() = default;
};
#endif
