#ifndef CELL_H
#define CELL_H
#include "subject.h"

class Cell : public Subject{
    const int r, c;
    BlockType type;
    
public:
    Cell(int r, int c, BlockType type);
    Info getInfo() const override;
    void changetype(BlockType type);
};
#endif
