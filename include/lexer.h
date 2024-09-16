#ifndef MINIMOONBIT_LEXER_H
#define MINIMOONBIT_LEXER_H

#include "token.h"
#include <fstream>
#include <iostream>
#include <map>

class Lexer {
    std::ifstream fin;
    std::ofstream fout;
    std::string filename;
    char CurChar;
    int linenum;

    void nextChar();
    Token getId();
    Token getNum();
    Token getOp();
    Token getSymb();
    void skipSpace();
    

public:
    Lexer(const std::string& fn);
    Token getToken();
    ~Lexer();
    void printTokens();
};



#endif