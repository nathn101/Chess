#ifndef __HUMAN_H__
#define __HUMAN_H__
#include "player.h"
// #include "board.h"

class human : public Player {
    public:
        // Ctor
        human(PlayerColour colour);
        // Dtor
        ~human();
};

#endif
