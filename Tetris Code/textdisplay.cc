#include <iostream>
#include <vector>
#include <iomanip>
#include "textdisplay.h"

using namespace std;
const int row = 18;
const int col = 11;

TextDisplay::TextDisplay(){
    theDisplay=vector<vector<char>>(row, vector<char>(col,' '));
}

void TextDisplay::notify(Subject &whoNotified){
    Info i = whoNotified.getInfo();
    int r = i.row;
    int c = i.col;
    BlockType type = i.type;
    if (type == BlockType::I) {
        theDisplay[r][c] = 'I';
    }else if (type == BlockType::J) {
        theDisplay[r][c] = 'J';
    }else if (type == BlockType::L) {
        theDisplay[r][c] = 'L';
    }else if (type == BlockType::O) {
        theDisplay[r][c] = 'O';
    }else if (type == BlockType::S) {
        theDisplay[r][c] = 'S';
    }else if (type == BlockType::Z) {
        theDisplay[r][c] = 'Z';
    }else if (type == BlockType::T) {
        theDisplay[r][c] = 'T';
    }else if (type == BlockType::L4) {
        theDisplay[r][c] = '*';
    } else if (type == BlockType::Hint) {
        theDisplay[r][c] = '?';
    } else {
        theDisplay[r][c] = ' ';
    }
}
void TextDisplay::printMessage(ostream &out, int score, int level, int highscore, int blocks) {
    out<<"============="<<endl;
    out<<"level: "<<setw(7)<<level<<endl;
    out<<"score: "<<setw(7)<<score<<endl;
    out<<"Hi score: "<<setw(4)<<highscore<<endl;
    out<<"Blocks without clear: "<< blocks<< endl;/////////
    out<<"   -----------"<<endl;
}
void TextDisplay::printNext(ostream &out, BlockType type){
    out<<"   -----------"<<endl;
    out<<"Next:"<<endl;
    if (type == BlockType::I) {
        out<<"IIII"<<endl;
    }else if (type == BlockType::J) {
        out<<"J"<<endl<<"JJJ"<<endl;
    }else if (type == BlockType::L) {
        out<<"  L"<<endl<<"LLL"<<endl;
    }else if (type == BlockType::O) {
        out<<"OO"<<endl<<"OO"<<endl;
    }else if (type == BlockType::S) {
        out<<" SS"<<endl<<"SS"<<endl;
    }else if (type == BlockType::Z) {
        out<<"ZZ"<<endl<<" ZZ"<<endl;
    }else if (type == BlockType::T) {
        out<<"TTT"<<endl<<" T"<<endl;
    }
    out<<"============="<<endl;
}

ostream &operator<<(std::ostream &out, const TextDisplay &td){
    for (int i = 0; i < row; ++i) {
        out<< setw(2) << i + 1 << '|';
        for (int j = 0; j < col; ++j) {
            out << td.theDisplay[i][j];
        }
        out << '|' << endl;
    }
    return out;
}
