
#include "cell.h"

Cell::Cell(int r, int c, BlockType type):r{r}, c{c}, type{type}{}

Info Cell::getInfo() const{
    return Info{r, c, type};
}
void Cell::changetype(BlockType type){
    this->type = type;
    notifyObservers();
}
