#include "game.h"
#include <string>

Game::Game() : whitePlayer{nullptr}, blackPlayer{nullptr} {
    whiteScore = 0;
    blackScore = 0;
    turn = PlayerColour::White;
}

Game::~Game() {
    delete whitePlayer;
    delete blackPlayer;
    whiteScore = 0;
    blackScore = 0;
}


void Game::setTurn(PlayerColour colour) {
    turn = colour;
}

PlayerColour Game::getTurn() const {
    return turn;
}

void Game::addToScore(PlayerColour colour, float score) {
    if (colour == PlayerColour::White) {
        whiteScore += score;
    } else if (colour == PlayerColour::Black) {
        blackScore += score;
    } else {
        whiteScore += score;
        blackScore += score;
    }
}
bool Game::nextTurn() {
    turn = (turn == PlayerColour::Black) ? PlayerColour::White : PlayerColour::Black;
    return true;
}

// Sets the player
void Game::setPlayer(string player, PlayerColour colour) {
    if (colour == PlayerColour::White && player == "human") whitePlayer = new human{PlayerColour::White};
    else if (colour == PlayerColour::Black && player == "human") blackPlayer = new human{PlayerColour::Black};
}
// Sets the computer
void Game::setPlayer(string player, int level, PlayerColour colour) {
    if (colour == PlayerColour::White && player == "cpu") {
        whitePlayer = new cpu{PlayerColour::White};
        cpu* compWhite = dynamic_cast<cpu *>(whitePlayer);
        if (compWhite) {
            compWhite->setLevel(level);
        }
    }

    if (colour == PlayerColour::Black && player == "cpu") {
        blackPlayer = new cpu{PlayerColour::Black};
        cpu* compBlack = dynamic_cast<cpu *>(blackPlayer);
        if (compBlack) {
            compBlack->setLevel(level);
        }
    }
}
// Gets the player
Player* Game::getPlayer(PlayerColour colour) {
    return (colour == PlayerColour::White)? whitePlayer:blackPlayer;
}
