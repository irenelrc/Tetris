#ifndef BLOCK_H
#define BLOCK_H
#include <vector>
#include "info.h"

class Block {
private:
    BlockType type;
    int bornLevel;    // the level when this block is born
    int cellNum;      // the number of cell have not been cleared
    std::vector<Info> components;    // cell's information

public:
    Block(BlockType type, int bornLevel, int startRow=3, int startCol=0);   // ctor of Block
    // startRow and startCol is the topleft num of the block
    
    BlockType getType() const;     //return type of block
    int getBornLevel() const;    // return bornLevel
    int getCellNum() const;    // return cellNum
    std::vector<Info> getCellsInfo() const;   // return components
    
    void changeToNewInfo(std::vector<Info> newInfo);    // change components to vectors of newInfo
    void cancel(int row);     // cancel all the cells in the row in the blocks and update the information
    int countScore() const;    // return 0 if block can not be scored(have not been totally cleared yet)
                               // or return num (> 0) which is its score
    bool contains (Info inf) const;    // return true if Info if is one of block's components
};


#endif
