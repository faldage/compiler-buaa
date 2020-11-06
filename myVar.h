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
    INT = 1, CHAR, FUNC_WITH_RETURN, FUNC_NO_RETURN, CONST_INT, CONST_CHAR, NONETYPE
};
//在生成目标代码及之后不涉及con_int con_char


enum INTER_SYM{
    I_CON = 1, I_VAR, I_PRINTF, I_SCANF, I_ASSIGN
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

class IntermediateCode{
public:
    INTER_SYM _interSym;
    int _symProperty;// 值的含义与symbol有关
    //sym = I_printf: 1:str 2:expression 3:both

    //var && con
    SIG_SYM _vcType;
    int _intValue;
    char _chValue;
    std::string _name;

    //printf
    int _printfType;
    std::string _priStr;
    std::string _PriExpResReg;
    SIG_SYM _PriExpType;

    //scanf
    std::string _scName;
    SIG_SYM _scType;

    //assign
    std::string _assName;
    std::string _assType;
    std::string _AssExpResReg;

    IntermediateCode(){};
    IntermediateCode(INTER_SYM interSym){
        _interSym  = interSym;
    }
};


int transNum(std::string str);
std::string myTolower(std::string myStr);

extern std::string names[50];

extern std::vector<Word>words;

extern std::vector<Error>errors;

extern std::map<std::string, Signal>globalSigTab;
extern std::map<std::string, Signal>funcSigTab;

extern std::ofstream output_error;
extern std::vector<IntermediateCode> intermediateCodes;

//in parse
extern int loc_f_p;
extern SYMBOL symbol_p;
extern std::string name_p;
extern int line_p;
extern Signal newSig;
extern SIG_SYM prev_type;
extern bool isGlobal;
extern std::string func_name;//def
extern std::stack<std::string> call_func_name;
extern bool hasReturn;


extern IntermediateCode newIntermediateCode;
extern std::stack<IntermediateCode> ICodesStack;

#endif