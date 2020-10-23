#ifndef COMPILER_GETTOKEN_H
#define COMPILER_GETTOKEN_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <map>
#include <stack>


enum SYMBOL{
    IDENFR = 1, INTCON, CHARCON, STRCON, CONSTTK, INTTK, CHARTK,
    VOIDTK, MAINTK, IFTK, ELSETK, SWITCHTK, CASETK, DEFAULTTK, WHILETK,
    FORTK, SCANFTK, PRINTFTK, RETURNTK, PLUS, MINU, MULT, DIV, LSS, LEQ,
    GRE, GEQ, EQL, NEQ, COLON, ASSIGN, SEMICN, COMMA, LPARENT, RPARENT,
    LBRACK, RBRACK, LBRACE, RBRACE
};

enum SIG_SYM{
    INT = 1, CHAR, FUNC_WITH_RETURN, FUNC_NO_RETURN, CONST_INT, CONST_CHAR,NONETYPE
};

class Word{
public:
    std::string _name;
    SYMBOL _symbol;
    int _line;
    Word(){
        _name = "";
        _line = 0;
    };
    Word(std::string name, SYMBOL symbol, int line){
        _name = name;
        _symbol = symbol;
        _line = line;
    }
    void getNewWord(std::string name, SYMBOL symbol, int line){
        _name = name;
        _symbol = symbol;
        _line = line;
    }
};

class Error{
public:
    std::string _error_type;
    int _line;
    std::string _info;

    Error(){
        _error_type = "";
        _line = 0;
        _info = "";
    };
    Error(std::string error_type, int line){
        _error_type = error_type;
        _line = line;
    }
    Error(std::string error_type, int line, std::string info){
        _error_type = error_type;
        _line = line;
        _info = info;
    }
};

class Signal{
public:
    std::string _name;
    SIG_SYM _type;
    int _line;

    //var
    int _dem;
    int _dem_num1;
    int _dem_num2;

    //func
    std::vector<SIG_SYM> _para_tab;
    SIG_SYM _returnType;

    Signal(){
        _name = "";
        _type = NONETYPE;
    };

    Signal(std::string name){
        _name = name;
        _type = NONETYPE;
    }

    Signal(std::string name, SIG_SYM type){
        _name = name;
        _type = type;
    }

    void func_addPara(SIG_SYM sigSym){
        _para_tab.push_back(sigSym);
    }
};


int transNum(std::string str);

extern std::string names[50];

extern std::vector<Word>words;

extern std::vector<Error>errors;

extern std::map<std::string, Signal>globalSigTab;
extern std::map<std::string, Signal>funcSigTab;

extern std::ofstream output;

#endif