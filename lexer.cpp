#include "./include/lexer.h"

Lexer::Lexer(const std::string& fn)
    : CurChar(' '), filename(fn),linenum(0)
{
    fin.open(fn);
    if (!fin.is_open()){
        std::cout << "Opening file failed! Please try again!"<<'\n';
    }
    nextChar();
}

Lexer::~Lexer(){
    if (fin.is_open()){
        fin.close();
    }
}

void Lexer::nextChar(){
    CurChar = fin.get();
}
Token Lexer::getId(){
    std::string res{""};
    while(isalnum(CurChar)){
        res += CurChar;
        nextChar();
    }
    if (res == "true")
        return Token(TokenType::TRUE,res,linenum);
    if (res == "false")
        return Token(TokenType::FALSE,res,linenum);
    if (res == "Bool")
        return Token(TokenType::BOOL,res,linenum);
    if (res == "Int")
        return Token(TokenType::INT,res,linenum);
    if (res == "Double")
        return Token(TokenType::DOUBLE,res,linenum);
    if (res == "Array")
        return Token(TokenType::ARRAY,res,linenum);
    if (res == "not")
        return Token(TokenType::NOT,res,linenum);
    if (res == "if")
        return Token(TokenType::IF,res,linenum);
    if (res == "else")
        return Token(TokenType::ELSE,res,linenum);
    if (res == "fn")
        return Token(TokenType::FN,res,linenum);  
    if (res == "let")
        return Token(TokenType::LET,res,linenum);
    return Token(TokenType::IDENTIFIER,res,linenum);    
}

Token Lexer::getNum(){
    std::string num{""};
    while(isdigit(CurChar)){
        num += CurChar;
        nextChar();
    }
    return Token(TokenType::NUMBER,num,linenum);
}

Token Lexer::getOp(){
    auto op = std::string(1,CurChar);
    switch (CurChar){
        case '+':
            nextChar();
            return Token(TokenType::ADD,op,linenum);
        case '-':  //这里要区分'-' 和 '->'，需要回溯
            nextChar(); 
            if (CurChar == '>'){
                op += CurChar;
                nextChar();
                return Token(TokenType::ARROW,op,linenum);
            }
            fin.unget();  //回溯
            nextChar();
            return Token(TokenType::SUB,op,linenum);
        case '*': 
            nextChar();
            return Token(TokenType::MUL,op,linenum);
        case '/':
            nextChar(); 
            return Token(TokenType::DIV,op,linenum);
        case '=': //这里要区分'=' 和 '=='，需要回溯
            nextChar(); 
            if (CurChar == '='){
                op += CurChar;
                nextChar();
                return Token(TokenType::EQ,op,linenum);
            }
            fin.unget();  //回溯
            nextChar();
            return Token(TokenType::ASSIGN,op,linenum);
        case '<': //官方给的Tokens并没有'<'，这里回溯是为了返回类型为UNKNOWN的Token，方便提示和报错
            nextChar(); 
            if (CurChar == '='){
                op += CurChar;
                nextChar();
                return Token(TokenType::LE,op,linenum);
            }
            fin.unget();  //回溯
            nextChar();
            return Token(TokenType::UNKNOWN,op,linenum);
        case '.': 
            nextChar();
            return Token(TokenType::DOT,op,linenum);
        default:
            nextChar();
            return Token(TokenType::UNKNOWN,op,linenum);
    }
}

Token Lexer::getSymb(){
    auto symb = std::string(1,CurChar);
    switch (CurChar){
        case '(':
            nextChar();
            return Token(TokenType::LPAREN,symb,linenum);
        case ')':
            nextChar();
            return Token(TokenType::RPAREN,symb,linenum);
        case '[':
            nextChar();
            return Token(TokenType::LBRACKET,symb,linenum);
        case ']':
            nextChar();
            return Token(TokenType::RBRACKET,symb,linenum);
        case '{':
            nextChar();
            return Token(TokenType::LCURLYBRACKET,symb,linenum);
        case '}':
            nextChar();
            return Token(TokenType::RCURLYBRACKET,symb,linenum);
        case ':':
            nextChar();
            return Token(TokenType::COLON,symb,linenum);
        case ';':
            nextChar();
            return Token(TokenType::SEMICOLON,symb,linenum);
        case ',':
            nextChar();
            return Token(TokenType::COMMA,symb,linenum);
        default:
            return Token(TokenType::UNKNOWN,symb,linenum);
    }
}

void Lexer::skipSpace(){
    while (isspace(CurChar) || CurChar == '\t' || 
            CurChar == '\r' || CurChar == '\n'){
        if (CurChar == '\n')
            linenum++;
        nextChar();
    }
        
}

bool isOp(char &c){
    return c == '+' || c == '-' || c == '*' || 
        c == '/' || c == '=' || c == '<' || c == '.' ;
}

bool isSymb(char &c){
    return c == '(' || c == ')' || c == '[' || 
        c == ']' || c == '{' || c == '}' || 
        c == ':' || c == ';' || c == ',' ;
}

Token Lexer::getToken(){
    skipSpace();
    if (isalpha(CurChar))
        return getId();     
    if (isdigit(CurChar))
        return getNum();
    if (isOp(CurChar))
        return getOp();
    if (isSymb(CurChar))
        return getSymb();
    nextChar();
    return Token(TokenType::UNKNOWN,std::string(1,CurChar),linenum);
}

std::map<TokenType,std::string> TT2Str =  //这个只是为了打印结果好看点
{
    {TokenType::TRUE,"TRUE"},
    {TokenType::FALSE,"FALSE"},
    {TokenType::UNIT,"UNIT"},
    {TokenType::BOOL,"BOOL"},
    {TokenType::INT,"INT"},
    {TokenType::DOUBLE,"DOUBLE"},
    {TokenType::ARRAY,"ARRAY"},
    {TokenType::NOT,"NOT"},
    {TokenType::IF,"IF"},
    {TokenType::ELSE,"ELSE"},
    {TokenType::FN,"FN"},
    {TokenType::LET,"LET"},
    {TokenType::NUMBER,"NUMBER"},
    {TokenType::IDENTIFIER,"IDENTIFIER"},
    {TokenType::DOT,"DOT"},
    {TokenType::ADD,"ADD"},
    {TokenType::SUB,"SUB"},
    {TokenType::MUL,"MUL"},
    {TokenType::DIV,"DIV"},
    {TokenType::ASSIGN,"ASSIGN"},
    {TokenType::EQ,"EQ"},
    {TokenType::LE,"LE"},
    {TokenType::LPAREN,"LPAREN"},
    {TokenType::RPAREN,"RPAREN"},
    {TokenType::LBRACKET,"LBRACKET"},
    {TokenType::RBRACKET,"RBRACKET"},
    {TokenType::LCURLYBRACKET,"LCURLYBRACKET"},
    {TokenType::RCURLYBRACKET,"RCURLYBRACKET"},
    {TokenType::ARROW,"ARROW"},
    {TokenType::COLON,"COLON"},
    {TokenType::SEMICOLON,"SEMICOLON"},
    {TokenType::COMMA,"COMMA"},
    {TokenType::UNKNOWN,"UNKNOWN"},
};

void Lexer::printTokens(){
    std::cout << "Tokens: " << '\n';
    while (CurChar != EOF){
       Token tok = getToken();
       //std::cout << "Get Token succeed!"<<'\n';
       std::cout << "{TokenType: " <<TT2Str[tok.tokty] << ", "
                << "TokenValue: "<< tok.value <<", "
                <<  "Linenum: " << tok.linenum <<" }\n";
    }
}