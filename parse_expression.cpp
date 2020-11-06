//
// Created by h on 2020/11/6.
//

#include <string>
#include <vector>
#include <map>
#include <stack>
#include "myVar.h"
#include "parse.h"

SIG_SYM parse_expression(){
    SIG_SYM res = NONETYPE;
    if(symbol_p == PLUS || symbol_p == MINU){
        res = INT;
        get_next_token();
    }
    SIG_SYM temp = parse_item();
    if(res == NONETYPE){
        res = temp;
    }

    while(symbol_p == PLUS || symbol_p == MINU){
        res = INT;
        get_next_token();
        parse_item();
    }
    myPrint("<表达式>");
    return res;
}
SIG_SYM parse_item(){
    SIG_SYM res = parse_factor();
    while(symbol_p == MULT || symbol_p == DIV){
        res = INT;
        get_next_token();
        parse_factor();
    }
    myPrint("<项>");
    return res;
}
SIG_SYM parse_factor(){
    SIG_SYM res = NONETYPE;
    if(symbol_p == INTCON || ((symbol_p == PLUS || symbol_p == MINU) &&words[loc_f_p + 1]._symbol == INTCON)){//++1
        res = INT;
        parse_int();
    } else if(symbol_p == CHARCON){
        res = CHAR;
        parse_char();
    } else if(symbol_p == LPARENT){
        get_next_token();
        res = parse_expression();
        if(symbol_p != RPARENT) {
            addError("l", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
    } else if(symbol_p == IDENFR){
        if(globalSigTab.count(myTolower(name_p)) != 0){
            res = globalSigTab[myTolower(name_p)]._type;
        } else if(funcSigTab.count(myTolower(name_p)) != 0){
            res = funcSigTab[myTolower(name_p)]._type;
        } else {
            addError("c", line_p);
        }
        if(words[loc_f_p + 1]._symbol == LBRACK){
            get_next_token();
            get_next_token();
            SIG_SYM temp = parse_expression();
            if(temp !=INT && temp != CONST_INT){
                addError("i", line_p);
            }
            if(symbol_p != RBRACK) {
                addError("m", words[loc_f_p - 1]._line);
            } else {
                get_next_token();
            }
            if(symbol_p == LBRACK){
                get_next_token();
                temp =  parse_expression();
                if(temp != INT && temp != CONST_INT){
                    addError("i", line_p);
                }
                if(symbol_p != RBRACK) {
                    addError("m", words[loc_f_p - 1]._line);
                } else {
                    get_next_token();
                }
            }
        } else if(words[loc_f_p + 1]._symbol == LPARENT){
            res = parse_func_call_with_return();
        } else{
            get_next_token();
        }
    }
    myPrint("<因子>");
    return res;
}