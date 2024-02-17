#ifndef __KING_H__
#define __KING_H__
#include "piece.h"

class King : public Piece {
    // White King Piece Square Table
    vector<vector<int>> WKPST = {
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-20,-30,-30,-40,-40,-30,-30,-20},
        {-10,-20,-20,-20,-20,-20,-20,-10},
        {20, 20,  0,  0,  0,  0, 20, 20},
        {20, 30, 10,  0,  0, 10, 30, 20}
    };
    // Black King Piece Square Table
    vector<vector<int>> BKPST = {
        {20, 30, 10,  0,  0, 10, 30, 20},
        {20, 20,  0,  0,  0,  0, 20, 20},
        {-10,-20,-20,-20,-20,-20,-20,-10},
        {-20,-30,-30,-40,-40,-30,-30,-20},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30}
    };
    public:
        King(int row, int column, Colour colour);
        King(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved);
        bool canMoveTo(vector<int> from, vector<int> to) override;
        vector<vector<int>> calcPath(vector<int> from, vector<int> to) const override;
        void updatePseudoLegalMoves(int boardSize) override;
        vector<vector<int>> getPST(Colour colour) override;
        ~King() = default;
};
#endif
