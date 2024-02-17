#ifndef __GRAPHICS_DISPLAY__
#define __GRAPHICS_DISPLAY__
#include <string>
#include "observer.h"
#include "window.h"
#include "board.h"

using namespace std;

class GraphicsDisplay : public Observer {
    Xwindow xw;
    int boardSize;
    int squareSize;
    public:
        GraphicsDisplay(Xwindow &xw, int bs); // constructor for the graphics display
        void notify(Board &b); // updates the graphics display based on the cell's state
        string convertPiecetoLetter(Piece *p);
        ~GraphicsDisplay(); // destructor for the graphics display
}; 

#endif
