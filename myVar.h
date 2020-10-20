#ifndef COMPILER_GETTOKEN_H
#define COMPILER_GETTOKEN_H

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
    Error(){
        _error_type = "";
        _line = 0;
    };
    Error(std::string error_type, int line){
        _error_type = error_type;
        _line = line;
    }
};

class Signal{
public:
    std::string _name;
    std::string _type;
    int _layer;

    Signal(){
        _name = "";
        _type = "";
        _layer = 0;
    };
    Signal(std::string name, std::string type, int layer){
        _name = name;
        _type = type;
        _layer = layer;
    }
};

extern std::string names[50];

extern std::vector<Word>words;

extern std::vector<Error>errors;
extern std::vector<Signal>sig_tab;
#endif