#ifndef __CPU_H__
#define __CPU_H__
#include "player.h"
// #include "board.h"

class cpu : public Player {
    int level;
    public:
        // Ctor
        cpu(PlayerColour colour);
        // Sets the level of the cpu
        void setLevel(int level);
        // Gets the level of the cpu
        int getLevel();
        // Dtor
        ~cpu();
};


#endif
