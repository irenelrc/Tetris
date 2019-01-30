#include "block.h"
using namespace std;

Block::Block(BlockType type, int bornLevel, int startRow, int startCol): type{type}, bornLevel{bornLevel} {
    if(type == BlockType::L4) cellNum=1;
	else cellNum=4;
	switch(type) {
        case BlockType::I :
            components.emplace_back(Info{startRow, startCol, type});
            components.emplace_back(Info{startRow, startCol+1, type});
            components.emplace_back(Info{startRow, startCol+2, type});
            components.emplace_back(Info{startRow, startCol+3, type});
            break;
        case BlockType::J :
            components.emplace_back(Info{startRow, startCol, type});
            components.emplace_back(Info{startRow+1, startCol, type});
            components.emplace_back(Info{startRow+1, startCol+1, type});
            components.emplace_back(Info{startRow+1, startCol+2, type});
            break;
        case BlockType::L :
            components.emplace_back(Info{startRow, startCol+2, type});
            components.emplace_back(Info{startRow+1, startCol, type});
            components.emplace_back(Info{startRow+1, startCol+1, type});
            components.emplace_back(Info{startRow+1, startCol+2, type});
            break;
        case BlockType::O :
            components.emplace_back(Info{startRow, startCol, type});
            components.emplace_back(Info{startRow, startCol+1, type});
            components.emplace_back(Info{startRow+1, startCol, type});
            components.emplace_back(Info{startRow+1, startCol+1, type});
            break;
        case BlockType::S :
            components.emplace_back(Info{startRow, startCol+1, type});
            components.emplace_back(Info{startRow, startCol+2, type});
            components.emplace_back(Info{startRow+1, startCol, type});
            components.emplace_back(Info{startRow+1, startCol+1, type});
            break;
        case BlockType::Z :
            components.emplace_back(Info{startRow, startCol, type});
            components.emplace_back(Info{startRow, startCol+1, type});
            components.emplace_back(Info{startRow+1, startCol+1, type});
            components.emplace_back(Info{startRow+1, startCol+2, type});
            break;
        case BlockType::T :
            components.emplace_back(Info{startRow, startCol, type});
            components.emplace_back(Info{startRow, startCol+1, type});
            components.emplace_back(Info{startRow, startCol+2, type});
            components.emplace_back(Info{startRow+1, startCol+1, type});
            break;
        case BlockType::L4 :
            components.emplace_back(Info{startRow, startCol, type});
            break;
        default: break;
    }
}

BlockType Block::getType() const { return type; }

int Block::getBornLevel() const { return bornLevel; }

int Block::getCellNum() const { return this->cellNum; }

vector<Info> Block::getCellsInfo() const{ return this->components; }

void Block::changeToNewInfo(vector<Info> newInfo) {
    components.clear();
    components = newInfo;
}

void Block::cancel(int row) {
    for (auto &it : components) {
        if (it.type == BlockType::N) continue;  // for cells already cleared
        if (it.row == row) {   // if cell is not cleared yet and is in the deleted row
            it.type = BlockType::N;    // clear the cell
            --cellNum;
        }
        else if (it.row > row) --(it.row); // if cell is cleared yet and is above the deleted row
    }
}

int Block::countScore() const {
    if (cellNum != 0) return 0;   // the block is not totally cleared
    return (bornLevel + 1) * (bornLevel + 1);
}

bool Block::contains (Info inf) const{ 
    for (auto it : components) {
        if (it.row == inf.row && it.col == inf.col && it.type == inf.type) return true;
    }
    return false;
}



