#ifndef __EMPTY_H__
#define __EMPTY_H__
#include "piece.h"

class Empty : public Piece {
    public:
        // Ctor
        Empty(int row, int column);
        Empty(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved);
        // Determines whether a piece can move to a square pseudo-legally
        bool canMoveTo(vector<int> from, vector<int> to) override;
        // Calculates the squares passed by a piece along its path to its destination
        vector<vector<int>> calcPath(vector<int> from, vector<int> to) const override;
        // Calculates all the attacked squares
        void updatePseudoLegalMoves(int boardSize) override;
        vector<vector<int>> getPST(Colour colour) override;
        ~Empty() = default;
};
#endif
