#ifndef _QUADRIS_H_
#define _QUADRIS_H_
#include "board.h"

class Quadris {
    bool matchcmd(const string cmd, int min, std::string complete);
    void printCopyright();
    void printcli();
    void printcmd();
    bool isBlockType(std::string cmd);
    void playgame(int argc, const char * argv[]);
public:
    int play(int argc, const char * argv[]);
};
#endif
