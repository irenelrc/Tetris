#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H
#include "window.h"
#include "subject.h"
#include "blocktype.h"
#include <memory>


class GraphicsDisplay:public Observer{
    shared_ptr<Xwindow>xw;
    int backgroundclor = Xwindow::PapayaWhip;
    int Iclor = Xwindow::DarkOrange;
    int Jclor = Xwindow::CadetBlue;
    int Lclor = Xwindow::LightSalmon;
    int Oclor = Xwindow::YellowGreen;
    int Sclor = Xwindow::PowderBlue;
    int Zclor = Xwindow::Tomato;
    int Tclor = Xwindow::DarkSlateGray;
    int Nclor = Xwindow::PapayaWhip;
    int L4clor = Xwindow::Brown;
    int Hclor = Xwindow::Black; // the colour of each type resp.
public:
    GraphicsDisplay(int width, int height);
    void notify(Subject &whoNotified) override;
    void printMessage(int level, int score, int hiscore, int sinceLastclear, int height, int width, BlockType type);
    // print the current game message on the window
    ~GraphicsDisplay();
};



#endif
