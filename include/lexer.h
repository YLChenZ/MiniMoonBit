#ifndef MINIMOONBIT_LEXER_H
#define MINIMOONBIT_LEXER_H

#include "token.h"
#include <fstream>
#include <iostream>
#include <map>

class Lexer {
    
    std::ofstream fout;
    std::string filename;
    char CurChar;
    int linenum;

    
    Token getId();
    Token getNum();
    Token getOp();
    Token getSymb();
    void skipSpace();
    

public:
    std::ifstream fin;
    size_t fppos;
    void nextChar();
    Lexer(const std::string& fn);
    Token getToken();
    ~Lexer();
    void printTokens();
};



#endif