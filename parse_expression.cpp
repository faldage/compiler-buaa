//
// Created by h on 2020/11/6.
//

#include <string>
#include <vector>
#include <map>
#include <stack>
#include "myVar.h"
#include "parse.h"
#include "parse_expression.h"

int itemReg;
int factorReg;

SIG_SYM parse_expression(){
    SIG_SYM res = NONETYPE;
    int thisExpRegNum = regNum;
    regNum++;
    funcAddSpaceForSp();

    int tempCal = 1;
    if(symbol_p == PLUS || symbol_p == MINU){
        if(symbol_p == PLUS){
            tempCal = 1;
        } else {
            tempCal = 2;
        }
        res = INT;
        get_next_token();
    }
    SIG_SYM temp = parse_item();

    newIntermediateCode._interSym = I_CAL;
    newIntermediateCode._symProperty = tempCal;
    newIntermediateCode._resType = 1;
    newIntermediateCode._resRegNum = thisExpRegNum;
    newIntermediateCode._cal1Type = 4;
    newIntermediateCode._int1 = 0;
    newIntermediateCode._cal2Type = 1;
    newIntermediateCode._reg2 = itemReg;
    addToICodes();


    if(res == NONETYPE){
        res = temp;
    }

    while(symbol_p == PLUS || symbol_p == MINU){
        if(symbol_p == PLUS){
            tempCal = 1;
        } else {
            tempCal = 2;
        }
        res = INT;
        get_next_token();
        parse_item();

        newIntermediateCode = IntermediateCode(I_CAL);
        newIntermediateCode._symProperty = tempCal;
        newIntermediateCode._resRegNum = thisExpRegNum;
        newIntermediateCode._resType = 1;
        newIntermediateCode._cal1Type = 1;
        newIntermediateCode._reg1 = thisExpRegNum;
        newIntermediateCode._cal2Type = 1;
        newIntermediateCode._reg2 = itemReg;
        addToICodes();
    }
    myPrint("<表达式>");
    expRegNum = thisExpRegNum;
    return res;
}
SIG_SYM parse_item(){
    int tempCal;
    int thisItemRegNum = regNum;
    regNum++;
    funcAddSpaceForSp();

    SIG_SYM res = parse_factor();
    newIntermediateCode = IntermediateCode(I_CAL);
    newIntermediateCode._symProperty = 1;//add
    newIntermediateCode._resType = 1;
    newIntermediateCode._resRegNum = thisItemRegNum;
    newIntermediateCode._cal1Type = 4;
    newIntermediateCode._int1 = 0;
    newIntermediateCode._cal2Type = 1;
    newIntermediateCode._reg2 = factorReg;
    addToICodes();

    while(symbol_p == MULT || symbol_p == DIV){
        if(symbol_p == MULT){
            tempCal  = 3;
        } else {
            tempCal = 4;
        }
        res = INT;
        get_next_token();
        parse_factor();

        newIntermediateCode = IntermediateCode(I_CAL);
        newIntermediateCode._symProperty = tempCal;
        newIntermediateCode._resType = 1;
        newIntermediateCode._resRegNum = thisItemRegNum;
        newIntermediateCode._cal1Type = 1;
        newIntermediateCode._reg1 = thisItemRegNum;
        newIntermediateCode._cal2Type = 1;
        newIntermediateCode._reg2 = factorReg;
        addToICodes();
    }
    myPrint("<项>");
    itemReg = thisItemRegNum;
    return res;
}
SIG_SYM parse_factor(){
    int thisFactorRegNum = regNum;
    regNum++;
    funcAddSpaceForSp();

    int isArray = 0;

    SIG_SYM res = NONETYPE;
    if(symbol_p == INTCON || ((symbol_p == PLUS || symbol_p == MINU) &&words[loc_f_p + 1]._symbol == INTCON)){//++1
        res = INT;
        newIntermediateCode._cal2Type = 4;
        newIntermediateCode._int2 = parse_int();
    } else if(symbol_p == CHARCON){
        res = CHAR;
        newIntermediateCode._cal2Type = 3;
        newIntermediateCode._ch2= parse_char();
    } else if(symbol_p == LPARENT){
        get_next_token();
        res = parse_expression();
        if(symbol_p != RPARENT) {
            addError("l", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
        newIntermediateCode._cal2Type = 1;
        newIntermediateCode._reg2 = expRegNum;
    } else if(symbol_p == IDENFR){
        newIntermediateCode._name = name_p;
        newIntermediateCode._funcCallName = name_p;


        SIG_SYM temp;
        if(globalSigTab.count(myTolower(name_p)) != 0){
            temp = globalSigTab[myTolower(name_p)]._type;
        } else if(funcSigTab.count(myTolower(name_p)) != 0){
            temp = funcSigTab[myTolower(name_p)]._type;
        } else {
            addError("c", line_p);
        }
        if(temp == INT || temp == CONST_INT){
            res = INT;
        } else if (temp == CHAR || temp == CONST_CHAR){
            res = CHAR;
        }

        newIntermediateCode._vcType = res;
        ICodesStack.push(newIntermediateCode);
        if(words[loc_f_p + 1]._symbol == LBRACK){
            isArray = 1;
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
                newIntermediateCode = ICodesStack.top();
                ICodesStack.pop();
                newIntermediateCode._interSym = I_ARR_GET;
                newIntermediateCode._symProperty = 2;
                newIntermediateCode._loc1 = expRegNum;

                if(funcSigTabMap[myTolower(func_name)].count(myTolower(newIntermediateCode._name)) != 0) {
                    newIntermediateCode._length = funcSigTabMap[myTolower(func_name)][myTolower(newIntermediateCode._name)]._dem_num2;
                } else if(globalSigTab.count(myTolower(newIntermediateCode._name)) != 0){
                    newIntermediateCode._length = globalSigTab[myTolower(newIntermediateCode._name)]._dem_num2;
                } else {
                    std::cout<<"wrong when find length! in get"<<std::endl;
                }

                ICodesStack.push(newIntermediateCode);

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

                newIntermediateCode = ICodesStack.top();
                ICodesStack.pop();
                newIntermediateCode._loc2 = expRegNum;
                newIntermediateCode._ansNum = thisFactorRegNum;
                addToICodes();
            }else{
                newIntermediateCode = ICodesStack.top();
                ICodesStack.pop();
                newIntermediateCode._symProperty = 1;
                newIntermediateCode._interSym = I_ARR_GET;
                newIntermediateCode._loc1 = expRegNum;
                newIntermediateCode._ansNum = thisFactorRegNum;
                addToICodes();
            }
        } else if(words[loc_f_p + 1]._symbol == LPARENT){
            res = parse_func_call_with_return();

        } else{
            newIntermediateCode = ICodesStack.top();
            ICodesStack.pop();
            newIntermediateCode._cal2Type = 2;
            newIntermediateCode._iden2 = name_p;
            get_next_token();
        }
    }
    if(isArray == 0){
        newIntermediateCode._interSym = I_CAL;
        newIntermediateCode._symProperty = 1;
        newIntermediateCode._resType = 1;
        newIntermediateCode._resRegNum = thisFactorRegNum;
        newIntermediateCode._cal1Type = 4;
        newIntermediateCode._int1 = 0;
        addToICodes();
    }
    factorReg = thisFactorRegNum;
    myPrint("<因子>");
    return res;
}