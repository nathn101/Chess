#ifndef __PIECE_H__
#define __PIECE_H__
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "util.h"

using namespace std;

enum class PieceType { Pawn, Rook, Knight, Bishop, King, Queen, Empty };
enum class Colour { Black, White, NoColour };

class Piece {
    int row;
    int column;
    Colour colour;
    PieceType type;
    int value;
    vector<vector<int>> squaresAttacking = vector<vector<int>>{};

    protected:
        vector<vector<int>> passed = vector<vector<int>>{};
        vector<vector<vector<int>>> pseudoLegalMoves;
        bool moved;

    public:
        // Constructor
        Piece(int row, int column, Colour colour, PieceType type, int value);
        Piece(int row, int column, Colour colour, PieceType type, int value,
            vector<vector<int>> passed, vector<vector<int>> squaresAttacking, bool moved);
        // Copy constructor
        Piece(const Piece& other);
        //Dtor
        virtual ~Piece() = 0;
        // Copy assignment operator
        Piece& operator=(Piece& other) = default;
        // Returns the colour of the piece
        Colour getColour() const;
        // Returns the position of the piece as a vector
        vector<int> getPos() const;
        // Sets the position of the piece
        void setPos(int row, int column);
        // Returns the value of the piece
        int getValue() const;
        // Returns the type of the piece
        PieceType getType() const;
        // Returns true if the piece can move from from to to
        virtual bool canMoveTo(vector<int> from, vector<int> to) = 0;
        // Updates the vector of passed squares
        void updatePassed(vector<vector<int>> passed);
        // Gets all the coordinates of the squares that the piece has passed along its path to its destination
        vector<vector<int>> getPassed();
        // Calculates the squares the piece has passed along its path to its destination
        //   Assumes it is called with valid parameters
        virtual vector<vector<int>> calcPath(vector<int> from, vector<int> to) const = 0;
        // Gets the length of the Passed vector
        int getNumPassed();
        // Clears previously passed pieces
        void clearPassed();
        // Returns true if the piece has moved
        bool hasMoved() const;
        // Set moved true
        virtual void setMoved();
        // Set moved false
        virtual void unsetMoved();
        // Set the piece's attacking vector
        void updateAttacking(vector<vector<int>> attacking);
        // Return all pseudo-legal squares the piece can move to
        virtual void updatePseudoLegalMoves(int boardSize) = 0;
        // Set the piece's pseudo-legal moves
        void setPseudoLegalMoves(vector<vector<vector<int>>> pseudoLegalMoves);
        // Get the piece's pseudo-legal moves
        vector<vector<vector<int>>> getPseudoLegalMoves();
        // Get the squares that the piece is attacking
        vector<vector<int>> getSquaresAttacking();
        virtual vector<vector<int>> getPST(Colour colour) = 0;
};
#endif
