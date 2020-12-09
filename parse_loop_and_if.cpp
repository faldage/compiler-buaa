//
// Created by h on 2020/12/5.
//
#include <vector>
#include <map>
#include <stack>
#include "myVar.h"
#include "parse.h"

void parse_cond_sent(){
    newIntermediateCode._interSym = I_IF;
    newIntermediateCode._endLabel = labelCount;
    labelCount++;
    ICodesStack.push(newIntermediateCode);

    //t2<<"parse_cond_sent"<<std::endl;
    if(symbol_p != IFTK)error_parse();
    get_next_token();
    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_condition();

    lastICNum.push(intermediateCodes.size());
    addToICodes();

    if(symbol_p != RPARENT) {
        addError("l", words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }

    parse_sent();

    if(symbol_p == ELSETK){

        int tempNum = lastICNum.top();
        intermediateCodes[tempNum]._elseLabel = labelCount;

        intermediateCodes[tempNum]._symProperty = 1;

        newIntermediateCode._interSym = I_J;
        newIntermediateCode._jLabelNum = intermediateCodes[tempNum]._endLabel;
        addToICodes();

        newIntermediateCode._interSym = I_LABEL;
        newIntermediateCode._labelName = std::to_string(labelCount);
        addToICodes();

        labelCount++;

        get_next_token();
        parse_sent();
    } else {
        int tempNum = lastICNum.top();
        intermediateCodes[tempNum]._symProperty = 0;
    }
    int tempNum = lastICNum.top();
    lastICNum.pop();

    newIntermediateCode._interSym = I_LABEL;
    newIntermediateCode._labelName = std::to_string(intermediateCodes[tempNum]._endLabel);
    addToICodes();

    myPrint("<条件语句>");
}
void parse_condition(){
    //t2<<"parse_condition"<<std::endl;
    SIG_SYM temp1 = parse_expression();
    int exp1 = expRegNum;

    int relation;
    //1:>= 2:> 3:<= 4:< 5:== 6:!=
    switch(symbol_p){
        case LSS:
            relation = 4;//<
            break;
        case LEQ:
            relation = 3; //<=
            break;
        case GRE:
            relation = 2;//>
            break;
        case GEQ:
            relation = 1;//>=
            break;
        case EQL:
            relation = 5;//==
            break;
        case NEQ:
            relation = 6;//!=
            break;
        default:
            std::cout<<name_p<<"___error in relation type!!! = "<<symbol_p<<std::endl;
    }
    parse_relation();
    SIG_SYM temp2 = parse_expression();
    newIntermediateCode = ICodesStack.top();
    ICodesStack.pop();
    newIntermediateCode._judgeType = relation;
    newIntermediateCode._judgeReg1 = exp1;
    newIntermediateCode._judgeReg2 = expRegNum;

    if((temp1 != INT && temp1 != CONST_INT)|| (temp2 != INT && temp2 != CONST_INT)){
        addError("f", line_p);
    }
    myPrint("<条件>");
}
void parse_loop_sent() {
    if (symbol_p == WHILETK) {

        newIntermediateCode._interSym = I_WHILE;
        newIntermediateCode._labelStart = labelCount;
        ICodesStack.push(newIntermediateCode);
        labelCount++;

        newIntermediateCode._interSym = I_LABEL;
        newIntermediateCode._labelName = std::to_string(labelCount-1);
        addToICodes();

        get_next_token();
        if (symbol_p != LPARENT)error_parse();
        get_next_token();
        parse_condition();
        if(symbol_p != RPARENT) {
            addError("l", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }

        lastICNum.push(intermediateCodes.size());
        addToICodes();

        parse_sent();

        int tempNum = lastICNum.top();
        lastICNum.pop();
        intermediateCodes[tempNum]._labelEnd = labelCount;
        labelCount++;

        newIntermediateCode._interSym = I_J;
        newIntermediateCode._jLabelNum = intermediateCodes[tempNum]._labelStart;
        addToICodes();

        newIntermediateCode._interSym = I_LABEL;
        newIntermediateCode._labelName = std::to_string(intermediateCodes[tempNum]._labelEnd);
        addToICodes();
    } else if (symbol_p == FORTK) {
        get_next_token();
        if (symbol_p != LPARENT)error_parse();
        get_next_token();

        //i = 1(example)
        //newIntermediateCode._interSym = I_FOR;
        newIntermediateCode._loopName1 = name_p;
        ICodesStack.push(newIntermediateCode);

        parse_iden();
        if (symbol_p != ASSIGN)error_parse();
        get_next_token();
        parse_expression();

        newIntermediateCode = ICodesStack.top();
        ICodesStack.pop();
        newIntermediateCode._interSym = I_ASSIGN;
        newIntermediateCode._assName = newIntermediateCode._loopName1;
        newIntermediateCode._assExpResReg = expRegNum;
        addToICodes();

        //newIntermediateCode._initialReg = expRegNum;
        newIntermediateCode._interSym = I_FOR;
        newIntermediateCode._labelStart = labelCount;
        labelCount++;
        ICodesStack.push(newIntermediateCode);

        newIntermediateCode._interSym = I_LABEL;
        newIntermediateCode._labelName = std::to_string(labelCount - 1);
        addToICodes();

        if(symbol_p != SEMICN) {
            addError("k", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
        //i <= 10 (example)
        parse_condition();
        if(symbol_p != SEMICN) {
            addError("k", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }

        //i = i + 1
        //sym = i_for: 1:++ 2:--
        newIntermediateCode._loopName2 = name_p;
        parse_iden();

        if (symbol_p != ASSIGN)error_parse();
        get_next_token();

        newIntermediateCode._loopName3 = name_p;
        parse_iden();

        if(symbol_p == PLUS){
            newIntermediateCode._symProperty = 1;
        } else { //MINU
            newIntermediateCode._symProperty = 2;
        }
        parse_plus();

        newIntermediateCode._step = parse_step_length();
        lastICNum.push(intermediateCodes.size());
        addToICodes();

        if(symbol_p != RPARENT) {
            addError("l", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }

        parse_sent();

        int tempNum = lastICNum.top();
        lastICNum.pop();
        intermediateCodes[tempNum]._labelEnd = labelCount;
        labelCount++;

        newIntermediateCode._interSym = I_CAL;
        newIntermediateCode._resType = 1;
        if(intermediateCodes[tempNum]._symProperty == 1){
            newIntermediateCode._symProperty = 1;
        } else if(intermediateCodes[tempNum]._symProperty == 2){
            newIntermediateCode._symProperty = 2;
        } else {
            std::cout<<"wrong +/- in for!"<<std::endl;
        }

        int tempResReg = regNum;
        regNum++;
        funcAddSpaceForSp();

        newIntermediateCode._cal1Type = 2;
        newIntermediateCode._iden1 = intermediateCodes[tempNum]._loopName3;
        newIntermediateCode._cal2Type = 4;
        newIntermediateCode._int2 = intermediateCodes[tempNum]._step;
        newIntermediateCode._resType = 1;
        newIntermediateCode._resRegNum = tempResReg;
        addToICodes();

        newIntermediateCode._interSym = I_ASSIGN;
        newIntermediateCode._assName = intermediateCodes[tempNum]._loopName2;
        newIntermediateCode._assExpResReg = tempResReg;
        addToICodes();

        newIntermediateCode._interSym = I_J;
        newIntermediateCode._jLabelNum = intermediateCodes[tempNum]._labelStart;
        addToICodes();

        newIntermediateCode._interSym = I_LABEL;
        newIntermediateCode._labelName = std::to_string(intermediateCodes[tempNum]._labelEnd);
        addToICodes();
    } else error_parse();
    myPrint("<循环语句>");
}
int parse_step_length(){
    int temp = parse_unsigned_int();
    myPrint("<步长>");
    return temp;
}
