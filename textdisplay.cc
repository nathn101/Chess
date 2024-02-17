#include "textdisplay.h"

using namespace std;

// Constructor
TextDisplay::TextDisplay (int n): gridSize{n} {
    // Initialize theDisplay
    for (int i = n; i > 0; --i) {
        std::vector<char> row;

        // Add the row labels
        for (int j = 0; j < n; ++j) {
            if ((i + j) % 2 == 1) {
                row.emplace_back('_');
            } else {
                row.emplace_back(' ');
            }
        }
        theDisplay.emplace_back(row);
    }
}

// notify function takes in a board (state?) and updates theDisplay

// maybe parse in FEN string instead of board?
void TextDisplay::notify(Board &b) {
    // Update theDisplay
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            // Piece (Uppercase for white, lowercase for black)
            switch (b.getSquare(i, j).getPiece()->getType()) {
                case PieceType::Pawn:
                    theDisplay[i][j] = (b.getSquare(i, j).getPiece()->getColour() == Colour::White) ?
                                            theDisplay[i][j] = 'P' : theDisplay[i][j] = 'p';
                    break;

                case PieceType::Rook:
                    theDisplay[i][j] = (b.getSquare(i, j).getPiece()->getColour() == Colour::White) ?
                                            theDisplay[i][j] = 'R' : theDisplay[i][j] = 'r';
                    break;
                
                case PieceType::Knight:
                    theDisplay[i][j] = (b.getSquare(i, j).getPiece()->getColour() == Colour::White) ?
                                            theDisplay[i][j] = 'N' : theDisplay[i][j] = 'n';
                    break;

                case PieceType::Bishop:
                    theDisplay[i][j] = (b.getSquare(i, j).getPiece()->getColour() == Colour::White) ?
                                            theDisplay[i][j] = 'B' : theDisplay[i][j] = 'b';
                    break;

                case PieceType::Queen:
                    theDisplay[i][j] = (b.getSquare(i, j).getPiece()->getColour() == Colour::White) ?
                                            theDisplay[i][j] = 'Q' : theDisplay[i][j] = 'q';
                    break;

                case PieceType::King:
                    theDisplay[i][j] = (b.getSquare(i, j).getPiece()->getColour() == Colour::White) ?
                                            theDisplay[i][j] = 'K' : theDisplay[i][j] = 'k';
                    break;

                // No piece
                default:
                    theDisplay[i][j] = ((i + j) % 2 == 1) ? '_' : ' ';
                    break;
            }
        }
    }
}

// Destructor
TextDisplay::~TextDisplay() {}

// Overloaded << operator
ostream &operator<<(ostream &out, const TextDisplay &td) {
    // Iterate the grid
    for (int i = 0; i <= td.gridSize; i++) {
        // Not the last row
        if (i < td.gridSize) {
            // Print the row number, and the pieces on the row
            char rowNo = 8 - i + '0';
            out << rowNo << "   ";
            for (int j = 0; j < td.gridSize; j++) {
                out << td.theDisplay[i][j] << " ";
            }
        // Last row (Letters)
        } else {
            out << endl << "    ";
            // Print the column letters
            for (int j = 0; j < td.gridSize; j++) {
                char columnNo = j + 'a';
                out << columnNo << " ";
            }
        }
        out << endl;
    }
    return out;
}
