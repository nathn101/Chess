#include "graphicsdisplay.h"

using namespace std;

GraphicsDisplay::GraphicsDisplay(Xwindow &xw, int bs): xw{xw}, boardSize{bs}, squareSize{512/bs} {
    // Initializing the graphics display to show the board
    bool even = true;
    char columnLabel = 'a';
    char rowLabel = '8';
    for (int i = 0; i < 512; i += squareSize) {
        bool alt = true;
        for (int j = 0; j < 512; j += squareSize) {
            // Configures the graphics display to show a chessboard with alternating square colours
            (even)? ((alt)? xw.fillRectangle(i, j, squareSize, squareSize, 0):xw.fillRectangle(i, j, squareSize, squareSize, 5)):((alt)? xw.fillRectangle(i, j, squareSize, squareSize, 5):xw.fillRectangle(i, j, squareSize, squareSize, 0));
            
            // switches if condition so that the colour of the square changes
            (alt)? alt = false:alt = true;

            // adding row labels
            if (j == 0) {
                string font = "fixed";
                xw.drawString(j + 5, i + 12, string(1, rowLabel), font);
                --rowLabel;
            }

            // adding column labels
            if (i == 512 - squareSize) {
                string font = "fixed";
                xw.drawString(j + 54, i + 60, string(1, columnLabel), font);
                ++columnLabel;
            }
        }
        // switches if conditions so that each row alternates pattern
        (even)? even = false:even = true;
    }
}

void GraphicsDisplay::notify(Board &b) {
    char columnLabel = 'a';
    char rowLabel = '8';

    // iterate through updated vector to redisplay the squares that updated
    for (const auto &pair : b.getUpdated()) {
        int i = pair[0];  
        int j = pair[1];  
        
        // Clears that square by drawing over it
        xw.fillRectangle(64 * j, 64 * i, squareSize, squareSize, (i + j) % 2 == 0 ? 0 : 5);

        // Updating the graphics display whenever the board updates
        if (b.getSquare(i, j).getPiece()->getType() == PieceType::Empty) {
            xw.fillRectangle(64 * j, 64 * i, squareSize, squareSize, (b.getSquare(i, j).getColour() == SquareColour::White)? 0:5);
        } else if (b.getSquare(i, j).getPiece()->getColour() == Colour::White) { // White Piece
                string font = "fixed"; // TODO -- Change back to avoid conflicts NOTE: maybe lets just keep it default font in case demo computer doesnt have a certain font
                xw.drawString(64 * j + 28, 64 * (i + 1) - 30, convertPiecetoLetter(b.getSquare(i, j).getPiece()), font);
        } else { // Black Piece
                string font  = "fixed"; // TODO -- Change back to avoid conflicts
                xw.drawBoldString(64 * j + 28, 64 * (i + 1) - 30, convertPiecetoLetter(b.getSquare(i, j).getPiece()), font);
        }

    }
    b.clearUpdated();

    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            // adding row labels so that they show up on top of previously drawn squares
            if (j == 0) {
                string font = "fixed";
                xw.drawString(64 * j + 5, 64 * i + 12, string(1, rowLabel), font);
                --rowLabel;
            }

            // adding column labels so that they show up on top of previously drawn squares
            if (i == boardSize - 1) {
                string font = "fixed";
                xw.drawString(64 * j + 54, 64 * i + 60, string(1, columnLabel), font);
                ++columnLabel;
            }
        }
    }
    
}

// Function to take in a piece pointer from a square and convert it to a letter to output to the board
string GraphicsDisplay::convertPiecetoLetter(Piece *p) {
    Colour colour = p->getColour();

    if (p->getType() == PieceType::King) {
        return (colour == Colour::White)? "K":"k";
    } else if (p->getType() == PieceType::Queen) {
        return (colour == Colour::White)? "Q":"q";
    } else if (p->getType() == PieceType::Bishop) {
        return (colour == Colour::White)? "B":"b";
    } else if (p->getType() == PieceType::Rook) {
        return (colour == Colour::White)? "R":"r";
    } else if (p->getType() == PieceType::Knight) {
        return (colour == Colour::White)? "N":"n";
    } else {
        return (colour == Colour::White)? "P":"p";
    }
}

GraphicsDisplay::~GraphicsDisplay() {}
