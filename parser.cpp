#include "./include/parser.h"

Parser::Parser(const std::string& fn)
    : filename(fn), CurTok{Token(TokenType::UNKNOWN,"",0)},
        lex(fn)
{}

void Parser::nextToken(){
    CurTok = lex.getToken();
}

void Parser::parseTopLevel(){
    if (CurTok.value == "let")
        parseTopLetDecl();
    if (CurTok.value == "fn")
        parseTopLevelFnDecl();
    std::cout << "Parse error in TopLevel!" << '\n';
}

void Parser::parseTopLetDecl(){
    nextToken();
    if (CurTok.tokty == TokenType::IDENTIFIER){
        nextToken();
        if (CurTok.value == ":"){
            nextToken();
            parseType();
            nextToken();
            if (CurTok.value == "="){
                nextToken();
                parseExpr();
                nextToken();
                if (CurTok.value == ";")
                    return;
                else 
                    std::cout << "Excepted ';' in TopLetDecl!"<<'\n';
            }
            else 
                std::cout << "Excepted '=' in TopLetDecl!"<<'\n';
        }
        else 
            std::cout << "Excepted ':' in TopLetDecl!"<<'\n';
    }
    else 
        std::cout << "Excepted an identifier in TopLetDecl!"<<'\n';
}

void Parser::parseTopLevelFnDecl(){
    nextToken();
    if (CurTok.value == "main" || CurTok.value == "init")
        parseMainFnDecl();
    if (CurTok.tokty == TokenType::IDENTIFIER)
        parseTopFnDecl();
    std::cout << "Parse error in TopLevelFnDecl!"<<'\n';
}

void Parser::parseMainFnDecl(){
    nextToken();
    parseFnBody();
}

void Parser::parseTopFnDecl(){
    nextToken();
    if (CurTok.value == "("){
        nextToken();
        if (CurTok.value == ")"){
            nextToken();
            if (CurTok.value == "->"){
                nextToken();
                parseType();
                parseFnBody();
            }
            else 
                std::cout<<"Excepted '->' in TopFnDecl!"<<'\n';
        }
        else {
            parseParamList();
            //nextToken();
            if (CurTok.value == ")"){
                nextToken();
                if (CurTok.value == "->"){
                    nextToken();
                    parseType();
                    parseFnBody();
                }
                else 
                    std::cout<<"Excepted '->' in TopFnDecl!"<<'\n';
            }
        }
    }
    else std::cout<<"Parse error in TopFnDecl!"<<'\n';
    return;
}

void Parser::parseParamList(){
    parseParam();
    nextToken();
    while (true){
        if (CurTok.value == ","){
            nextToken();
            parseParam();
            nextToken();
        }
        break;
    }
    return;
}

void Parser::parseParam(){
    if (CurTok.tokty == TokenType::IDENTIFIER)
        parseTypeAnnotation();
    else std::cout<<"Parse error in Param!"<<'\n';
    return;
}

void Parser::parseFnBody(){
    if (CurTok.value == "{"){
        nextToken();
        parseStmt();
        nextToken();
        if (CurTok.value == "}")
            return;
        else 
            std::cout<<"Excepted '}' in TopFnDecl!"<<'\n';
    }
    else std::cout<<"Excepted '{' in TopFnDecl!"<<'\n';
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
    if (CurTok.value == "let"){
        nextToken();
        if (CurTok.value == "(")
            parseLetTupleStmt();
        else
            parseLetStmt();
    }
    if (CurTok.value == "fn")
        parseFnDeclStmt();
    //区分assign_stmt和expr_stmt需要回溯Token，暂时的想法是：
    //1.利用循环unget()，这样感觉很慢
    //2.事先将Tokens全部输入到容器中（token较少时可以，token太多则可能溢出）
    //parseAssignStmt,parseExprStmt  todo
    return;
}


void Parser::parseLetTupleStmt(){
    nextToken();
    if (CurTok.tokty == TokenType::IDENTIFIER){
        nextToken();
        while (true){
            if (CurTok.value == ","){
                nextToken();
                if (CurTok.tokty == TokenType::IDENTIFIER)
                    nextToken();
                else {
                    std::cout <<"Excepted an identifier in LetTupleStmt!"<<'\n';
                    break;
                }
            }
            std::cout <<"Excepted ';' in LetTupleStmt!"<<'\n';
            break;
        }
        if (CurTok.value == ")"){
            nextToken();
            if (CurTok.value == ":")
                parseTypeAnnotation();
            if (CurTok.value == "="){
                nextToken();
                parseExpr();
                nextToken();
                if (CurTok.value == ";"){
                    nextToken();
                    parseStmt();
                }
                else 
                    std::cout <<"Excepted ';' in LetTupleStmt!"<<'\n';
            }
            else std::cout <<"Excepted ':' or '=' in LetTupleStmt!"<<'\n';
        }
        else std::cout <<"Excepted ')' in LetTupleStmt!"<<'\n';
    }
    else std::cout <<"Excepted an identifier in LetTupleStmt!"<<'\n';
    return;
}

void Parser::parseLetStmt(){
    if (CurTok.tokty == TokenType::IDENTIFIER){
        nextToken();
        if (CurTok.value == ":")
            parseTypeAnnotation();
        if (CurTok.value == "="){
            nextToken();
            parseExpr();
            nextToken();
            if (CurTok.value == ";"){
                nextToken();
                parseStmt();
            }
            else std::cout <<"Excepted ';' in LetStmt!"<<'\n';
        }
        else std::cout <<"Excepted ':' or '=' in LetStmt!"<<'\n';
    }
    else std::cout <<"Excepted an identifier in LetStmt!"<<'\n';
    return;
}

void Parser::parseTypeAnnotation(){
    nextToken();
    parseType();
}
void Parser::parseFnDeclStmt(){
    parseTopFnDecl();
    nextToken();
    if (CurTok.value == ";")
        parseStmt();
    else
        std::cout<<"Excepted ';' in TopFnDecl!"<<'\n';
    return;
}
void Parser::parseAssignStmt(){
    parseGetExpr();
    if (CurTok.value == "="){
        nextToken();
        parseExpr();
        nextToken();
        if (CurTok.value == ";"){
            nextToken();
            parseStmt();
        }
        else std::cout<<"Excepted ';' in AssignStmt!"<<'\n';
    }
    else std::cout<<"Excepted '=' in AssignStmt!"<<'\n';
    return;
}
void Parser::parseExprStmt(){
    parseExpr();
    return;
}
void Parser::parseExpr(){
    // unit_expr和tuple_type要区分开，需要多看一个Token
    //todo
}

void Parser::parseAddSubLevelExpr(){
    //todo
}
void Parser::parseMulDivLevelExpr(){
    //todo
}
void Parser::parseIfLevelExpr(){
    //todo
}
void Parser::parseIfExpr(){
    //todo
}
void Parser::parseGetOrApplyLevelExpr(){
    //todo
}
void Parser::parseGetExpr(){
    //todo
}
void Parser::parseApplyExpr(){
    //todo
}
void Parser::parseEmptyApplyExpr(){
    //todo
}
void Parser::parseNonemptyApplyExpr(){
    //todo
}
void Parser::parseValueExpr(){
    //todo
}
void Parser::parseUnitExpr(){
    //todo
}
void Parser::parseTupleExpr(){
    //todo
}
void Parser::parseBlockExpr(){
    //todo
}
void Parser::parseBoolExpr(){
    //todo
}
void Parser::parseNegExpr(){
    //todo
}
void Parser::parseFloatingPointExpr(){
    //todo
}
void Parser::parseIntExpr(){
    //todo
}
void Parser::parseNotExpr(){
    //todo
}
void Parser::parseArrayMakeExpr(){
    //todo
}
void Parser::parseIdentifierExpr(){
    //todo
}
bool Parser::isBaseType(){
    auto v = CurTok.value;
    return v == "Unit" || v == "Bool" || v == "Int" || v == "Double";
}
void Parser::parseType(){
    if (isBaseType())
        std::cout << CurTok.value << '\n'; //目前只打印基本类型
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
        if (CurTok.value == ")"){ //tuple_type
            nextToken();
            if (CurTok.value == "->"){ //function_type
                nextToken();
                parseType();
                return;
            }
            else return;
        }
        else std::cout<<"Excepted ')' in tuple_type or function_type!"<<'\n';
    }
    else std::cout<<"Excepted '(' in tuple_type or function_type!"<<'\n';
    return;
}
void Parser::parseArrayType(){
    nextToken();
    if (CurTok.value == "["){
        nextToken();
        parseType();
        nextToken();
        if (CurTok.value == "]")
            return;
        else std::cout<<"Excepted ']' in ArrayType!"<<'\n';
    }
    else std::cout<<"Excepted '[' in ArrayType!"<<'\n';
    return;
}
