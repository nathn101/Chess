#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include <iostream>
#include <vector>
#include "board.h"
#include "observer.h"

class TextDisplay : public Observer{
  // The actual display board
  std::vector<std::vector<char>> theDisplay;
  // Size of the display board
  const int gridSize;
 public:
  // Constructor with an integer parameter to initialize the gridSize
  TextDisplay(int n); 
  // Notify TextDisplay of change
  void notify(Board &b) override;
  // Destructor
  ~TextDisplay();
  // Overload <<
  friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};
#endif
