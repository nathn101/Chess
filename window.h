#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>

class Xwindow {
  Display *d;
  Window w;
  int s;
  GC gc;
  unsigned long colours[10];

 public:
  Xwindow(int width=512, int height=512);  // Constructor; displays the window.
  ~Xwindow();                              // Destructor; destroys the window.

  enum {White=0, Black, Red, Green, Blue, Brown}; // Available colours.

  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour=Black);

  // Sets up font for string
  void setUpFont(const std::string& fontname);

  // Draws a string
  void drawString(int x, int y, std::string msg, const std::string& fontName);
  
  // Draws bolded string
  void drawBoldString(int x, int y, std::string msg, const std::string& fontName);
};
#endif
