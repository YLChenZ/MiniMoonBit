#include "./include/token.h"


Token::Token(TokenType tt, const std::string& v,int ln)
    : tokty(tt), value(v), linenum(ln)
{}

