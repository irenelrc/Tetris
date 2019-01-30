#ifndef BOARD_H
#define BOARD_H
#include <fstream>
#include "cell.h"
#include "block.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"

enum class Direction {Right, Left, Down};

class Board {
private:
    std::vector<std::vector<Cell>> theBoard;   // store cells' information
    std::vector<Block> blocks;   // store the exsting blocks
    int score = 0;
    int hiscore = 0;
    int blocksSinceLastClear;
    int level = 0;
    int width = 11;
    int height = 18;
    std::shared_ptr<TextDisplay> td = nullptr;
    std::shared_ptr<GraphicsDisplay> gd = nullptr;
    BlockType nextType = BlockType::N;    // type of the next block, which has not been push into blocks currently
    struct Bound; // a structure store the top/bot/left/right bound
    std::ifstream file;
    bool isHeavy = false;
    bool textonly;
    bool random = true;
    int seed;
    std::string seqfile = "sequence.txt";
    bool isSeedSet;
    
    bool isOccupied(int r, int c) const;   // return true if cell in (r, c) has been occupied
    Board::Bound getBound(const vector<Info> CellsInfo) const;  // get the Bound of passed block of CellsInfo
    
    // Setting the sequence file
    bool setSeqFile(string seqfile);
    
    // Moving
    bool isValidMove(std::vector<Info> CellsInfo) const;
    // return true if CellsInfo is valid in the current board
    int move(Direction dir, int multi = 1);
    // return possiblePaces (<= multi) in dir, return 0 if no move
    
    // Cancelling
    bool isEmpty(int row) const;   // check if a row is empty
    bool isFull(int row) const;    // check if a row is full
    void cancelRow(int row);  // cancel the row
    void cancel();    // cancel all possible row
    
    // Generating next block
    void generateType();    // update newType with nextType
    bool canGenerateBlock() const;    // return true if can generate next block, otherwise return false
    void generateBlock();   // use nextType to generate next Block

    // Hint
    void drawHint(vector<Info> cellsInfo);
    vector<Info> getHintInfo();
    
    // Counting preformance of different possible hint blocks
    int landingHeight(std::vector<Info> CellsInfo);
    int rowsEliminated(vector<vector<int>> numBoard);
    int rowTransitions(vector<vector<int>> numBoard);
    int colTransitions(vector<vector<int>> numBoard);
    int emptyHoles(vector<vector<int>> numBoard);
    int wellNums(vector<vector<int>> numBoard);
    
public:
    // Initialization
    void init(int width, int height, int level, std::string file, bool textonly, int seed);
    
    // Information printing on the board
    int getScore() const;   // return the score
    int getHiScore() const;   // return the hiscore
    int getBlocksSinceLastClear() const;   // return blocksSinceLastClear
    int getlevel() const;   // return current level
    
    // Level
    void setlevel(int level);
    void drop1x1Block();  // Drop the 1x1Block in Level4
    
    // Move
    void left(int multi = 1);
    void right(int multi = 1);
    void down(int multi = 1);
    bool dropAndNextRound(int multi = 1); // drop the current block and generate next block, return true if succeed, return false if cannot generate next block
    void clockwise(int multi = 1);
    void counterClockwise(int multi = 1);
    void autodown();  // down when the level is isHeavy
    
    // Hint
    void getHint();
    void clearHint();
    
    // Other Commands
    void changeRandomState(bool random); // for command "random" and "norandom"
    void replaceCurBlock(char c);   // replace current undropped block with type c
    bool autoMove();   // automatically play the game and put blocks based on the hint position
    
    // Output
    friend std::ostream &operator<<(std::ostream &out, const Board &b);
};

#endif
