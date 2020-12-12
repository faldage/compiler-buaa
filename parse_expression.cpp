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
int ifItemIsCon = 0;
int itemValue = 0;
int ifFactorIsCon = 0;
int factorValue = 0;
std::vector<Temp> Items;

std::stack<std::vector<Temp>>ItemsStack;

SIG_SYM parse_expression(){
    ifExpIsCon = 0;
    expValue = 0;
    SIG_SYM res = NONETYPE;
/*
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
*/
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
    int thisExpRegNum, thisExpValue = 0;
    if(ifItemIsCon == 1){
        ifItemIsCon = 0;
        if(tempCal == 1){
            thisExpValue += itemValue;
        } else {
            thisExpValue -= itemValue;
        }
    } else {
        Items.push_back(Temp(tempCal, 1, itemReg));
    }

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

        ItemsStack.push(Items);
        Items.clear();
        parse_item();
        Items = ItemsStack.top();
        ItemsStack.pop();

        if(ifItemIsCon == 1){
            ifItemIsCon = 0;
            if(tempCal == 1){
                thisExpValue += itemValue;
            } else {
                thisExpValue -= itemValue;
            }
        } else {
            Items.push_back(Temp(tempCal, 1, itemReg));
        }
    }

    if(Items.size() == 0){
        ifExpIsCon = 1;
        expValue = thisExpValue;
    } else {
        expRegNum = Items[0]._regNum;
        //expRegNum = regNum;
        //regNum++;
        //funcAddSpaceForSp();
        if(thisExpValue != 0 || Items[0]._cal != 1){
            newIntermediateCode = IntermediateCode(I_CAL);
            newIntermediateCode._symProperty = Items[0]._cal;
            newIntermediateCode._resRegNum = expRegNum;
            newIntermediateCode._resType = 1;
            newIntermediateCode._cal2Type = 1;
            newIntermediateCode._reg2 = Items[0]._regNum;
            newIntermediateCode._cal1Type = 4;
            newIntermediateCode._int1 = thisExpValue;
            addToICodes();
        }

        for(int i = 1; i < Items.size(); i++){
            newIntermediateCode = IntermediateCode(I_CAL);
            newIntermediateCode._symProperty = Items[i]._cal;
            newIntermediateCode._resRegNum = expRegNum;
            newIntermediateCode._resType = 1;
            newIntermediateCode._cal1Type = 1;
            newIntermediateCode._reg1 = expRegNum;
            newIntermediateCode._cal2Type = 1;
            newIntermediateCode._reg2 = Items[i]._regNum;
            addToICodes();
        }
    }

    myPrint("<表达式>");
    Items.clear();
    return res;
}
SIG_SYM parse_item(){
    ifItemIsCon = 0;
    itemValue = 0;
    int tempCal;
    /*
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
    */
    SIG_SYM res = parse_factor();
    int thisItemRegNum;

    if(ifFactorIsCon == 1){
        ifFactorIsCon = 0;
        itemValue = factorValue;
        ifItemIsCon = 1;

        thisItemRegNum = regNum;
        regNum++;
        funcAddSpaceForSp();

        newIntermediateCode = IntermediateCode(I_CAL);
        newIntermediateCode._symProperty = 1;//add
        newIntermediateCode._resType = 1;
        newIntermediateCode._resRegNum = thisItemRegNum;
        newIntermediateCode._cal1Type = 4;
        newIntermediateCode._int1 = 0;
        newIntermediateCode._cal2Type = 4;
        newIntermediateCode._int2 = factorValue;
        addToICodes();

    }else{
        thisItemRegNum = factorReg;
    }

    while(symbol_p == MULT || symbol_p == DIV){
        ifItemIsCon = 0;
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
        if(ifFactorIsCon == 1){
            ifFactorIsCon = 0;
            newIntermediateCode._cal2Type = 4;
            newIntermediateCode._int2 = factorValue;
        } else {
            newIntermediateCode._cal2Type = 1;
            newIntermediateCode._reg2 = factorReg;
        }
        addToICodes();
    }
    if(ifItemIsCon == 1){
        regNum--;
        funcSubSpaceForSp();
        intermediateCodes.pop_back();
    }
    myPrint("<项>");
    itemReg = thisItemRegNum;
    return res;
}
SIG_SYM parse_factor(){
    int thisFactorRegNum = regNum;
    ifFactorIsCon = 0;
    factorValue = 0;
    regNum++;
    funcAddSpaceForSp();

    SIG_SYM res = NONETYPE;
    if(symbol_p == INTCON || ((symbol_p == PLUS || symbol_p == MINU) &&words[loc_f_p + 1]._symbol == INTCON)){//++1
        res = INT;

        ifFactorIsCon = 1;
        factorValue = parse_int();

        regNum--;
        funcSubSpaceForSp();

    } else if(symbol_p == CHARCON){
        res = CHAR;

        ifFactorIsCon = 1;
        factorValue = int(parse_char());

        regNum--;
        funcSubSpaceForSp();
    } else if(symbol_p == LPARENT){
        get_next_token();
        res = INT;
        parse_expression();
        if(symbol_p != RPARENT) {
            addError("l", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }

        if(ifExpIsCon == 1){
            ifExpIsCon = 0;
            regNum--;
            funcSubSpaceForSp();

            ifFactorIsCon = 1;
            factorValue = int(expValue);
        } else {
            ifFactorIsCon = 0;
            thisFactorRegNum = expRegNum;
        }

    } else if(symbol_p == IDENFR){
        newIntermediateCode._name = name_p;
        newIntermediateCode._funcCallName = name_p;


        SIG_SYM temp;
        if(funcSigTab.count(myTolower(name_p)) != 0){
            temp = funcSigTab[myTolower(name_p)]._type;
        } else if(globalSigTab.count(myTolower(name_p)) != 0){
            temp = globalSigTab[myTolower(name_p)]._type;
        } else {
            addError("c", line_p);
        }
        if(temp == INT || temp == CONST_INT){
            res = INT;
        } else if (temp == CHAR || temp == CONST_CHAR){
            res = CHAR;
        }

        newIntermediateCode._interSym = I_CAL;
        newIntermediateCode._resType = 1;
        newIntermediateCode._vcType = res;
        ICodesStack.push(newIntermediateCode);

        if(words[loc_f_p + 1]._symbol == LBRACK){
            newIntermediateCode = ICodesStack.top();
            ICodesStack.pop();
            newIntermediateCode._interSym = I_ARR_GET;
            ICodesStack.push(newIntermediateCode);

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
                if(ifExpIsCon == 1){
                    ifExpIsCon = 0;
                    newIntermediateCode._loc1Type = 2;
                    newIntermediateCode._locInt1 = expValue;
                } else {
                    newIntermediateCode._loc1Type = 1;
                    newIntermediateCode._loc1 = expRegNum;
                }


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

                if(ifExpIsCon == 1){
                    ifExpIsCon = 0;
                    newIntermediateCode._loc2Type = 2;
                    newIntermediateCode._locInt2 = expValue;
                } else {
                    newIntermediateCode._loc2Type = 1;
                    newIntermediateCode._loc2 = expRegNum;
                }

                newIntermediateCode._ansNum = thisFactorRegNum;
                addToICodes();
            }else{
                newIntermediateCode = ICodesStack.top();
                ICodesStack.pop();
                newIntermediateCode._symProperty = 1;
                newIntermediateCode._interSym = I_ARR_GET;
                if(ifExpIsCon == 1){
                    ifExpIsCon = 0;
                    newIntermediateCode._loc1Type = 2;
                    newIntermediateCode._locInt1 = expValue;
                } else {
                    newIntermediateCode._loc1Type = 1;
                    newIntermediateCode._loc1 = expRegNum;
                }
                newIntermediateCode._ansNum = thisFactorRegNum;
                addToICodes();
            }
        } else if(words[loc_f_p + 1]._symbol == LPARENT){
            ICodesStack.pop();
            res = parse_func_call_with_return();
            newIntermediateCode._interSym = I_CAL;
            newIntermediateCode._symProperty = 1;
            newIntermediateCode._resType = 1;
            newIntermediateCode._resRegNum = thisFactorRegNum;
            newIntermediateCode._cal1Type = 5;
            newIntermediateCode._sReg1 = 2;
            newIntermediateCode._cal2Type = 4;
            newIntermediateCode._int2 = 0;
            addToICodes();
        } else{
            newIntermediateCode = ICodesStack.top();
            ICodesStack.pop();
            newIntermediateCode._cal2Type = 2;
            newIntermediateCode._iden2 = name_p;
            newIntermediateCode._interSym = I_CAL;
            newIntermediateCode._symProperty = 1;
            newIntermediateCode._resType = 1;
            newIntermediateCode._resRegNum = thisFactorRegNum;
            newIntermediateCode._cal1Type = 4;
            newIntermediateCode._int1 = 0;
            addToICodes();
            get_next_token();
        }
    }
    factorReg = thisFactorRegNum;
    myPrint("<因子>");
    return res;
}