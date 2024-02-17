#ifndef __PAWN_H__
#define __PAWN_H__
#include "piece.h"

class Pawn : public Piece {
    bool justMoved;
    bool justMovedBuffer;
    // White Pawn Piece Square Table
    vector<vector<int>> WPPST = {
        {0,  0,  0,  0,  0,  0,  0,  0},
        {50, 50, 50, 50, 50, 50, 50, 50},
        {10, 10, 20, 30, 30, 20, 10, 10},
        {5,  5, 10, 25, 25, 10,  5,  5},
        {0,  0,  0, 20, 20,  0,  0,  0},
        {5, -5,-10,  0,  0,-10, -5,  5},
        {5, 10, 10,-20,-20, 10, 10,  5},
        {0,  0,  0,  0,  0,  0,  0,  0}
    };
    // Black Pawn Piece Square Table
    vector<vector<int>> BPPST = {
        {0,  0,  0,  0,  0,  0,  0,  0},
        {5, 10, 10,-20,-20, 10, 10,  5},
        {5, -5,-10,  0,  0,-10, -5,  5},
        {0,  0,  0, 20, 20,  0,  0,  0},
        {5,  5, 10, 25, 25, 10,  5,  5},
        {10, 10, 20, 30, 30, 20, 10, 10},
        {50, 50, 50, 50, 50, 50, 50, 50},
        {0,  0,  0,  0,  0,  0,  0,  0}
    };
    public:
        Pawn(int row, int column, Colour colour);
        Pawn(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved, bool justMoved, bool justMovedBuffer);
        Pawn(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved);
        bool canMoveTo(vector<int> from, vector<int> to) override;
        vector<vector<int>> calcPath(vector<int> from, vector<int> to) const override;
        void updatePseudoLegalMoves(int boardSize) override;
        void setMoved() override;
        void unsetMoved() override;
        void updateJustMoved();
        bool hasJustMoved() const;
        bool getJustMovedBuffer() const;
        vector<vector<int>> getPST(Colour colour) override;
        ~Pawn() = default;
};

#endif
