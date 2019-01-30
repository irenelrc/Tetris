#include <time.h>
#include "board.h"

using namespace std;

vector<double> Level1= {1.0/12.0, 1.0/6.0, 2.0/6.0, 3.0/6.0, 4.0/6.0, 5.0/6.0};
vector<double> Level2= {1.0/7.0, 2.0/7.0, 3.0/7.0, 4.0/7.0, 5.0/7.0, 6.0/7.0};
vector<double> Level3= {2.0/9.0, 4.0/9.0, 5.0/9.0, 6.0/9.0, 7.0/9.0, 8.0/9.0};

struct Board::Bound {
    int top;
    int bot;
    int left;
    int right;
};

bool Board::isOccupied(int r, int c) const{
    return theBoard[r][c].getInfo().type != BlockType::N;
}

Board::Bound Board::getBound(const vector<Info> CellsInfo) const{
    Board::Bound boundary;
    int top = CellsInfo[0].row;
    int bot = top;
    int left = CellsInfo[0].col;
    int right = left;
    for (int i = 1; i < 4; ++i) {
        if (CellsInfo[i].row < top) {
            top = CellsInfo[i].row;
        }
        if (CellsInfo[i].row > bot) {
            bot = CellsInfo[i].row;
        }
        if (CellsInfo[i].col > right) {
            right = CellsInfo[i].col;
        }
        if (CellsInfo[i].col < left) {
            left = CellsInfo[i].col;
        }
    }
    boundary.top = top;
    boundary.bot = bot;
    boundary.left = left;
    boundary.right = right;
    return boundary;
}

bool Board::setSeqFile(string seqfile) {
    ifstream temp{seqfile};
    if (!temp.is_open()) {
        cout << "Could not open sequence file." << endl;
        return false;
    }
    this->file.swap(temp);
    return true;
}

// determine if the destination of a move is valid according to current board
bool Board::isValidMove(vector<Info> CellsInfo) const{
    for (const auto &it : CellsInfo) {
        if (it.row < 0 || it.row >= height || it.col < 0 || it.col >= width ||
            (isOccupied(it.row, it.col) && !blocks.back().contains(it)))
            return false;
    }
    return true;
}

// actually move the current block, return how many moves succeeded
int Board::move(Direction dir, int multi) {
    int possiblePace = 0;   // count the possible move
    
    vector<Info> oldCellsInfo = blocks.back().getCellsInfo();
    vector<Info> CellsInfo = blocks.back().getCellsInfo();
    
    for(; possiblePace < multi; ++possiblePace) {
        for (auto &it : CellsInfo) {     // change cellsinfo to their new info after moving in dir
            switch(dir) {
                case Direction::Right: it.col++; break;
                case Direction::Left: it.col--; break;
                case Direction::Down: it.row++; break;
            }
        }
        
        if (!isValidMove(CellsInfo)) break;
        // if valid move, move and return possiblePace
        blocks.back().changeToNewInfo(CellsInfo);
    }
    
    for (auto it : oldCellsInfo) theBoard[it.row][it.col].changetype(BlockType::N);
    CellsInfo = blocks.back().getCellsInfo();
    for (auto it : CellsInfo) theBoard[it.row][it.col].changetype(it.type);
    return possiblePace;
}

// return true if a specific row is empty
bool Board::isEmpty(int row) const{
    for (int i = 0; i < width; ++i) {
        Info inf = theBoard[row][i].getInfo();
        BlockType type = inf.type;
        if (type != BlockType::N) {
            return false;
        }
    }
    return true;
}

// return true if a specific row is full
bool Board::isFull(int row) const{
    for (int i = 0; i < width; ++i) {
        Info inf = theBoard[row][i].getInfo();
        BlockType type = inf.type;
        if (type == BlockType::N) {
            return false;
        }
    }
    return true;
}

// called when a specific row is full, cancel the row
void Board::cancelRow(int row){
    blocksSinceLastClear = 0;
    for (int r = row; r >= 1; --r) {
        if (isEmpty(r)) {
            break;
        }
        for (int c = 0; c < width; ++c) {
            theBoard[r][c].changetype(theBoard[r - 1][c].getInfo().type);// change the type to its upper row
        }
    }
    for (auto &i : blocks) {
        i.cancel(row);
    }
}

void Board::cancel(){
    // cancel row
    int cancelRowNum = 0;
    for (int i = 0; i < height; ++i) {
        if (isFull(i)) {
            ++cancelRowNum;
            cancelRow(i);
        }
    }
    // add score with canceled row
    if (cancelRowNum) score += (level + cancelRowNum) * (level + cancelRowNum);
    
    // cancel cleared out block and add score with cleared out block
    for (int i = blocks.size() - 1; i >= 0; --i) {
        if (blocks[i].getCellNum() == 0) {   // when all cells are cleared out (when we can get score)
            score += blocks[i].countScore();
            blocks.erase(blocks.begin() + i);
        }
    }
    
    // update hiscore
    hiscore = (hiscore > score? hiscore : score);
}

// generate next block type ranomly or according to current level
void Board::generateType(){
    if (level == -1) { // test level
        nextType = BlockType::I;
        return;
    }
    if (level == 0 || random == false) {
        string ntype;
        if (!(file >> ntype)) {
            file = ifstream {seqfile};
        }
        if (ntype == "Z") {
            nextType = BlockType::Z;
        } else if (ntype == "S") {
            nextType = BlockType::S;
        } else if (ntype == "L") {
            nextType = BlockType::L;
        } else if (ntype == "J") {
            nextType = BlockType::J;
        } else if (ntype == "I") {
            nextType = BlockType::I;
        } else if (ntype == "O") {
            nextType = BlockType::O;
        } else if (ntype == "T") {
            nextType = BlockType::T;
        }
    } else {
        vector<double> range;
        if (level == 1) {
            range = Level1;
        } else if (level == 2) {
            range = Level2;
        } else if (level == 3 || level == 4) {
            range = Level3;
        }
        double random = (rand() % 10000) / 10000.0;
        if (random <= range.at(0)) {
            nextType = BlockType::S;
        } else if (random <= range.at(1)) {
            nextType = BlockType::Z;
        } else if (random <= range.at(2)) {
            nextType = BlockType::I;
        } else if (random <= range.at(3)) {
            nextType = BlockType::O;
        } else if (random <= range.at(4)) {
            nextType = BlockType::T;
        } else if (random <= range.at(5)) {
            nextType = BlockType::L;
        } else {
            nextType = BlockType::T;
        }
    }
}

// determine if it can generate a next block, which means the game is not over
bool Board::canGenerateBlock() const{
    Block newBlock {nextType, level};
    vector<Info> newBlockCellsInfo = newBlock.getCellsInfo();
    for (auto it : newBlockCellsInfo) {
        if (isOccupied(it.row, it.col)) return false;
    }
    return true;
}

// generate next block
void Board::generateBlock() {
    if (canGenerateBlock()) {
        Block newBlock {nextType, level};
        blocks.emplace_back(newBlock);
        vector<Info> newBlockCellsInfo = blocks.back().getCellsInfo();
        for (auto it : newBlockCellsInfo) {  // change the cell in theBoard
            theBoard[it.row][it.col].changetype(it.type);
        }
        generateType();
    }
}

void Board::drawHint(vector<Info> cellsInfo) {
    for (auto it : cellsInfo) theBoard[it.row][it.col].changetype(BlockType::Hint);
}

///////////////////////////////////////////////////////////////////////////////
// The AI core algorithm is referenced in the following blog post
// http://imake.ninja/el-tetris-an-improvement-on-pierre-dellacheries-algorithm/
// It may vary a little bit
///////////////////////////////////////////////////////////////////////////////
vector<Info> Board::getHintInfo() {
    double bestScore = -1000000.0;
    
    for (int i = 0 ; i < height; ++i) {
        for (int j = 0 ; j < width; ++j) {
            if (!textonly) {
                theBoard[i][j].detach();
            }
            
        }
    }
    Block originBlock = blocks.back();
    vector<Info> originCellsInfo = originBlock.getCellsInfo();
    vector<Info> bestCellsInfo = originCellsInfo;
    
    for (int i = 0; i < 4; ++i) {
        Block NoRotateBlock = blocks.back();
        vector<Info> NoRotateCellsInfo = NoRotateBlock.getCellsInfo();
        
        clockwise(i); // rotate the block
        do {  // first loop:  from up -> bottom
            Block NoLeftRightBlock = blocks.back();
            vector<Info> NoLeftRightCellsInfo = NoLeftRightBlock.getCellsInfo();
            
            while (move(Direction::Left)) ;      // second loop:  from left -> right
            do {
                Block NoDropBlock = blocks.back();
                vector<Info> NoDropCellsInfo = NoDropBlock.getCellsInfo();
                while (move(Direction::Down)) ;
                
                // count for score for this possible block
                vector<vector<int>> numBoard;
                for (int i = 0; i < this->height; ++i) {
                    vector<int> v;
                    for (int j = 0; j < this->width; ++j) {
                        if (theBoard[i][j].getInfo().type == BlockType::N) v.emplace_back(0);
                        else v.emplace_back(1);
                    }
                    numBoard.emplace_back(v);
                }
                
                int lh = landingHeight(blocks.back().getCellsInfo());
                int re = rowsEliminated(numBoard);
                int rt = rowTransitions(numBoard);
                int ct = colTransitions(numBoard);
                int eh = emptyHoles(numBoard);
                int wn = wellNums(numBoard);
                
                double curScore = (-4.500158825082766) * lh + (3.4181268101392694) * re + (-3.2178882868487753) * rt + (-9.348695305445199) * ct + (-7.899265427351652) * eh + (-3.3855972247263626) * wn;
                
                if (curScore > bestScore) {
                    bestCellsInfo = blocks.back().getCellsInfo();
                    bestScore = curScore;
                }
                
                // change back to the block before rotation and dropping
                // first with all Cells
                vector<Info> curCellsInfo = blocks.back().getCellsInfo();
                for (auto it : curCellsInfo) theBoard[it.row][it.col].changetype(BlockType::N);
                for (auto it : NoDropCellsInfo) theBoard[it.row][it.col].changetype(it.type);
                // then with the block
                blocks.pop_back();
                blocks.emplace_back(NoDropBlock);
            } while (move(Direction::Right));
            
            // change back to the block before moving left to right
            // first with all Cells
            vector<Info> curCellsInfo = blocks.back().getCellsInfo();
            for (auto it : curCellsInfo) theBoard[it.row][it.col].changetype(BlockType::N);
            for (auto it : NoLeftRightCellsInfo) theBoard[it.row][it.col].changetype(it.type);
            // then with the block
            blocks.pop_back();
            blocks.emplace_back(NoLeftRightBlock);
            
        } while (move(Direction::Down));
        
        
        // change back to the block before rotating
        // first with all Cells
        vector<Info> curCellsInfo = blocks.back().getCellsInfo();
        for (auto it : curCellsInfo) theBoard[it.row][it.col].changetype(BlockType::N);
        for (auto it : NoRotateCellsInfo) theBoard[it.row][it.col].changetype(it.type);
        // then with the block
        blocks.pop_back();
        blocks.emplace_back(NoRotateBlock);
    }
    
    // change back to the block before any move
    // first with all Cells
    vector<Info> curCellsInfo = blocks.back().getCellsInfo();
    for (auto it : curCellsInfo) theBoard[it.row][it.col].changetype(BlockType::N);
    for (auto it : originCellsInfo) theBoard[it.row][it.col].changetype(it.type);
    // then with the block
    blocks.pop_back();
    blocks.emplace_back(originBlock);
    for (int i = 0 ; i < height; ++i) {
        for (int j = 0 ; j < width; ++j) {
            if (!textonly) {
                theBoard[i][j].attach(gd);
            }
        }
    }
    return bestCellsInfo;
}

// The height where the piece is put
// = the height of the column + (the height of the piece / 2)
int Board::landingHeight(std::vector<Info> CellsInfo) {
    Bound boundary = this->getBound(CellsInfo);
    return (height - boundary.top + height - boundary.bot) / 2;
}

// The number of rows eliminated
int Board::rowsEliminated(vector<vector<int>> numBoard) {
    int rowsEliminated = 0;
    for (int i = 0; i < height; ++i) {
        bool canEliminated = true;
        for (int j = 0; j < width; ++j) {
            if (numBoard[i][j] == 0) {
                canEliminated = false;
                break;
            }
        }
        if (canEliminated) rowsEliminated++;
    }
    return rowsEliminated;
}

// The total number of row transitions. A row transition occurs
// when an empty cell is adjacent to a filled cell on the same row and vice versa
int Board::rowTransitions(vector<vector<int>> numBoard) {
    int totalRowTrans = 0;
    for (int i = 0; i < height; ++i) {
        int preColour = 1;
        for (int j = 0; j < width; ++j) {
            if (numBoard[i][j] != preColour) ++totalRowTrans;
            preColour = numBoard[i][j];
        }
        if (preColour == 0) ++totalRowTrans;
    }
    return totalRowTrans;
}

// The total number of column transitions.
// A column transition occurs when an empty cell is adjacent
// to a filled cell on the same column and vice versa.
int Board::colTransitions(vector<vector<int>> numBoard) {
    int totalColTrans = 0;
    for (int j = 0; j < width; ++j) {
        int preColour = 1;
        for (int i = 0; i < height; ++i) {
            if (numBoard[i][j] != preColour) ++totalColTrans;
            preColour = numBoard[i][j];
        }
        if (preColour == 0) ++totalColTrans;
    }
    return totalColTrans;
}

// A hole is an empty cell that has at least one filled cell
// above it in the same column.
int Board::emptyHoles(vector<vector<int>> numBoard) {
    int totalHoles = 0;
    for ( int i = 0; i < width; ++i ) {
        int emptyHoles = 0;
        int j;
        // find a filled cell
        for ( j = 0; j < height; ++j ) {
            if (numBoard[j][i] == 1 ) {
                j++;
                break;
            }
        }
        // if we find a filled cell, we count every cell below it that is empty
        for ( ; j < height; ++j ) {
            if ( numBoard[j][i] == 0 ) {
                emptyHoles++;
            }
        }
        totalHoles += emptyHoles;
    }
    return totalHoles;
}

// A well is a succession of empty cells such that their left cells
// and right cells are both filled.
int Board::wellNums(vector<vector<int>> numBoard) {
    int totalWellDepth = 0;
    int wellDepth = 0;
    int tDepth = 0;
    // find left well
    for (int j = 0; j < height; ++j) {
        if ( numBoard[j][0] == 0 && numBoard[j][1] == 1 ) {
            tDepth++;
        } else {
            wellDepth += tDepth * (tDepth + 1) / 2;
            tDepth = 0;
        }
    }
    wellDepth += tDepth * (tDepth + 1) / 2;
    totalWellDepth += wellDepth;
    
    // find middle wells
    wellDepth = 0;
    for (int i = 0; i < width - 1; ++i) {
        tDepth = 0;
        for (int j = 0; j < height; j++) {
            if ( numBoard[j][i] == 0 && numBoard[j][i - 1] == 1 && numBoard[j][i + 1] == 1 ) {
                tDepth++;
            } else {
                wellDepth += tDepth * (tDepth + 1) / 2;
                tDepth = 0;
            }
        }
        wellDepth += tDepth * (tDepth + 1) / 2;
    }
    totalWellDepth += wellDepth;
    
    // find right wells
    wellDepth = 0;
    tDepth = 0;
    for (int j = 0; j < height; j++) {
        if ( numBoard[j][width - 1] == 0 && numBoard[j][width - 2] == 1 ) {
            tDepth++;
        } else {
            wellDepth += tDepth * (tDepth + 1) / 2;
            tDepth = 0;
        }
    }
    wellDepth += tDepth * (tDepth + 1) / 2;
    totalWellDepth += wellDepth;
    
    return totalWellDepth;
}

//----------------------------------------------------------------------------------------
//--------------------------------- below is public --------------------------------------
//----------------------------------------------------------------------------------------

void Board::init(int width, int height, int level, string seqfile, bool textonly, int seed) {
    this->width = width;
    this->height = height;
    setlevel(level);
    this->seqfile = seqfile;
    this->textonly = textonly;
    this->seed = seed;
    if (this->seed == 1) { // According to some research,
        //   If seed is set to 1, the generator is reinitialized to its initial value
        // and produces the same values as before any call to rand or srand.
        //   In order to generate random-like numbers, srand is usually initialized
        // to some distinctive runtime value, like the value returned by function time
        // (declared in header <ctime>). This is distinctive enough for most trivial
        // randomization needs. So the user input seed number to be 1, we can have random
        // like generator. 
        srand(time(NULL));
    } else {
        srand(seed);
    }
    this->score = 0;
    this->blocksSinceLastClear = 0;
    if (!setSeqFile(seqfile)) {
        exit(1);
    }
    generateType();
    vector<vector<Cell>>().swap(theBoard); // clear previous cells (if any)
    for (int i = 0; i < height; ++i) { // create theBoard
        vector<Cell> v;
        for (int j = 0; j < width; ++j) {
            Cell c{i, j, BlockType::N};
            v.push_back(c);
        }
        theBoard.push_back(v);
    }
    td = make_shared<TextDisplay>();
    if (!textonly) {
        gd = make_shared<GraphicsDisplay>(18, 11);
    }
    vector<Block>().swap(blocks); // clear previous blocks (if any)
    for (int i = 0; i < height; ++i) { // add observers
        for (int j = 0; j < width; ++j) {
            theBoard[i][j].attach(td);
            if (!textonly) {
                theBoard[i][j].attach(gd);
            }
        }
    }
    generateBlock();
}

int Board::getScore() const{ return score; }

int Board::getHiScore() const{ return hiscore; }

int Board::getBlocksSinceLastClear() const{ return blocksSinceLastClear; }

int Board::getlevel() const{ return level; }

void Board::setlevel(int level){
    if (level < 0) {
        this->level = 0;
        return;
    }
    if (level > 4 ) {
        this->level = 4;
        return;
    }
    this->level = level;
    isHeavy = false;
    if (level >= 3) {
        isHeavy = true;
        if (level == 4) {
            blocksSinceLastClear = 0;
        }
    }
}

// for level 4
void Board::drop1x1Block(){
    int midWidth = width / 2;
    int blockRow = 0;
    while(blockRow < height) {
        if (isOccupied(blockRow, midWidth)) break;
        ++blockRow;
    }
    --blockRow;
    theBoard[blockRow][midWidth].changetype(BlockType::L4);
    Block newBlock {BlockType::L4, level, blockRow, midWidth};
    blocks.emplace_back(newBlock);
}

void Board::left(int multi) { move(Direction::Left, multi); }

void Board::right(int multi) { move(Direction::Right, multi); }

void Board::down(int multi) { move(Direction::Down, multi); }

bool Board::dropAndNextRound(int multi) {
    while (multi) {
        while(move(Direction::Down, height)) ;
        ++blocksSinceLastClear;
        cancel();
        if (!canGenerateBlock()) return false;
        if (blocksSinceLastClear % 5 == 0 && blocksSinceLastClear > 0 && level == 4) {
            drop1x1Block();
        }
        generateBlock();
        --multi;
    }
    
    return true;
}

void Board::clockwise(int multi) {
    multi = multi % 4;
    int possiblePace = 0;   // count the possible move
    
    vector<Info> oldCellsInfo = blocks.back().getCellsInfo();
    vector<Info> CellsInfo = blocks.back().getCellsInfo();
    
    for(; possiblePace < multi; ++possiblePace) {
        Board::Bound boundary = getBound(CellsInfo);
        int top = boundary.top;
        int bot = boundary.bot;
        int left = boundary.left;
        int right = boundary.right;
        for (int i = 0; i < 4; ++i) {
            CellsInfo[i].row = -1 * (CellsInfo[i].row - bot);
            CellsInfo[i].col -= left;
            swap(CellsInfo[i].row, CellsInfo[i].col);
            int new_row = CellsInfo[i].col;
            int new_col = -1 * CellsInfo[i].row + (right - left);
            swap(new_row, new_col);
            new_col += left;
            new_row = -1 * (new_row - bot);
            CellsInfo[i].row = new_row;
            CellsInfo[i].col = new_col;
        }
    }
    if (isValidMove(CellsInfo)) {
        for (auto it : oldCellsInfo) theBoard[it.row][it.col].changetype(BlockType::N);
        for (auto it : CellsInfo) theBoard[it.row][it.col].changetype(it.type);
        blocks.back().changeToNewInfo(CellsInfo);
        
    }
}

void Board::counterClockwise(int multi){
    int time = 4 - (multi % 4);
    clockwise(time);
}

void Board::autodown(){
    if (this->isHeavy) down();
}

void Board::getHint() {
    auto hintInfo = getHintInfo();
    drawHint(hintInfo);
}

void Board::clearHint() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width ; ++j) {
            if (theBoard[i][j].getInfo().type == BlockType::Hint) {
                theBoard[i][j].changetype(BlockType::N);
            }
        }
    }
}

// for command "random" and "norandom"
void Board::changeRandomState(bool random){
    if (level == 3 || level == 4) {
        this->random = random;
    }
}

void Board::replaceCurBlock(char c) {
    vector<Info> oldCellsInfo= blocks.back().getCellsInfo();
    int bornLevel = blocks.back().getBornLevel();
    blocks.pop_back();
    int top = oldCellsInfo[0].row;
    int left = oldCellsInfo[0].col;
    for (int i = 1; i < 4; ++i) {
        if (oldCellsInfo[i].row < top) top = oldCellsInfo[i].row;
        if (oldCellsInfo[i].col < left) left = oldCellsInfo[i].col;
    }
    switch(c) {
        case 'I': blocks.emplace_back(Block {BlockType::I, bornLevel, top, left}); break;
        case 'J': blocks.emplace_back(Block {BlockType::J, bornLevel, top, left}); break;
        case 'L': blocks.emplace_back(Block {BlockType::L, bornLevel, top, left}); break;
        case 'O': blocks.emplace_back(Block {BlockType::O, bornLevel, top, left}); break;
        case 'S': blocks.emplace_back(Block {BlockType::S, bornLevel, top, left}); break;
        case 'Z': blocks.emplace_back(Block {BlockType::Z, bornLevel, top, left}); break;
        case 'T': blocks.emplace_back(Block {BlockType::T, bornLevel, top, left}); break;
    }
    vector<Info> cellsInfo= blocks.back().getCellsInfo();
    for (auto it : oldCellsInfo) theBoard[it.row][it.col].changetype(BlockType::N);
    for (auto it : cellsInfo) theBoard[it.row][it.col].changetype(it.type);
}

// automatically generate on the hint position
bool Board::autoMove() {
    auto hint = getHintInfo();
    auto originInfo = blocks.back().getCellsInfo();
    Block curBlock = blocks.back();
    blocks.back().changeToNewInfo(hint);
    for (auto it : originInfo) theBoard[it.row][it.col].changetype(BlockType::N);
    for (auto it : hint) theBoard[it.row][it.col].changetype(curBlock.getType());
    return dropAndNextRound();
}

std::ostream &operator<<(std::ostream &out, const Board &b){
    b.td->printMessage(out, b.score, b.level, b.hiscore, b.blocksSinceLastClear);
    out << *(b.td);
    b.td->printNext(out, b.nextType);
    if (!b.textonly) {
        b.gd->printMessage(b.level, b.score, b.hiscore, b.blocksSinceLastClear, b.height, b.width, b.nextType);
    }
    return out;
}
