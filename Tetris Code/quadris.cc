#include "quadris.h"
#include <sstream>
#include <fstream>


using namespace std;
// use pattern recognize technology to read command
bool Quadris::matchcmd(const string cmd, int min, string complete) {
    if (cmd.length() > complete.length() || cmd.length() < min) {
        return false;
    }
    for (int i = 0; i < cmd.length(); ++i) {
        if (cmd.at(i) != complete.at(i)) {
            return false;
        }
    }
    return true;
}
void Quadris::printCopyright() {
    cout<<
    "Quadris. Created by Miaoqi Zhang, Runcheng Liu and Yi Ren on 2018-07.\n"
    "Copyright © 2018 VCW. All rights reserved.\n\n";
}
void Quadris::printcli() { // print command line interface
    cout<<
    "Options:\n"
    "  -text                    Run in text-only mode\n"
    "  -seed [seed]             Set the random number generator's seed\n"
    "  -scriptfile [file]       Use [file] instead of the default sequence.txt as a source of blocks for level 0\n"
    "  -startlevel [n]          Start game in level [n]\n"
    "  -help                    Display this help message\n";
}
void Quadris::printcmd() {
    cout<<
    "Commands:\n"
    "  left or a\n"
    "  right or d\n"
    "  down or s\n"
    "  clockwise or e\n"
    "  counterclockwise or q\n"
    "  levelup \n"
    "  leveldown \n"
    "  restart\n"
    "  norandom \n"
    "  random \n"
    "  sequence [file]            Executes the sequence of commands found in file\n"
    "  I, J, L, etc.              Replace current block with specified block\n"
    "  hint                       Suggests a landing place for the current block\n\n"
    "  auto                       Start AI"
    "  Commands can be shortened (eg. left -> lef)\n"
    "  and executed multiple times (eg. 2left)\n\n";
}

//return true if the input command is a BlockType (I, L, Z etc...)
bool Quadris::isBlockType(string cmd) {
    return (cmd == "I" || cmd == "J" || cmd == "L" || cmd == "O" || cmd == "S" || cmd == "Z" || cmd == "T" );
}

void Quadris::playgame(int argc, const char * argv[]) {
    cin.exceptions(ios::eofbit|ios::failbit);
    bool textonly = false;
    int seed = 1;
    string scriptfile = "sequence.txt";
    int startlevel = 0;
    int width = 11;
    int height = 18;
    Board b;
    try {
        for (int i = 1; i < argc; ++i){
            string option = argv[i];
            if (option == "-text") {
                textonly = true;
            }else if (option == "-seed") {
                ++i;
                stringstream {argv[i]} >> seed;
            }else if (option == "-scriptfile") {
                ++i;
                stringstream {argv[i]} >> scriptfile;
            }else if (option == "-startlevel") {
                ++i;
                int templevel;
                stringstream {argv[i]} >> templevel;
                if (templevel >= 0 || templevel <= 4) {
                    startlevel = templevel;
                }
            }else if (option == "-help") {
                printCopyright();
                printcli();
                printcmd();
            }else {
                cout<<"Unknown option: "<<option<<endl;
                printcli();
            }
        }
        b.init(width, height, startlevel, scriptfile, textonly, seed);
        cout << b;
        string cmd;
        ifstream in;
        
        while (true) { 
            if (!(in >> cmd)) {
                cin >> cmd;
            }
            int multiplicity = 1;
            if (cmd.at(0) <= '9' && cmd.at(0) >= '0') {
                multiplicity = cmd.at(0) - '0';
                cmd = cmd.substr(1);
            }
            if (cmd.at(0) <= '9' && cmd.at(0) >= '0') {
                multiplicity *= 10;
                multiplicity += cmd.at(0) - '0';
                cmd = cmd.substr(1);
            } 
            if (matchcmd(cmd, 2, "restart")) {
                b.init(width, height, startlevel, scriptfile, textonly, seed);
                cout << b;
                
            } else if (matchcmd(cmd, 3, "left") || cmd == "a") {
                b.left(multiplicity);
                b.autodown();
                cout << b;
            } else if (matchcmd(cmd, 2, "right") || cmd == "d") {
                b.right(multiplicity);
                b.autodown();
                cout << b;
            } else if (matchcmd(cmd, 2, "down") || cmd == "s") {
                b.down(multiplicity);
                b.autodown();
                cout << b;
            } else if (matchcmd(cmd, 2, "drop")) {
                if (!b.dropAndNextRound(multiplicity)) break;
                cout << b;
            } else if (matchcmd(cmd, 2, "clockwise") || cmd == "e") {
                b.clockwise(multiplicity);
                b.autodown();
                cout << b;
            } else if (matchcmd(cmd, 2, "counterclockwise") || cmd == "q") {
                b.counterClockwise(multiplicity);
                b.autodown();
                cout << b;
            } else if (matchcmd(cmd, 6, "levelup")) {
                b.setlevel(b.getlevel() + multiplicity);
                cout << b;
            } else if (matchcmd(cmd, 6, "leveldown")) {
                b.setlevel(b.getlevel() - multiplicity);
                cout << b;
            } else if (matchcmd(cmd, 2, "norandom")) {
                b.changeRandomState(false);
                cout << b;
            } else if (matchcmd(cmd, 2, "random")) {
                b.changeRandomState(true);
                cout << b;
            } else if (matchcmd(cmd, 2, "sequence")) {
                string file;
                cin >> file;
                if (!(in = ifstream{file})) {
                    cout << file << ": No such file or couldn't open the file " ;
                }
                
            } else if (isBlockType(cmd)) {
                b.replaceCurBlock(cmd[0]);
                cout << b;
            } else if (matchcmd(cmd, 2, "hint")) {
                b.getHint();
                cout << b;
                b.clearHint();
            } else if (matchcmd(cmd, 2, "auto")) {
                while (b.autoMove()) {
                    cout << b;
                }
                cout << b;
            } else if (matchcmd(cmd, 2, "help")) {
                printcmd();
                
            } else {
                cout << "Unknown command: " << cmd << endl;
                cout << "Please type \"help\" to see the correct command" << endl;
            }
        }
        cout << "Game Over! You scored " << b.getScore() << " points." << endl;
        cout << "Your highest score is " << b.getHiScore() << " points." << endl;
        // break the record!
    }
    catch (ios::failure &) {}  // Any I/O failure quits
}
int Quadris::play(int argc, const char * argv[]) {
    while (true) {
        playgame(argc, argv);
        cout << "Do you want to restart the game?(Y/N)" << endl;
        char c;
        while (cin >> c) {
            if (c == 'y' || c == 'Y') {
                break;
            } else if (c == 'n' || c == 'N') {
                return 0;
            } else {
                cout << "Unknown input, please input (Y/N)" << endl;
            }
        }
    }
}
