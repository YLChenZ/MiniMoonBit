#include "lexer.h"

class Parser{
    Lexer lex;
    std::string filename;

    std::ofstream fout;
    Token CurTok;

    
public:
    Parser(const std::string& fn);
    ~Parser();
    void nextToken();
    void parseProg();
    void parseTopLevel();
    void parseTopLetDecl();
    void parseTopLevelFnDecl();
    void parseMainFnDecl();
    void parseTopFnDecl();
    void parseParamList();
    void parseParam();
    void parseFnBody();
    void parseNontopFnDecl();
    void parseNontopParamList();
    void parseNontopParam();
    void parseStmt();
    void parseLetTupleStmt();
    void parseLetStmt();
    void parseTypeAnnotation();
    void parseFnDeclStmt();
    void parseAssignStmt();
    void parseExprStmt();
    void parseExpr();
    void parseAddSubLevelExpr();
    void parseMulDivLevelExpr();
    void parseIfLevelExpr();
    void parseIfExpr();
    void parseGetOrApplyLevelExpr();
    void parseGetExpr();
    void parseApplyExpr();
    void parseEmptyApplyExpr();
    void parseNonemptyApplyExpr();
    void parseValueExpr();
    void parseUnitExpr();
    void parseTupleExpr();
    void parseBlockExpr();
    void parseBoolExpr();
    void parseNegExpr();
    void parseFloatingPointExpr();
    void parseIntExpr();
    void parseNotExpr();
    void parseArrayMakeExpr();
    void parseIdentifierExpr();
    bool isBaseType();
    void parseType();
    void parseArrayType();

};