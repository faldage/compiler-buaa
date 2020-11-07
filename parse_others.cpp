//
// Created by h on 2020/11/6.
//
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include "parse.h"
#include "myVar.h"

void addToICodes(){
    IntermediateCode temp = newIntermediateCode;
    intermediateCodes.push_back(temp);
}

bool isSameType1(SIG_SYM sig1, SIG_SYM sig2){
    if(sig1 == INT || sig1 == CONST_INT){
        if(sig2 == INT || sig2 == CONST_INT){
            return true;
        }
        return false;
    }
    if(sig1 == CHAR || sig1 == CONST_CHAR){
        if(sig2 == CHAR || sig2 == CONST_CHAR){
            return true;
        }
        return false;
    }
}


void clearSig(){
    newSig = Signal();
}

void addError(std::string error_type, int temp_line){
    errors.push_back(Error(error_type, temp_line));
}

void addToTab(){
    std::string temp =  myTolower(newSig._name);
    if (isGlobal) {
        if(globalSigTab.count(temp) == 0) {
            globalSigTab[temp] = newSig;
        } else {
            addError("b", newSig._line);
        }
    } else if(newSig._type == FUNC_NO_RETURN || newSig._type == FUNC_WITH_RETURN) {
        if(globalSigTab.count(temp) == 0) {
            globalSigTab[temp] = newSig;
        } else {
            addError("b", newSig._line);
        }
        funcSigTab[temp] = newSig;
    } else {
        if(funcSigTab.count(temp) == 0) {
            funcSigTab[temp] = newSig;
        } else {
            addError("b", newSig._line);
        }
    }
    clearSig();
}

void clearFuncSigTab(){
    funcSigTab.clear();
}

bool isDefined(std::string str_name){
    if(globalSigTab.count(myTolower(str_name)) == 0 && funcSigTab.count(myTolower(str_name)) == 0){
        return false;
    }
    return true;
}

void myPrint(std::string str){
    if(0) {
        output_error << str << std::endl;
    }
}

void error_parse(){
    myPrint("error in parse!");
}
void get_next_token(){
    loc_f_p++;
    symbol_p = words[loc_f_p]._symbol;
    name_p = words[loc_f_p]._name;
    line_p = words[loc_f_p]._line;
    myPrint(names[words[loc_f_p - 1]._symbol] + " "+ words[loc_f_p - 1]._name);
}

void parse_iden(){
    if(!isDefined(name_p)){
        addError("c", line_p);
    }
    if(symbol_p != IDENFR)error_parse();
    get_next_token();
}

void parse_new_iden(){
    if(symbol_p != IDENFR)error_parse();
    newSig._name = name_p;
    get_next_token();
}

void parse_plus(){
    if(symbol_p != PLUS && symbol_p != MINU)error_parse();
    get_next_token();
}
void parse_mult(){
    if(symbol_p != MULT && symbol_p != DIV)error_parse();
    get_next_token();
}
void parse_relation(){
    if(symbol_p == LSS || symbol_p == LEQ || symbol_p == GRE || symbol_p == GEQ
       || symbol_p == EQL || symbol_p == NEQ){
        get_next_token();
        return;
    }
    error_parse();
}
char parse_char(){
    char ch = name_p[0];
    if(symbol_p != CHARCON)error_parse();
    get_next_token();
    return ch;
}
void parse_string() {
    if (symbol_p != STRCON)error_parse();
    stringList.push_back(name_p);
    get_next_token();
    myPrint("<字符串>");
}

int parse_unsigned_int(){
    if(symbol_p != INTCON)error_parse();
    int res = transNum(name_p);
    get_next_token();
    myPrint("<无符号整数>");
    return res;
}
int parse_int(){
    int res;
    if(symbol_p == PLUS || symbol_p == MINU){
        parse_plus();
    }
    if(symbol_p == MINU){
        res = -1;
    } else {
        res = 1;
    }

    res *= parse_unsigned_int();
    myPrint("<整数>");
    return res;
}


SIG_SYM parse_constant(){
    //t2<<"parse_constant"<<std::endl;
    SIG_SYM res = NONETYPE;
    if(symbol_p == CHARCON){
        res = CHAR;
        newIntermediateCode._chValue = parse_char();
    } else if(symbol_p == INTCON || ((symbol_p == PLUS || symbol_p == MINU) && (words[loc_f_p + 1]._symbol == INTCON))){
        newIntermediateCode._intValue = parse_int();
        res = INT;
    }else error_parse();
    myPrint("<常量>");
    return res;
}

void parse_type_iden(){
    if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
    get_next_token();
}
