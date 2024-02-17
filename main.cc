#include <iostream>
#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <cstring>
#include "textdisplay.h"
#include "graphicsdisplay.h"
#include "board.h"

using namespace std;

int main(int argc, char* argv[]) {
    bool eval = false;

    if ( (argc > 1) && (strcmp(argv[1], "-eval") == 0) ) eval = true;
    string command, aux, players;
    int boardSize = 8;

    float white = 0;
    float black = 0;

    // Initialize TextDisplay and GraphicsDisplay
    TextDisplay *td = new TextDisplay(boardSize);
    Xwindow xw;
    GraphicsDisplay *gd = new GraphicsDisplay(xw, boardSize);

    // Initialize the Board with td and gd as observers
    Board *b = new Board(boardSize);
    b->attach(td);
    b->attach(gd);
    bool gameStarted = false;

    // Command loop
    while (true) {
        cout << "Please enter \'setup\', \'game <human / computer#> <human / computer#>\', or CTRL+D" << endl;
        cin >> command;
        // Exit program
        if (cin.fail() || std::cin.eof()) {
            cout << "Final Score:" << endl;
            cout << "White: " << white << endl;
            cout << "Black: " << black << endl;
            delete b;
            delete td;
            return 0;
        }

        // Start a new game
        if (command == "game") {
            while (true) {
                try {
                    // Retrieve players
                    getline(cin, players);
                    // Exit program
                    if (cin.fail() || cin.eof()) {
                        cout << "Final Score:" << endl;
                        cout << "White: " << white << endl;
                        cout << "Black: " << black << endl;
                        delete b;
                        delete td;
                        return 0;
                    }
                    istringstream plyrs{players};
                    string whiteP, blackP;
                    // Try to read into players
                    if (plyrs >> whiteP && plyrs >> blackP) {
                        // Checks if any of the player inputs were wrong
                        if (whiteP != "human" && whiteP != "computer1" 
                        && whiteP != "computer2" && whiteP != "computer3" && whiteP != "computer4") throw (invalid_argument("Error: Invalid Player"));
                        
                        if (blackP != "human" && blackP != "computer1" 
                        && blackP != "computer2" && blackP != "computer3" && blackP != "computer4") throw (invalid_argument("Error: Invalid Player"));

                        if (whiteP == "human") b->setPlayer("human", PlayerColour::White);
                        else {
                            int level = whiteP[whiteP.length() - 1] - '0';
                            if (level < 1 || level > 4) throw (invalid_argument("Error: Invalid Level"));
                            b->setPlayer("cpu", level, PlayerColour::White);
                        }
                        if (blackP == "human") b->setPlayer("human", PlayerColour::Black);
                        else {
                            int level = blackP[blackP.length() - 1] - '0';
                            if (level < 1 || level > 4) throw (invalid_argument("Error: Invalid Level"));
                            b->setPlayer("cpu", level, PlayerColour::Black);
                        }
                        break;
                    } else {
                        cout << "Please select player types (on one line)" << endl;
                    }
                } catch (const invalid_argument &e) {
                    cerr << e.what() << endl;
                }
            }
            
            
            gameStarted = true;
            b->notifyObservers();
            cout << *td;

            // Game loop
            while (true) {
                cout << "Please enter \'move ?location location? ?promotion piece?\', \'resign\'" << endl;
                try {
                    getline(cin, aux);
                    // Edit program
                    if (cin.fail() || cin.eof()) {
                        cout << "Final Score:" << endl;
                        cout << "White: " << white << endl;
                        cout << "Black: " << black << endl;
                        delete b;
                        delete td;
                        return 0;
                    }
                    istringstream iss{aux};
                    iss >> command;
                    // Current player resigns. Give win to the other player
                    if (command == "resign") {
                        b->nextTurn();
                        b->endGame(b->getTurn());
                        if (b->getTurn() == PlayerColour::White) cout << "White wins!" << endl;
                        else cout << "Black wins!" << endl;
                        white += b->getScore(PlayerColour::White);
                        black += b->getScore(PlayerColour::Black);
                        delete b;
                        b = new Board(boardSize);
                        b->attach(td);
                        b->attach(gd);
                        gameStarted = false;
                        break;

                    // Move from from to to, promote to promotion if requested
                    } else if (command == "move") {
                        string from;
                        string to;
                        char promotion;
                        bool moveMade;

                        // Read in squares
                        iss >> from;
                        iss >> to;

                        // Cannot read into from meaning just "move" was called
                        if (from == "") {
                            // Try to cast the player to a cpu
                            cpu* computer = dynamic_cast<cpu *>(b->getPlayer(b->getTurn()));
                            // If the player is a cpu then it will cast and perform a move
                            if (computer) {
                                if (b->move()) {
                                    cout << *td;
                                    if (eval) b->printEvaluation();
                                    if (!b->nextTurn())  {
                                        white += b->getScore(PlayerColour::White);
                                        black += b->getScore(PlayerColour::Black);
                                        delete b;
                                        b = new Board(boardSize);
                                        b->attach(td);
                                        b->attach(gd);
                                        gameStarted = false;
                                        break;
                                    }
                                } else {
                                    throw (std::invalid_argument("Error: CPU made an invalid move"));
                                }
                            } else { // If the player is not a cpu then wrong turn
                                throw (std::invalid_argument("Error: Cannot make CPU move as a human"));
                            } 
                        } else {
                            // Try to cast the player to a human
                            human* person = dynamic_cast<human *>(b->getPlayer(b->getTurn()));
                            if (person) {
                                // Validate square input
                                if (from.length() != 2 || to.length() != 2) {
                                    throw (std::invalid_argument("Error: Square to or from not found"));
                                } else if (iss >> promotion) { 
                                    // Make the move // ADD THE CHECK
                                    moveMade = b->move(from, to, promotion);
                                } else {
                                    moveMade = b->move(from, to);
                                }
                            }
                            // If the move is valid, print the board and go to the next turn
                            if (moveMade) {
                                cout << *td;
                                if (eval) b->printEvaluation();
                                // If next turn cannot be reached, reset the board
                                if (!b->nextTurn())  {
                                    white += b->getScore(PlayerColour::White);
                                    black += b->getScore(PlayerColour::Black);
                                    delete b;
                                    b = new Board(boardSize);
                                    b->attach(td);
                                    b->attach(gd);
                                    gameStarted = false;
                                    break;
                                }
                            } else {
                                throw (std::invalid_argument("Error: Invalid human move"));
                            }
                        }
                    } else {
                        cout << "Invalid command" << endl;
                    }
                } catch (const std::invalid_argument &e) {
                    cerr << e.what() << endl;
                } 
            }

        // Setup a new game 
        } else if (command == "setup") {
            if (gameStarted) {
                cerr << "Game in progress, cannot enter setup mode" << endl;
                continue;
            }
            cout << "Welcome to setup mode!" << endl;
            cout << "White pieces are capitalized, black pieces are lowercase" << endl;
            cout << "+ piece location adds a piece to the board" << endl;
            cout << "- piece location removes a piece from the board" << endl;
            cout << "= colour sets the player with colour \'colour\' to move first" << endl;
            cout << "done finishes setup mode" << endl;

            b->clearBoard();
            b->notifyObservers();
            while (true) {
                try {
                    cin >> command;
                    // Exit the program
                    if (cin.fail() || cin.eof()) {
                        cout << "Final Score:" << endl;
                        cout << "White: " << white << endl;
                        cout << "Black: " << black << endl;
                        delete b;
                        delete td;
                        return 0;
                    }
                    if (command == "+") { // Add Piece On A Square
                        char piece;
                        string pos;
                        cin >> piece >> pos;

                        b->addPiece(piece, pos); 
                        b->notifyObservers();
                        cout << *td;

                    } else if (command == "-") { // Remove Piece From Square
                        string pos;
                        cin >> pos;

                        // Validate square input
                        if (pos.length() != 2) {
                            throw (std::invalid_argument("Error: Square not found"));
                        } else if (b->getSquare(pos).getPiece() != nullptr) {
                            // Remove piece and update display
                            b->removePiece(pos);
                            b->notifyObservers();
                            cout << *td;
                        }

                    } else if (command == "=") { // Change Player
                        string colour;
                        cin >> colour;

                        // Validate player colour
                        if (colour == "White" || colour == "white" || colour == "WHITE") {
                            b->setTurn(PlayerColour::White);
                        } else if (colour == "Black" || colour == "black" || colour == "BLACK") {
                            b->setTurn(PlayerColour::Black);
                        } else {
                            throw (std::invalid_argument("Error: Invalid Player"));
                        }
                        
                    } else if (command == "done") { // Exit Setup Mode
                        if (!b->setUpValid()) {
                            throw (std::invalid_argument("Error: Invalid Setup"));
                        } else {
                            cout << "Valid setup. Type \"game <human / computer#> <human / computer#>\" to begin." << endl;
                            break;
                        }
                    } else {
                        throw (std::invalid_argument("Error: Invalid Command"));
                    }
                } catch (const std::invalid_argument &e) {
                    cerr << e.what() << endl;
                } 
            } 

        } else {
            cerr << "Invalid Command, Try Again" << endl;
            cin.clear();
        }
    }
    delete b;
    delete td;
    return 0;
}
