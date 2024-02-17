#include "cpu.h"

cpu::cpu(PlayerColour colour): Player{colour, PlayerType::CPU} {
    
}

void cpu::setLevel(int level) {
    this->level = level;
}

int cpu::getLevel() {
    return level;
}

cpu::~cpu() {}
