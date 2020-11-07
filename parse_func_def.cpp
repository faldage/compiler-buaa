//
// Created by h on 2020/11/6.
//

#include <string>
#include <vector>
#include <map>
#include <stack>
#include "myVar.h"
#include "parse.h"
void parse_para_tab(){
    if(symbol_p != RPARENT) {
        if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
        if(symbol_p == INTTK)newSig._type = INT;
        else newSig._type = CHAR;

        newSig._line = line_p;

        globalSigTab[myTolower(func_name)].func_addPara(newSig._type);

        get_next_token();

        parse_new_iden();
        addToTab();


        while(symbol_p == COMMA){
            get_next_token();
            if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
            if(symbol_p == INTTK)newSig._type = INT;
            else newSig._type = CHAR;

            newSig._line = line_p;

            globalSigTab[myTolower(func_name)].func_addPara(newSig._type);

            get_next_token();

            parse_new_iden();
            addToTab();
        }
    }
    myPrint("<参数表>");
}
void parse_statement_head(){
    if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
    SYMBOL temp_type = symbol_p;

    get_next_token();

    func_name = words[loc_f_p]._name;

    parse_new_iden();
    myPrint("<声明头部>");
}

void parse_func_with_return_def(){
    newSig._type = FUNC_WITH_RETURN;
    newSig._line = line_p;
    if(symbol_p == INTTK){
        newSig._returnType = INT;
    } else if(symbol_p == CHARTK) {
        newSig._returnType = CHAR;
    }

    parse_statement_head();
    addToTab();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_para_tab();

    if(symbol_p != RPARENT) {
        addError("l", words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }

    if(symbol_p != LBRACE)error_parse();
    get_next_token();

    parse_compound_sent();

    if(symbol_p != RBRACE)error_parse();
    if(!hasReturn){
        std::cout<<func_name<<"-return wrong 1"<<std::endl;
        addError("h", line_p);
    } else {
        hasReturn = false;
    }
    get_next_token();
    myPrint("<有返回值函数定义>");
}
void parse_func_no_return_def(){
    if(symbol_p != VOIDTK)error_parse();
    get_next_token();

    newSig._type = FUNC_NO_RETURN;
    newSig._returnType = NONETYPE;
    newSig._line = line_p;

    func_name = words[loc_f_p]._name;

    parse_new_iden();
    addToTab();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_para_tab();

    if(symbol_p != RPARENT) {
        addError("l", words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }

    if(symbol_p != LBRACE)error_parse();
    get_next_token();

    parse_compound_sent();

    if(symbol_p != RBRACE)error_parse();
    get_next_token();
    clearFuncSigTab();
    myPrint("<无返回值函数定义>");
}
