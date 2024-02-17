#ifndef __BOARD_H__
#define __BOARD_H__
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include "observer.h"
#include "square.h"
#include "game.h"
// #include "player.h"

using namespace std;

enum class SpecialMoveType{ShortCastleW, LongCastleW, ShortCastleB, LongCastleB, EnPassant};

class Board : public Game {
    int boardSize;
    vector<vector<Square>> theBoard;
    vector<Observer*> observers;
    vector<vector<int>> blackAttacking;
    vector<vector<int>> whiteAttacking;
    vector<vector<int>> updated;

    // Returns true if pos is a valid location on the chess board
    bool validLocation(string pos);
    bool validLocation(int row, int col);

    // Copy constructor
    Board(Board& b);
    // Returns true if c is in turn
    bool isinTurn(PlayerColour c) const;
    // Convert a piece string and a location to a Piece object
    Piece* convertPiece(char piece, string location) const;
    // Convert a string position to a vector<int>{row, col}
    vector<int> convertPos(string pos) const;
    // Converts a vector position to a string position
    string convertPos(vector<int> pos) const;
    // Returns true if c is checkmated
    bool isCheckmated(PlayerColour c);
    bool isStalemated(PlayerColour c);
    // Returns true if t is legal
    bool canCastle(SpecialMoveType t);
    // Returs true if en passant is legal from from to to
    bool canEnPassant(string from, string to);
    bool canEnPassant(vector<int> from, vector<int> to);
    // Detach o from the board's observers
    void detach(Observer *o);
    // Updates the board's attacking vectors, along with the pieces'
    void updateAttacking(vector<vector<int>> recentlyMoved, bool simulatedMove);
    void updateAttackingAt(int row, int col, bool updatePseudoLegal);

    public:
        // Constructor for Board
        Board(int size);
        // Dtor for Board
        ~Board() override;
        // Clear all pieces on the board
        void clearBoard();
        // Attach an observer to the board's observers
        void attach(Observer *o);
        // Returns a reference to the square signified by the given location
        Square& getSquare(string location);
        // Returns a reference to the square signified by the given row and column
        Square& getSquare(int row, int col);
        // Adds the provided piece to theBoard at the given location
        void addPiece(char piece, string location);
        // Adds the provided piece to theBoard at the given row and column
        void addPiece(Piece *p, int row, int col);
        // Removes a piece from theBoard at the given location
        void removePiece(string location);
        // Removes a piece from theBoard at the given row and column
        void removePiece(int row, int col);
        // Notify the observers of the board
        void notifyObservers();
        // End the game. Assign the winner 1 point, or 0.5 points for a draw. Reset the board
        void endGame(PlayerColour winner) override;
        // Move the piece from square from to square to
        bool move(string from, string to);
        // CPU move
        bool move();
        // Move from from to to and promote to promotion
        bool move(string from, string to, char promotion);
        // Returns true if moving from from to to is a legal move
        bool isLegalMove(vector<int> from, vector<int> to);
        // Returns true if c is in check
        bool isInCheck(PlayerColour c);
        // Returns true if the square at row, col is attacked by by
        bool isAttacked(int row, int col, PlayerColour by);
        // Get updated squares
        vector<vector<int>>& getUpdated();
        // Clear updated squares
        void clearUpdated();
        // Move to the next turn. If checkmate, stalemate, end the game and print text
        bool nextTurn() override;
        // Get the score of c
        float getScore(PlayerColour c);
        // Get the legal moves for the player whose turn it is
        vector<vector<vector<int>>> getLegalMoves();
        // Get the legal moves for c
        vector<vector<vector<int>>> getLegalMoves(PlayerColour c);
        // Returns true if theBoard has a valid setup
        bool setUpValid();
        // Print the evaluation of the current position
        void printEvaluation();
};
#endif
