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
    I_CON = 1, I_VAR, I_PRINTF, I_SCANF, I_ASSIGN, I_CAL, I_WHILE, I_FOR, I_IF,
    I_J, I_JR_RA, I_FUNC_DEF, I_FUNC_DEF_END, I_FUNC_CALL, I_FUNC_CALL_END,
    I_LABEL, I_SWITCH, I_SWITCH_IF, I_ARR_GET, I_ARR_ASS, I_CODE
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
    std::vector<std::string>_para_tab_name;
    SIG_SYM _returnType;
    int _spaceCount;//for $sp

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

    void func_addPara(SIG_SYM sigSym, std::string func_add_name){
        _para_tab.push_back(sigSym);
        _para_tab_name.push_back(func_add_name);

    }
};

class IntermediateCode{
public:
    INTER_SYM _interSym;
    int _symProperty;// 值的含义与symbol有关
    //sym = I_printf: 1:str 2:expression 3:both: str
    //sym = i_cal: 1:add, 2:sub, 3:mul, 4:div
    //sym = i_while:
    //sym = i_for: 1:++ 2:--
    //sym = i_var: 0:dem 1:dem2 3:dem2
    //sym = i_func: 0: no_return 1:return int 2:return char
    //sym = i_func_call: 0:no_return 1:return int 2:return char
    //sym = i_if: 0:no else 1:else
    //sym = i_arr_get: 1:dem1 2:dem2
    //sym = i_arr_ass: 1:dem1 2:dem2

    //var && con
    int _initialized;
    SIG_SYM _vcType;
    std::vector<int> _intValue;
    std::vector<char> _chValue;
    std::string _name;
    int _dem1;
    int _dem2;

    //arr_get
    //用上面的名字 & vcType
    int _loc1;//a[!]
    int _loc2;//a[][!]
    int _length;//a[][2]
    int _ansNum;

    //arr_ass
    //上面的_loc1 _loc2 _length assType
    int _arr_ass_regNum;


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
    SIG_SYM _assType;
    int _assExpResReg;//NOLINT

    //cal
    int _resRegNum;
    int _resReg2;
    int _resType;//1:temp 2:固定寄存器
    int _cal1Type;//1:reg 2:iden 3:char 4:int 5:固定寄存器
    int _cal2Type;//same with above
    int _reg1;
    std::string _iden1;
    char _ch1;
    int _int1;
    int _sReg1;
    int _reg2;
    std::string _iden2;
    char _ch2;
    int _int2;
    int _sReg2;

    //while & if
    int _judgeType;//1:>= 2:> 3:<= 4:< 5:== 6:!=
    int _judgeReg1;
    int _judgeReg2;
    int _labelStart;//if: no use
    int _labelEnd;//if: no use

    //if
    int _endLabel;
    int _elseLabel;

    //for
    std::string _loopName1;
    std::string _loopName2;
    std::string _loopName3;
    int _initialReg;
    int _step;//需要栈
    //条件使用上面的

    //switch
    int _switchReg;
    SIG_SYM _swType;
    int _switchEndLabel;

    //switch_if 用上面的（（
    //常量在_intValue and _chValue
    int _nextIfLabel;

    //func_call
    std::string _funcCallName;
    int _returnReg;//$v0 = $2
    std::vector<int>_paraRegNum;
    std::vector<SIG_SYM>_paraType;

    //func_call_end

    //func_def
    std::string _funcDefName;

    //func_def_end

    //j
    int _jLabelNum;

    //label
    std::string _labelName;

    //code
    std::string _code;

    IntermediateCode(){};//NOLINT
    IntermediateCode(INTER_SYM interSym){//NOLINT
        _interSym  = interSym;
    }
};

int transNum(std::string str);
std::string myTolower(std::string myStr);
void finishCountSpace();
void funcAddSpaceForSp();
void funcSubSpaceForSp();
void funcInitialSpaceForSp();

extern std::string names[50];

extern std::vector<Word>words;

extern std::vector<Error>errors;

extern std::map<std::string, Signal>globalSigTab;
extern std::map<std::string, Signal>funcSigTab;
extern std::map<std::string, std::map<std::string, Signal>>funcSigTabMap;

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

extern int labelCount;
extern std::stack<int> endLabelStack;
extern std::stack<int> labelStack;

extern std::stack<int>lastICNum;

extern int spCount;


#endif