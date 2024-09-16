#ifndef MINIMOONBIT_TOKEN_H
#define MINIMOONBIT_TOKEN_H

#include <string>

enum class TokenType{
    TRUE,
    FALSE,
    UNIT,
    BOOL,
    INT,
    DOUBLE,
    ARRAY,
    NOT,
    IF,
    ELSE,
    FN,
    LET,
    NUMBER,
    IDENTIFIER,
    DOT,
    ADD,
    SUB,
    MUL,
    DIV,
    ASSIGN,
    EQ,
    LE,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    LCURLYBRACKET,
    RCURLYBRACKET,
    ARROW,
    COLON,
    SEMICOLON,
    COMMA,
    WS,
    UNKNOWN
};

class Token{
public:
    TokenType tokty;
    std::string value; 
    int linenum;
    
    Token(TokenType tt, const std::string& v,int ln);
    
};

#endif