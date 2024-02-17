#ifndef __GAME_H__
#define __GAME_H__
#include "human.h"
#include "cpu.h"

using namespace std;

class Game {
    Player *whitePlayer = nullptr;
    Player *blackPlayer = nullptr;
    // Whose turn is it
    PlayerColour turn;

    protected:
        float whiteScore;
        float blackScore;
        // Add score to the player with colour colour, or to both players if colour is none
        void addToScore(PlayerColour colour, float score);

    public:
        // Ctor
        Game();
        // Dtor
        virtual ~Game();
        // 
        void startGame();
        // End the game with winner as the winner
        virtual void endGame(PlayerColour winner) = 0;
        // Set turn to the player who has colour colour
        void setTurn(PlayerColour colour);
        // Get the colour of the person whose turn it is
        PlayerColour getTurn() const;
        // Sets the player
        void setPlayer(string player, PlayerColour colour);
        // Sets the computer
        void setPlayer(string player, int level, PlayerColour colour);
        // Gets the player
        Player* getPlayer(PlayerColour colour);
        // Move to the next turn 
        virtual bool nextTurn();
};

#endif
