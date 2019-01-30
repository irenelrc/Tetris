#include "graphicsdisplay.h"
#include <string>
#include "window.h"

using namespace std;

GraphicsDisplay:: GraphicsDisplay(int height, int width) {
    int side = 30; // the side length of a cell
    int spacebetweenlines = 50;
    xw = make_shared<Xwindow>(650, side * height);
    int cnt = 0;
    xw->fillRectangle(0, 0, 650, side * height, backgroundclor);
    // fill the background colour
    for (int i = 15; i < side * height; i += side) {
        xw->drawString(1, i, to_string(cnt));
        cnt++;
    } // the verticle axis
    xw->fillRectangle(25, 0, 2, side * height, Xwindow::Grey19);
    xw->fillRectangle(30 + side * width, 0, 2, side * height, Xwindow::Grey19);
    xw->drawString(30 + side * width + 10, 15, "Level:");
    xw->drawString(30 + side * width + 10, 15 + spacebetweenlines, "Score:");
    xw->drawString(30 + side * width + 10, 15 + 2 * spacebetweenlines, "Hi Score:");
    xw->drawString(30 + side * width + 10, 15 + 3 * spacebetweenlines, "Blocks since last clear:");
    xw->drawString(30 + side * width + 10, 15 + 4 * spacebetweenlines, "Next:");
    // print message informations
    xw->drawBigString(30 + side * width + 10, 515, "Quadris. Created by M. Zhang, R. Liu and Y. Ren");
    xw->drawBigString(30 + side * width + 10, 530, "Copyright © 2018 VCW. All rights reserved.");
}

void GraphicsDisplay:: notify(Subject &whoNotified) {
    Info info = whoNotified.getInfo();
    int side = 30;
    int r = (int) info.col;
    int c = (int) info.row;
    BlockType type = info.type;
    switch(type) {
            // if a cell has changed, change the cell's colour according to the corresponding state
        case BlockType::I :
            xw->fillRectangle((side * r) + 30, side * c, side, side, Iclor);
            break;
        case BlockType::J :
            xw->fillRectangle((side * r) + 30, side * c, side, side, Jclor);
            break;
        case BlockType::L :
            xw->fillRectangle((side * r) + 30, side * c, side, side, Lclor);
            break;
        case BlockType::O :
            xw->fillRectangle((side * r) + 30, side * c, side, side, Oclor);
            break;
        case BlockType::S :
            xw->fillRectangle((side * r) + 30, side * c, side, side, Sclor);
            break;
        case BlockType::Z :
            xw->fillRectangle((side * r) + 30, side * c, side, side, Zclor);
            break;
        case BlockType::T :
            xw->fillRectangle((side * r) + 30, side * c, side, side, Tclor);
            break;
        case BlockType::N :
            xw->fillRectangle((side * r) + 30, side * c, side, side, Nclor);
            break;
        case BlockType::L4 :
            xw->fillRectangle((side * r) + 30, side * c, side, side, L4clor);
            break;
        case BlockType::Hint :
            xw->fillRectangle((side * r) + 30, side * c, side, side, Hclor);
            break;
    }
}

void GraphicsDisplay:: printMessage(int level, int score, int hiscore, int sinceLastclear, int height, int width, BlockType type) {
    int side = 30;
    int spacebetweenlines = 50;
    // cover/erase the original information
    xw->fillRectangle(30 + side * width + 10, 20, 200, 30, backgroundclor);
    xw->fillRectangle(30 + side * width + 10, 20 + spacebetweenlines, 200, 30, backgroundclor);
    xw->fillRectangle(30 + side * width + 10, 20 + 2 * spacebetweenlines, 200, 30, backgroundclor);
    xw->fillRectangle(30 + side * width + 10, 20 + 3 * spacebetweenlines, 200, 30, backgroundclor);
    xw->fillRectangle(30 + side * width + 10, 20 + 4 * spacebetweenlines, 4 * side, 4 * side, backgroundclor);
    // update the new information
    xw->drawString(30 + side * width + 10, 35, to_string(level));
    xw->drawString(30 + side * width + 10, 35 + spacebetweenlines, to_string(score));
    xw->drawString(30 + side * width + 10, 35 + 2 * spacebetweenlines, to_string(hiscore));
    xw->drawString(30 + side * width + 10, 35 + 3 * spacebetweenlines, to_string(sinceLastclear));
    switch(type) {
            // draw next type
        case BlockType::I :
            xw->fillRectangle(30 + side * width + 10, 30 + 4 * spacebetweenlines, 4 * side, side, Iclor);
            break;
        case BlockType::J :
            xw->fillRectangle(30 + side * width + 10, 30 + 4 * spacebetweenlines, side, side, Jclor);
            xw->fillRectangle(30 + side * width + 10, 30 + 4 * spacebetweenlines + side, 3 * side, side, Jclor);
            break;
        case BlockType::L :
            xw->fillRectangle(30 + side * width + 10 + (2 * side), 30 + 4 * spacebetweenlines, side, side, Lclor);
            xw->fillRectangle(30 + side * width + 10, 30 + 4 * spacebetweenlines + side, 3 * side, side, Lclor);
            break;
        case BlockType::O :
            xw->fillRectangle(30 + side * width + 10, 30 + 4 * spacebetweenlines, 2 * side, 2 * side, Oclor);
            break;
        case BlockType::S :
            xw->fillRectangle(30 + side * width + 10 + (1 * side), 30 + 4 * spacebetweenlines, 2 * side, side, Sclor);
            xw->fillRectangle(30 + side * width + 10, 30 + 4 * spacebetweenlines + side, 2 * side, side, Sclor);
            break;
        case BlockType::Z :
            xw->fillRectangle(30 + side * width + 10, 30 + 4 * spacebetweenlines, 2 * side, side, Zclor);
            xw->fillRectangle(30 + side * width + 10 + side, 30 + 4 * spacebetweenlines + side, 2 * side, side, Zclor);
            break;
        case BlockType::T :
            xw->fillRectangle(30 + side * width + 10, 30 + 4 * spacebetweenlines, 3 * side, side, Tclor);
            xw->fillRectangle(30 + side * width + 10 + side, 30 + 4 * spacebetweenlines + side, side, side, Tclor);
            break;
        case BlockType::N :
            break;
    }
}

GraphicsDisplay:: ~GraphicsDisplay(){
}

