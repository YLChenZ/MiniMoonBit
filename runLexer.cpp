#include "./include/lexer.h"

int main(){
    Lexer lex("example.txt");
    auto backPoint1 = lex.fin.tellg();
    Token t1 = lex.getToken();
    auto backPoint2 = lex.fppos;
    t1 = lex.getToken();
    std::cout << backPoint1 <<'\n';
    std::cout << backPoint2 <<'\n';
    std::cout << "t1: " << t1.value << '\n';

    // 回溯
    lex.fppos = backPoint1-1;
    lex.fin.seekg(lex.fppos);
    lex.nextChar(); // 读取下一个字符，确保指针正确位置

    Token t2 = lex.getToken();
    Token t3 = lex.getToken();
    std::cout <<lex.fppos<<'\n';
    std::cout << "t2: " << t2.value << '\n';
    std::cout << "t3: " << t3.value << '\n';
    //lex.printTokens();
}
    