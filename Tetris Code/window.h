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
    unsigned long colours[11];
    int width, height;
    
public:
    Xwindow(int width=500, int height=500);  // Constructor; displays the window.
    ~Xwindow();                              // Destructor; destroys the window.
    Xwindow(const Xwindow&) = delete;
    Xwindow &operator=(const Xwindow&) = delete;
    
    enum {Black=0, PapayaWhip, Brown, Tomato, DarkOrange, LightSalmon, YellowGreen, PowderBlue, CadetBlue, DarkSlateGray, Grey19}; // Available colours.
    
    // Draws a rectangle
    void fillRectangle(int x, int y, int width, int height, int colour=Black);
    
    // Draws a string
    void drawString(int x, int y, std::string msg, int colour=Grey19);
    
    void drawBigString(int x, int y, std::string msg, int colour=Grey19);
    
    // Prints the first 10000 available fonts
    void showAvailableFonts();
};

#endif
