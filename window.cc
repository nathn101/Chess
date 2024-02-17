#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "window.h"

using namespace std;

Xwindow::Xwindow(int width, int height) {

  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapRaised(d, w);

  Pixmap pix = XCreatePixmap(d,w,width,
        height,DefaultDepth(d,DefaultScreen(d)));
  gc = XCreateGC(d, pix, 0,(XGCValues *)0);

  XFlush(d);
  XFlush(d);

  // Set up colours.
  XColor xcolour;
  Colormap cmap;
  char color_vals[6][10]={"white", "black", "red", "green", "blue", "brown"};

  cmap=DefaultColormap(d,DefaultScreen(d));
  for(int i=0; i < 6; ++i) {
      XParseColor(d,cmap,color_vals[i],&xcolour);
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }

  XSetForeground(d,gc,colours[Black]);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d,True);

  usleep(1000);
}

Xwindow::~Xwindow() {
  XFreeGC(d, gc);
  
  XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::setUpFont(const std::string& fontname) {
    XFontStruct * font = XLoadQueryFont(d, fontname.c_str() );
    if (! font ) {
        cerr << "unable to load font " << fontname << ": using fixed" << endl;
        font = XLoadQueryFont(d, "fixed");
    }
    XSetFont(d, gc, font->fid);
    XFreeFontInfo(NULL, font, 1);
} 

void Xwindow::drawString(int x, int y, string msg, const std::string& fontName) {
  setUpFont(fontName);
  XDrawString(d, w, gc, x, y, msg.c_str(), msg.length());
}

void Xwindow::drawBoldString(int x, int y, string msg, const std::string& fontName) {
  drawString(x, y, msg, fontName); 
  drawString(x + 1, y + 1, msg, fontName);
  }
