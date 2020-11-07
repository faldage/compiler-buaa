#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include "parse.h"
#include "myVar.h"

void parse_program(){
    //t2<<"parse_program"<<std::endl;
    if(symbol_p == CONSTTK){
        parse_const_explain();
    }
    if((symbol_p == INTTK || symbol_p == CHARTK) && words[loc_f_p + 1]._symbol == IDENFR
       && words[loc_f_p + 2]._symbol != LPARENT){
        parse_var_explain();
    }
    clearFuncSigTab();
    clearSig();
    isGlobal = false;
    while(words[loc_f_p + 1]._symbol == IDENFR && words[loc_f_p + 2]._symbol == LPARENT){
        if(symbol_p == VOIDTK){
            parse_func_no_return_def();
        } else if(symbol_p == INTTK || symbol_p == CHARTK){
            parse_func_with_return_def();
        } else error_parse();
        clearFuncSigTab();
        clearSig();
    }
    parse_main();
    myPrint("<程序>");
}

void parse_compound_sent(){
    if(symbol_p == CONSTTK){
        parse_const_explain();
    }
    if(symbol_p == INTTK || symbol_p == CHARTK){
        parse_var_explain();
    }
    parse_sent_col();
    myPrint("<复合语句>");
}

void parse_main(){
    func_name = "main";

    if(symbol_p != VOIDTK)error_parse();
    get_next_token();
    if(symbol_p != MAINTK)error_parse();
    get_next_token();

    newSig._name = "main";
    newSig._type = FUNC_NO_RETURN;
    newSig._returnType = NONETYPE;
    newSig._line = line_p;
    addToTab();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();
    if(symbol_p != RPARENT) {
        addError("l",words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }

    if(symbol_p != LBRACE)error_parse();
    get_next_token();

    parse_compound_sent();

    if(symbol_p != RBRACE)error_parse();
    myPrint(names[words[loc_f_p]._symbol] + " " + words[loc_f_p]._name);
    myPrint("<主函数>");
}

void parse_sent(){
    if(symbol_p == WHILETK || symbol_p == FORTK)parse_loop_sent();
    else if(symbol_p == IFTK)parse_cond_sent();
    else if(symbol_p == IDENFR){
        if(words[loc_f_p + 1]._symbol == LPARENT){
            if(globalSigTab[words[loc_f_p]._name]._type == FUNC_NO_RETURN){
                parse_func_call_no_return();
            } else parse_func_call_with_return();
        }
        else parse_assign_sent();

        if(symbol_p != SEMICN) {
            addError("k", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
    }
    else if(symbol_p == SCANFTK){
        parse_read_sent();
        if(symbol_p != SEMICN) {
            addError("k", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
    }
    else if(symbol_p == PRINTFTK){
        parse_print_sent();
        if(symbol_p != SEMICN) {
            addError("k", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
    }
    else if(symbol_p == SWITCHTK)parse_case_sent();
    else if(symbol_p == SEMICN){
        get_next_token();
    }
    else if(symbol_p == RETURNTK){
        parse_return_sent();
        if(symbol_p != SEMICN) {
            addError("k", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
    }
    else if(symbol_p == LBRACE){
        get_next_token();
        parse_sent_col();
        if(symbol_p != RBRACE)error_parse();
        get_next_token();
    }
    else error_parse();
    myPrint("<语句>");
}
void parse_assign_sent(){
     newIntermediateCode._interSym = I_ASSIGN;

    if(globalSigTab.count(myTolower(name_p)) != 0 &&
        (globalSigTab[myTolower(name_p)]._type == CONST_INT
            || globalSigTab[myTolower(name_p)]._type == CONST_CHAR)){
        addError("j", line_p);
    } else if(funcSigTab.count(myTolower(name_p)) != 0 &&
              (funcSigTab[myTolower(name_p)]._type == CONST_INT
               || funcSigTab[myTolower(name_p)]._type == CONST_CHAR)){
        addError("j", line_p);
    }

    if(globalSigTab.count(myTolower(name_p)) != 0){
        if(globalSigTab[myTolower(name_p)]._type == INT){
            newIntermediateCode._assType = INT;
        } else {
            newIntermediateCode._assType = CHAR;
        }
    } else if(funcSigTab.count(myTolower(name_p)) != 0){
        if(funcSigTab[myTolower(name_p)]._type == INT){
            newIntermediateCode._assType = INT;
        } else {
            newIntermediateCode._assType = CHAR;
        }
    }

    newIntermediateCode._assName = name_p;
    parse_iden();
    if(symbol_p == ASSIGN){
        get_next_token();
        ICodesStack.push(newIntermediateCode);
        newIntermediateCode = IntermediateCode();//??????????????????
        parse_expression();
        newIntermediateCode = ICodesStack.top();
        newIntermediateCode._assExpResReg = expRegNum;
        ICodesStack.pop();
        addToICodes();
    } else if(symbol_p == LBRACK){
        get_next_token();
        SIG_SYM temp = parse_expression();
        if(temp != INT && temp != CONST_INT){
            addError("i", line_p);
        }
        if(symbol_p != RBRACK) {
            addError("m", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
        if(symbol_p == LBRACK){
            get_next_token();
            temp = parse_expression();
            if(temp != INT && temp != CONST_INT){
                addError("i", line_p);
            }
            if(symbol_p != RBRACK) {
                addError("m", words[loc_f_p - 1]._line);
            } else {
                get_next_token();
            }
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();
            parse_expression();
        } else if(symbol_p == ASSIGN){
            get_next_token();
            parse_expression();
        } else error_parse();
    } else error_parse();
    myPrint("<赋值语句>");
}
void parse_cond_sent(){
    //t2<<"parse_cond_sent"<<std::endl;
    if(symbol_p != IFTK)error_parse();
    get_next_token();
    if(symbol_p != LPARENT)error_parse();
    get_next_token();
    parse_condition();
    if(symbol_p != RPARENT) {
        addError("l", words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }

    parse_sent();
    if(symbol_p == ELSETK){
        get_next_token();
        parse_sent();
    }
    myPrint("<条件语句>");
}
void parse_condition(){
    //t2<<"parse_condition"<<std::endl;
    SIG_SYM temp1 = parse_expression();
    parse_relation();
    SIG_SYM temp2 = parse_expression();
    if((temp1 != INT && temp1 != CONST_INT)|| (temp2 != INT && temp2 != CONST_INT)){
        addError("f", line_p);
    }
    myPrint("<条件>");
}
void parse_loop_sent() {
    if (symbol_p == WHILETK) {
        get_next_token();
        if (symbol_p != LPARENT)error_parse();
        get_next_token();
        parse_condition();
        if(symbol_p != RPARENT) {
            addError("l", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
        parse_sent();
    } else if (symbol_p == FORTK) {
        get_next_token();
        if (symbol_p != LPARENT)error_parse();
        get_next_token();
        parse_iden();
        if (symbol_p != ASSIGN)error_parse();
        get_next_token();
        parse_expression();
        if(symbol_p != SEMICN) {
            addError("k", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
        parse_condition();
        if(symbol_p != SEMICN) {
            addError("k", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
        parse_iden();
        if (symbol_p != ASSIGN)error_parse();
        get_next_token();
        parse_iden();
        parse_plus();
        parse_step_length();
        if(symbol_p != RPARENT) {
            addError("l", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
        parse_sent();
    } else error_parse();
    myPrint("<循环语句>");
}
void parse_step_length(){
    parse_unsigned_int();
    myPrint("<步长>");
}
void parse_case_sent(){
    if(symbol_p != SWITCHTK)error_parse();
    get_next_token();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    SIG_SYM temp = parse_expression();

    if(symbol_p != RPARENT) {
        addError("l", words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }

    if(symbol_p != LBRACE)error_parse();
    get_next_token();

    parse_case_tab(temp);
    parse_default();

    if(symbol_p != RBRACE)error_parse();
    get_next_token();
    myPrint("<情况语句>");
}
void parse_case_tab(SIG_SYM sig_sym){
    if(symbol_p != CASETK)error_parse();
    parse_case_sub_sent(sig_sym);

    while(symbol_p == CASETK){
        parse_case_sub_sent(sig_sym);
    }
    myPrint("<情况表>");
}
void parse_case_sub_sent(SIG_SYM sig_sym){
    if(symbol_p != CASETK)error_parse();
    get_next_token();

    if(parse_constant()!= sig_sym){
        addError("o", line_p);
    }

    if(symbol_p != COLON)error_parse();
    get_next_token();

    parse_sent();
    myPrint("<情况子语句>");
}
void parse_default(){
    if(symbol_p != DEFAULTTK) {
        addError("p", line_p);
        return;
    }
    get_next_token();

    if(symbol_p != COLON)error_parse();
    get_next_token();

    parse_sent();
    myPrint("<缺省>");
}
void parse_sent_col(){
    while(symbol_p != RBRACE){
        parse_sent();
    }
    myPrint("<语句列>");
}
void parse_read_sent(){
    newIntermediateCode._interSym = I_SCANF;
    if(symbol_p != SCANFTK)error_parse();
    get_next_token();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    if(globalSigTab.count(myTolower(name_p)) != 0 &&
       (globalSigTab[myTolower(name_p)]._type == CONST_INT
        || globalSigTab[myTolower(name_p)]._type == CONST_CHAR)){
        addError("j", line_p);
    } else if(funcSigTab.count(myTolower(name_p)) != 0 &&
              (funcSigTab[myTolower(name_p)]._type == CONST_INT
               || funcSigTab[myTolower(name_p)]._type == CONST_CHAR)){
        addError("j", line_p);
    }
    newIntermediateCode._scName = name_p;
    if(globalSigTab.count(myTolower(name_p)) != 0){
        if(globalSigTab[myTolower(name_p)]._type == INT){
            newIntermediateCode._scType = INT;
        } else {
            newIntermediateCode._scType = CHAR;
        }
    } else if(funcSigTab.count(myTolower(name_p)) != 0){
        if(funcSigTab[myTolower(name_p)]._type == INT){
            newIntermediateCode._scType = INT;
        } else {
            newIntermediateCode._scType = CHAR;
        }
    }
    addToICodes();
    parse_iden();

    if(symbol_p != RPARENT) {
        addError("l", words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }
    myPrint("<读语句>");
}
void parse_print_sent(){
    if(symbol_p != PRINTFTK)error_parse();
    get_next_token();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    if(symbol_p == STRCON){
        newIntermediateCode._interSym = I_PRINTF;
        newIntermediateCode._symProperty = 1;
        newIntermediateCode._priStr = name_p;
        parse_string();
        addToICodes();
        if(symbol_p == COMMA){
            get_next_token();
            newIntermediateCode._priExpType = parse_expression();
            newIntermediateCode._interSym = I_PRINTF;
            newIntermediateCode._symProperty = 2;
            newIntermediateCode._priExpResReg = expRegNum;//NOLINT
            addToICodes();
        }
    }else{
        newIntermediateCode._priExpType = parse_expression();
        newIntermediateCode._interSym = I_PRINTF;
        newIntermediateCode._symProperty = 2;
        newIntermediateCode._priExpResReg = expRegNum;//NOLINT
        addToICodes();
    }

    if(symbol_p != RPARENT) {
        addError("l", words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }
    myPrint("<写语句>");
}
void parse_return_sent(){
    hasReturn = true;
    if(symbol_p != RETURNTK)error_parse();
    get_next_token();

    SIG_SYM returnRes;
    if(symbol_p == LPARENT){
        if(globalSigTab[myTolower(func_name)]._returnType == NONETYPE){
            addError("g", line_p);
        }
        get_next_token();
        returnRes = parse_expression();

        if(globalSigTab[myTolower(func_name)]._returnType != NONETYPE
            && returnRes != globalSigTab[myTolower(func_name)]._returnType){
            addError("h", line_p);
        }

        if(symbol_p != RPARENT) {
            addError("l", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
    } else {
        if(globalSigTab[myTolower(func_name)]._returnType != NONETYPE){
            addError("h", line_p);
        }
    }
    myPrint("<返回语句>");
}

void parse(){
    loc_f_p = 0;
    symbol_p = words[0]._symbol;
    parse_program();
}