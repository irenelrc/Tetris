#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H
#include <iostream>
#include <vector>
#include <memory>
#include "cell.h"


class TextDisplay: public Observer {
    std::vector<std::vector<char>> theDisplay;
public:
    TextDisplay();
    void notify(Subject &whoNotified) override;
    void printMessage(std::ostream &out, int score, int level, int highscore, int blocks); // print all the informations
    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
    void printNext(std::ostream &out, BlockType type); // print the next block 
};
#endif
