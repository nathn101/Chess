#include "player.h"

Player::Player(PlayerColour colour, PlayerType type): colour{colour}, type{type} {}
PlayerType Player::getType() { return type; }
Player::~Player() {}
