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
    I_CON = 1, I_VAR, I_PRINTF, I_SCANF, I_ASSIGN, I_CAL
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
    //sym = I_printf: 1:str 2:expression 3:both: str
    //sym = i_cal: 1:add, 2:sub, 3:mul, 4:div

    //var && con
    SIG_SYM _vcType;
    int _intValue;
    char _chValue;
    std::string _name;

    //printf
    int _printfType;
    std::string _priStr;
    int _priExpResReg;//NOLINT
    SIG_SYM _priExpType;//NOLINT

    //scanf
    std::string _scName;
    SIG_SYM _scType;

    //assign
    std::string _assName;
    std::string _assType;
    int _assExpResReg;//NOLINT

    //cal
    int _resRegNum;
    int _cal1Type;//1:reg 2:iden 3:char 4:int
    int _cal2Type;//same with above
    int _reg1;
    std::string _iden1;
    char _ch1;
    int _int1;
    int _reg2;
    std::string _iden2;
    char _ch2;
    int _int2;

    IntermediateCode(){};//NOLINT
    IntermediateCode(INTER_SYM interSym){//NOLINT
        _interSym  = interSym;
    }
};

class VarLoc{
public:
    int loc_type;
    //1:reg 2:mem 3:gp 4:sp;

    int num;
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

//get inter code
extern IntermediateCode newIntermediateCode;
extern std::stack<IntermediateCode> ICodesStack;

extern int regNum;

extern int expRegNum;

//get MIPS
extern int strCount;
extern std::map<std::string, int>stringList;

extern std::map<std::string, VarLoc>locAssign;//变量名对应位置
extern std::map<int, int>regAssign;//寄存器已分配的情况
extern int gpCount;//gp分配位置

#endif