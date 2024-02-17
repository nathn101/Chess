#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <iostream>
#include <string>
#include <vector>
// #include "board.h"

using namespace std;

enum class PlayerColour { Black, White, NoColour };
enum class PlayerType { Human, CPU };

class Player {
    // Board *board;
    PlayerColour colour;
    PlayerType type;
    public:
        // Ctor
        Player(PlayerColour colour, PlayerType type);
        // Gets Player Type
        PlayerType getType();
        // Dtor
        virtual ~Player();
    
};
#endif
