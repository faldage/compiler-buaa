//
// Created by h on 2020/12/5.
//
#include <vector>
#include <map>
#include <stack>
#include "myVar.h"
#include "parse.h"


void parse_case_sent(){
    if(symbol_p != SWITCHTK)error_parse();
    get_next_token();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    SIG_SYM temp = parse_expression();

    newIntermediateCode._interSym = I_SWITCH;

    Value tempValue;
    if(ifExpIsCon == 1){
        ifExpIsCon = 0;
        tempValue = Value(2, expValue);
    } else{
        tempValue = Value(1, expRegNum);
    }
    newIntermediateCode._switchValue = tempValue;

    newIntermediateCode._switchEndLabel = labelCount;
    newIntermediateCode._swType = temp;
    endLabelStack.push(labelCount);
    labelCount++;
    addToICodes();

    if(symbol_p != RPARENT) {
        addError("l", words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }

    if(symbol_p != LBRACE)error_parse();
    get_next_token();

    parse_case_tab(temp, tempValue);
    parse_default();

    newIntermediateCode._interSym = I_LABEL;
    newIntermediateCode._labelName = std::to_string(endLabelStack.top());
    endLabelStack.pop();
    addToICodes();

    if(symbol_p != RBRACE)error_parse();
    get_next_token();
    myPrint("<情况语句>");
}

void parse_case_tab(SIG_SYM sig_sym, Value lastExpValue){
    newIntermediateCode._interSym = I_SWITCH_IF;
    newIntermediateCode._nextIfLabel = labelCount;
    newIntermediateCode._swType = sig_sym;
    newIntermediateCode._switchValue = lastExpValue;

    labelStack.push(labelCount);
    labelCount++;

    if(symbol_p != CASETK)error_parse();
    parse_case_sub_sent(sig_sym);//里面有一个addToICodes

    newIntermediateCode._interSym = I_J;
    newIntermediateCode._jLabelNum = endLabelStack.top();
    addToICodes();

    while(symbol_p == CASETK){
        newIntermediateCode._interSym = I_LABEL;
        newIntermediateCode._labelName = std::to_string(labelStack.top());
        labelStack.pop();
        addToICodes();

        newIntermediateCode._interSym = I_SWITCH_IF;
        newIntermediateCode._nextIfLabel = labelCount;
        newIntermediateCode._swType = sig_sym;
        newIntermediateCode._switchValue = lastExpValue;
        labelStack.push(labelCount);
        labelCount++;

        parse_case_sub_sent(sig_sym);

        newIntermediateCode._interSym = I_J;
        newIntermediateCode._jLabelNum = endLabelStack.top();
        addToICodes();
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

    addToICodes();

    parse_sent();
    myPrint("<情况子语句>");
}
void parse_default(){
    newIntermediateCode._interSym = I_LABEL;
    newIntermediateCode._labelName = std::to_string(labelStack.top());
    labelStack.pop();
    addToICodes();

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