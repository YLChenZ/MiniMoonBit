#include "./include/parser.h"

Parser::Parser(const std::string& fn)
    : filename(fn), CurTok{Token(TokenType::UNKNOWN,"",0)},
        lex(fn)
{
    nextToken();
}

Parser::~Parser(){
    if (lex.fin.is_open())
        lex.fin.close();
}

void Parser::nextToken(){
    CurTok = lex.getToken();
}

void Parser::parseProg(){
    while(CurTok.value == "let" || CurTok.value == "fn"){
        parseTopLevel();
        nextToken();
    }
    if (CurTok.value[0] == EOF)
        return;
    else {
        std::cout << "Parse error in Prog!"<<'\n';
        return;
    }
    return;
}

void Parser::parseTopLevel(){
    std::cout <<CurTok.value<<" in TopLevel"<<'\n';
    if (CurTok.value == "let"){
        parseTopLetDecl();
        return;
    }  
    if (CurTok.value == "fn"){
        parseTopLevelFnDecl();
        return;
    }
    std::cout << "Parse error in TopLevel!" << '\n';
    return;
}

void Parser::parseTopLetDecl(){
    nextToken();
    if (CurTok.tokty != TokenType::IDENTIFIER){
        std::cout << "Excepted an identifier in TopLetDecl!"<<'\n';
        return;
    }
    std::cout <<CurTok.value<<" in TopLetDecl" <<'\n';
    nextToken();
    if (CurTok.value != ":"){
         std::cout << "Excepted ':' in TopLetDecl!"<<'\n';
        return;
    }
    nextToken();
    parseType();
    nextToken();
    if (CurTok.value != "="){
        std::cout << "Excepted '=' in TopLetDecl!"<<'\n';
        return;
    }
    nextToken();
    parseExpr();
    nextToken();
    if (CurTok.value == ";")
        return;
    std::cout << "Excepted ';' in TopLetDecl!"<<'\n';
    return;
}

void Parser::parseTopLevelFnDecl(){
    nextToken();
    std::cout <<CurTok.value<<" in TopLevelFnDecl"<<'\n';
    if (CurTok.value == "main" || CurTok.value == "init")
        parseMainFnDecl();
    if (CurTok.tokty == TokenType::IDENTIFIER)
        parseTopFnDecl();
    //std::cout << "Parse error in TopLevelFnDecl!"<<'\n';
    return;
}

void Parser::parseMainFnDecl(){
    nextToken();
    parseFnBody();
    return;
}

void Parser::parseTopFnDecl() {
    nextToken();
    std::cout <<CurTok.value<<" in TopFnDecl"<<'\n';
    if (CurTok.value != "(") {
        std::cout << "Expected '(' after parameters in TopFnDecl!" << '\n';
        return;
    }
    nextToken();
    std::cout <<CurTok.value<<" in TopFnDecl"<<'\n';
    if (CurTok.value == ")") {// 空参数列表
        nextToken();
    } else {
        parseParamList();     // 非空参数列表
        if (CurTok.value != ")") {
            std::cout << "Expected ')' after parameters in TopFnDecl!" << '\n';
            return;
        }
        std::cout <<CurTok.value<<"in TopFnDecl"<<'\n';
        nextToken();
    }
    if (CurTok.value == "->") {// 返回类型
        std::cout <<CurTok.value<<" in TopFnDecl"<<'\n';
        nextToken();
        parseType();
        nextToken();
        parseFnBody();
    } else {
        std::cout << "Expected '->' in TopFnDecl!" << '\n';
        return;
    }
    return;
}


void Parser::parseParamList(){
    parseParam();
    nextToken();
    while (CurTok.value == ","){
        nextToken();
        parseParam();
        nextToken();
    }
    return;
}

void Parser::parseParam(){
    if (CurTok.tokty == TokenType::IDENTIFIER){
        nextToken();
        parseTypeAnnotation();
        return;
    }
    std::cout<<"Parse error in Param (excepted an identifier)!"<<'\n';
    return;
}

void Parser::parseFnBody(){
    std::cout <<CurTok.value<<" in FnBody"<<'\n';
    if (CurTok.value != "{"){
        std::cout<<"Excepted '{' in TopFnDecl!"<<'\n';
        return;
    }
    //nextToken();
    std::cout << "here0" <<'\n';
    parseStmt();
    nextToken();
    if (CurTok.value != "}"){
        std::cout<<"Excepted '}' in TopFnDecl!"<<'\n';
        return;
    }
    std::cout <<CurTok.value<<" in FnBody"<<'\n';
    return;
}

void Parser::parseNontopFnDecl(){
    //todo
}

void Parser::parseNontopParamList(){
    //todo
}

void Parser::parseNontopParam(){
    //todo
}

void Parser::parseStmt(){
	auto backPoint = lex.fppos;
	nextToken();
    std::cout<<CurTok.value <<" in Stmt"<<'\n';
    if (CurTok.value == "let"){
        nextToken();
        if (CurTok.value == "("){
            parseLetTupleStmt();
            return;
        }
        parseLetStmt();
        return;
    }
    if (CurTok.value == "fn"){
        parseFnDeclStmt();
        return;
    }
    //区分assign_stmt和expr_stmt需要回溯Token，暂时的想法是：
    //1.利用循环unget()，这样感觉很慢
    //2.事先将Tokens全部输入到容器中（token较少时可以，token太多则可能溢出）
    //3. 组合利用tellg()和seekg()来回溯
    
    bool flag = false;
    if (true) {
        while (CurTok.value != "[" && CurTok.value != "}"){
            std::cout<<CurTok.value <<" for finding '['"<<'\n';
            nextToken();
        } 
        if (CurTok.value == "}")
            flag == false;
        else flag = true;
    }
    lex.fppos = backPoint;
    lex.fin.seekg(lex.fppos-1);
    lex.nextChar();
    lex.fppos--;
    nextToken();
    std::cout<<CurTok.value <<" in Stmt"<<'\n';
    if (flag){
        parseAssignStmt();
        return;
    }
    std::cout << "here00" <<'\n';
    //std::cout << CurTok.value <<'\n';
    parseExprStmt();
    return;
}


void Parser::parseLetTupleStmt(){
    nextToken();
    if (CurTok.tokty != TokenType::IDENTIFIER){
        std::cout <<"Excepted an identifier in LetTupleStmt!"<<'\n';
        return;
    }
    nextToken();
    while (CurTok.value == ","){
        nextToken();
        if (CurTok.tokty == TokenType::IDENTIFIER)
            nextToken();
        std::cout <<"Excepted an identifier in LetTupleStmt!"<<'\n';
        return;
    }
    if (CurTok.value != ")"){
        std::cout <<"Excepted ')' in LetTupleStmt!"<<'\n';
        return;
    }
    nextToken();
    parseTypeAnnotation();
    nextToken();
    if (CurTok.value != "="){
        std::cout <<"Excepted '=' in LetTupleStmt!"<<'\n';
        return;
    }
    nextToken();
    parseExpr();
    nextToken();
    if (CurTok.value != ";"){
        std::cout <<"Excepted ';' in LetTupleStmt!"<<'\n';
        return;    
    }
    nextToken();
    parseStmt();
    return;    
}

void Parser::parseLetStmt(){
    if (CurTok.tokty == TokenType::IDENTIFIER){
        std::cout <<"Excepted an identifier in LetStmt!"<<'\n';
        return;
    }
    nextToken();
    parseTypeAnnotation();
    if (CurTok.value != "="){
        std::cout <<"Excepted '=' in LetStmt!"<<'\n';
        return;
    }
    nextToken();
    parseExpr();
    nextToken();
    if (CurTok.value != ";"){
        std::cout <<"Excepted ';' in LetStmt!"<<'\n';
        return;
    }
    nextToken();
    parseStmt();
    return;
}

void Parser::parseTypeAnnotation(){
    if (CurTok.value != ":"){
        std::cout <<"Excepted ':' in TypeAnnotation!"<<'\n';
        return;
    }
    std::cout <<CurTok.value<<" in TypeAnnotation"<<'\n';
    nextToken();
    parseType();
    return;
}

void Parser::parseFnDeclStmt(){
    parseTopFnDecl();
    nextToken();
    if (CurTok.value != ";"){
        std::cout<<"Excepted ';' in TopFnDecl!"<<'\n';
        return;
    }
    parseStmt();
    return;
}

void Parser::parseAssignStmt(){
    parseGetExpr();
    if (CurTok.value != "="){
        std::cout<<"Excepted '=' in AssignStmt!"<<'\n';
        return;
    }
    nextToken();
    parseExpr();
    nextToken();
    if (CurTok.value != ";"){
        std::cout<<"Excepted ';' in AssignStmt!"<<'\n';
        return;
    }
    nextToken();
    parseStmt();
    return;
}

void Parser::parseExprStmt(){
    parseExpr();
    return;
}
void Parser::parseExpr(){
	std::cout <<"here1"<<'\n';
    parseAddSubLevelExpr();
    std::cout <<"here1 back"<<'\n';
    auto backPoint = lex.fppos;
    std::cout <<CurTok.value<<" in Expr"<<'\n';
    nextToken();
    //nextToken();
    std::cout <<CurTok.value<<" in Expr"<<'\n';
    if (CurTok.value == "==" || CurTok.value == "<="){
    	std::cout <<CurTok.value<<" in Expr"<<'\n';
        nextToken();
        parseExpr();
        std::cout <<"here1"<<'\n';
        return;
    }
    else {
        lex.fppos = backPoint;
        lex.fin.seekg(lex.fppos-1);
        lex.nextChar();
        lex.fppos--;
    }
    return;
}

void Parser::parseAddSubLevelExpr(){
std::cout <<"here2"<<'\n';
    parseMulDivLevelExpr();
    auto backPoint = lex.fppos;
    nextToken();
    if (CurTok.value == "+" || CurTok.value == "-"){
    	std::cout <<CurTok.value<<'\n';
        nextToken();
        parseAddSubLevelExpr();
        return;
    }
    else {
        lex.fppos = backPoint;
        lex.fin.seekg(lex.fppos-1);
        lex.nextChar();
        lex.fppos--;
    }
    return;
}
void Parser::parseMulDivLevelExpr(){
std::cout <<"here3"<<'\n';
    parseIfLevelExpr();
    auto backPoint = lex.fppos;
    nextToken();
    if (CurTok.value == "*" || CurTok.value == "/"){
    	std::cout <<CurTok.value<<'\n';
        nextToken();
        parseMulDivLevelExpr();
        return;
    }
    else {
        lex.fppos = backPoint;
        lex.fin.seekg(lex.fppos-1);
        lex.nextChar();
        lex.fppos--;
    }
    return;
}
void Parser::parseIfLevelExpr(){
std::cout <<"here4"<<'\n';
    if (CurTok.value == "if"){
    	//std::cout <<CurTok.value<<'\n';
        parseIfExpr();
        return;
    }
    else parseGetOrApplyLevelExpr();
    return;
}
void Parser::parseIfExpr(){
	std::cout << "here9" <<'\n';
    nextToken();
    parseExpr();
    std::cout << "here9" <<'\n';
    nextToken();
    parseBlockExpr();
    auto backPoint = lex.fppos;
    nextToken();
    if (CurTok.value == "else"){
    	std::cout <<CurTok.value<<" in IfExpr"<<'\n';
        nextToken();
        parseBlockExpr();
        return;
    }
    else {
        lex.fppos = backPoint;
        lex.fin.seekg(lex.fppos-1);
        lex.nextChar();
        lex.fppos--;
    }
    return;
}
void Parser::parseGetOrApplyLevelExpr(){
    auto backPoint = lex.fppos;
    std::cout << "here5" <<'\n';
    std::cout <<backPoint <<"here5" <<'\n';
    parseValueExpr();
    std::cout << "here5 back"<<'\n';
    nextToken();
    auto backPoint1 = lex.fppos;
    std::cout <<backPoint1 <<"here5" <<'\n';
    std::cout << CurTok.value << " in GetOrApplyLevelExpr"<<'\n';
    if (CurTok.value == "["){
        lex.fppos = backPoint;
        lex.fin.seekg(lex.fppos-1);
        lex.nextChar();
        lex.fppos--;
        nextToken();
        parseGetExpr();
        return;
    }
    if (CurTok.value == "("){
        lex.fppos = backPoint;
        lex.fin.seekg(lex.fppos-1);
        lex.nextChar();
        lex.fppos--;
        nextToken();
        parseApplyExpr();
        return;
    }
    lex.fppos = backPoint1;
    lex.fin.seekg(lex.fppos-1);
    lex.nextChar();
    lex.fppos--;
    nextToken();
    std::cout << CurTok.value << " in GetOrApplyLevelExpr"<<'\n';
    return;
}
void Parser::parseGetExpr(){
    parseValueExpr();
    nextToken();
    std::cout <<CurTok.value<<" in GetExpr"<<'\n';
    if (CurTok.value != "["){
        std::cout <<"Excepted '[' in GetExpr"<<'\n';
        return;
    }
    nextToken();
    parseExpr();
    nextToken();
    if (CurTok.value != "]"){
        std::cout <<"Excepted ']' in GetExpr"<<'\n';
        return;
    }
    return;
}

void Parser::parseApplyExpr(){ //EmptyApplyExpr和NonemptyApplyExpr放在一起
    parseValueExpr();
    nextToken();
    if (CurTok.value != "("){
        std::cout <<"Excepted '(' in ApplyExpr"<<'\n';
        return; 
    }
    nextToken();
    if (CurTok.value != ")"){
        nextToken();
        parseExpr();
        nextToken();
        while (CurTok.value == ","){
            nextToken();
            parseExpr();
            nextToken();
        }
        if (CurTok.value == ")")
            return;
        std::cout <<"Excepted ')' in NonemptyApplyExpr"<<'\n';
        return;
    }
    return;
}
void Parser::parseEmptyApplyExpr(){
    //
}
void Parser::parseNonemptyApplyExpr(){
    //
}
void Parser::parseValueExpr(){
    // unit_expr和tuple_type要区分开，需要多看一个Token
    //int_expr,floating_point_expr区分开，也需要多看一个token
    //block_expr和前面的fn_body是一样的
    std::cout << "here6" <<'\n';
    auto backPoint = lex.fppos;
    if (CurTok.value == "("){ 
        nextToken();
        if (CurTok.value == ")"){ 
            return;         //unit_expr
        }
        lex.fppos = backPoint;
        lex.fin.seekg(lex.fppos-1);
        lex.nextChar();
        lex.fppos--;
        nextToken();
        parseTupleExpr();   //tuple_type
        return;
    }
    if (CurTok.value == "{"){
        parseBlockExpr();
        return;
    }
    if (CurTok.value == "true" || CurTok.value == "false"){
        parseBoolExpr();
        return;
    }
    if (CurTok.value == "-"){
        parseNegExpr();
        return;
    }
    if (CurTok.value == "not"){
        parseNotExpr();
        return;
    }
    if (CurTok.value == "Array"){
        parseArrayMakeExpr();
        return;
    }    
    if (CurTok.tokty == TokenType::IDENTIFIER){
    	std::cout << "here10"<<'\n';
        parseIdentifierExpr();
        return;
    } 
    if (CurTok.tokty == TokenType::NUMBER){
    	std::cout << "here7" <<'\n';
        nextToken();
        if (CurTok.value == "."){
            auto backPoint1 = lex.fppos;
            nextToken();
            if (CurTok.tokty == TokenType::NUMBER){ 
                return; //1.1
            }
            lex.fppos = backPoint;
            lex.fin.seekg(lex.fppos-1);
            lex.nextChar();
            lex.fppos--;
            //nextToken();
            return; //1.
        }
        std::cout << "here8" <<'\n';
        lex.fppos = backPoint;
        lex.fin.seekg(lex.fppos-1);
        lex.nextChar();
        lex.fppos--;
        //nextToken();
        std::cout << "here8" <<'\n';
        //std::cout << CurTok.value <<'\n';
        return; //1
    }
    return;
}
void Parser::parseUnitExpr(){
    //
}
void Parser::parseTupleExpr(){
    nextToken();
    parseExpr();
    nextToken();
    while (CurTok.value == ","){
        nextToken();
        parseExpr();
        nextToken();
    }
    if (CurTok.value == ")"){
        return;
    }
    std::cout<<"Excepted ')' in TupleExpr!"<<'\n';
    return;
}
void Parser::parseBlockExpr(){
	std::cout << "here11" <<'\n';
    //nextToken();
    parseStmt();
    nextToken();
    std::cout <<CurTok.value <<'\n';
    if (CurTok.value != "}"){
        std::cout<<"Excepted '}' in BlockExpr!"<<'\n';
        return;
    } 
    return;
}
void Parser::parseBoolExpr(){
    std::cout << CurTok.value << " in BoolExpr"<<'\n';
}
void Parser::parseNegExpr(){
    nextToken();
    parseValueExpr();
}
void Parser::parseFloatingPointExpr(){
    //
}
void Parser::parseIntExpr(){
    std::cout<<CurTok.value<<'\n';
}
void Parser::parseNotExpr(){
    nextToken();
    if (CurTok.value != "("){
        std::cout<<"Excepted '(' in NotExpr!"<<'\n';
        return;
    }
    nextToken();
    parseExpr();
    nextToken();
    if (CurTok.value != ")"){
        std::cout<<"Excepted ')' in NotExpr!"<<'\n';
        return;
    }
    return;  
}
void Parser::parseArrayMakeExpr(){
    nextToken();
    if (CurTok.value != ":"){
        std::cout<<"Excepted ':' in ArrayMakeExpr!"<<'\n';
        return;
    }
    nextToken();
    if (CurTok.value != ":"){
        std::cout<<"Excepted ':' in ArrayMakeExpr!"<<'\n';
        return;
    }
    nextToken();
    if (CurTok.value != "make"){
        std::cout<<"Excepted 'make' in ArrayMakeExpr!"<<'\n';
        return;
    }
    nextToken();
    if (CurTok.value != "("){
        std::cout<<"Excepted '(' in ArrayMakeExpr!"<<'\n';
        return;
    }
    nextToken();
    parseExpr();
    nextToken();
    if (CurTok.value != ","){
        std::cout<<"Excepted ',' in ArrayMakeExpr!"<<'\n';
        return;
    }
    nextToken();
    parseExpr();
    nextToken();
    if (CurTok.value != ")"){
        std::cout<<"Excepted ')' in ArrayMakeExpr!"<<'\n';
        return;
    }
    return;
}

void Parser::parseIdentifierExpr(){
    std::cout<<CurTok.value<< " in IdentifierExpr"<<'\n';
}
bool Parser::isBaseType(){
    auto v = CurTok.value;
    return v == "Unit" || v == "Bool" || v == "Int" || v == "Double";
}
void Parser::parseType(){
    if (isBaseType())
        std::cout <<CurTok.value<<" in Type"<<'\n'; //目前只打印基本类型
    if (CurTok.value == "Array")
        parseArrayType();
    if (CurTok.value == "("){
        nextToken();
        parseType();
        nextToken();
        while (CurTok.value == ","){
            nextToken();
            parseType();
            nextToken();
        }
        if (CurTok.value != ")"){ 
            std::cout<<"Excepted ')' in tuple_type or function_type!"<<'\n';
            return;
        }
        auto backPoint = lex.fppos;
        nextToken(); 
        if (CurTok.value != "->"){
            lex.fppos = backPoint;
            lex.fin.seekg(lex.fppos-1);
            lex.nextChar();
            lex.fppos--;
            nextToken();
            return;  //tuple_type
        }
        nextToken();
        parseType();
        return;  //function_type
    }
    //std::cout<<"Unknown type!"<<'\n';
    return;
}
void Parser::parseArrayType(){
    nextToken();
    if (CurTok.value != "["){
        std::cout<<"Excepted '[' in ArrayType!"<<'\n';
        return;
    }
    nextToken();
    parseType();
    nextToken();
    if (CurTok.value != "]"){
        std::cout<<"Excepted ']' in ArrayType!"<<'\n';
        return;
    }
    return;
}