#ifndef BY2_GETTOKEN_H
#define BY2_GETTOKEN_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

enum SYMBOL{
    IDENFR = 1, INTCON, CHARCON, STRCON, CONSTTK, INTTK, CHARTK,
    VOIDTK, MAINTK, IFTK, ELSETK, SWITCHTK, CASETK, DEFAULTTK, WHILETK,
    FORTK, SCANFTK, PRINTFTK, RETURNTK, PLUS, MINU, MULT, DIV, LSS, LEQ,
    GRE, GEQ, EQL, NEQ, COLON, ASSIGN, SEMICN, COMMA, LPARENT, RPARENT,
    LBRACK, RBRACK, LBRACE, RBRACE
};
extern std::string names[50];

class Word{
public:
    std::string _name;
    SYMBOL _symbol;
    Word(){};
    Word(std::string name, SYMBOL symbol){
        _name = name;
        _symbol = symbol;
    }
    void getNewWord(std::string name, SYMBOL symbol){
        _name = name;
        _symbol = symbol;
    }
};

extern std::vector<Word>words;

extern char* ch;
extern char myCh;
extern std::string token;

extern int num;

void clearToken();
void retract();

void catToken();

void getChar();

SYMBOL reserver();

int transNum();
void error();

bool isSpace();
bool isNewLine();
bool isTab();
bool isLetter();
bool isDigit();
bool isCharCon();
bool isStrCon();
bool isPlus();
bool isMinu();
bool isMult();
bool isDiv();
bool isLss();
bool isEql();
bool isGre();
bool isNeq();
bool isColon();
bool isSemicn();
bool isComma();
bool isLparent();
bool isRparent();
bool isLBRACK();
bool isRBRACK();
bool isLBRACE();
bool isRBRACE();

void system();
void initial();

#endif
