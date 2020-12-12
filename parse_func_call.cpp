//
// Created by h on 2020/11/6.
//
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "myVar.h"
#include "parse.h"

SIG_SYM parse_func_call_with_return(){
    //sym = i_func_call: 0: no_return 1:return int 2:return char
    newIntermediateCode._interSym = I_FUNC_CALL;
    if(globalSigTab[myTolower(name_p)]._returnType == INT){
        newIntermediateCode._symProperty = 1;
    } else if(globalSigTab[myTolower(name_p)]._returnType == CHAR){
        newIntermediateCode._symProperty = 2;
    } else{
        newIntermediateCode._symProperty = 0;
    }
    call_func_name.push(name_p);
    newIntermediateCode._funcCallName = name_p;
    ICodesStack.push(newIntermediateCode);

    parse_iden();
    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_val_para_tab();
    addToICodes();

    if(symbol_p != RPARENT) {
        addError("l", words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }

    newIntermediateCode._interSym = I_FUNC_CALL_END;
    newIntermediateCode._funcCallName = call_func_name.top();
    addToICodes();

    myPrint("<有返回值函数调用语句>");
    SIG_SYM temp = globalSigTab[myTolower(call_func_name.top())]._returnType;
    call_func_name.pop();
    return temp;
}
SIG_SYM parse_func_call_no_return(){
    newIntermediateCode._interSym = I_FUNC_CALL;
    newIntermediateCode._funcCallName = name_p;
    ICodesStack.push(newIntermediateCode);

    call_func_name.push(name_p);
    parse_iden();
    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_val_para_tab();
    addToICodes();

    if(symbol_p != RPARENT) {
        addError("l", words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }

    newIntermediateCode._interSym = I_FUNC_CALL_END;
    newIntermediateCode._funcCallName = call_func_name.top();
    addToICodes();

    myPrint("<无返回值函数调用语句>");
    call_func_name.pop();
    return NONETYPE;
}
void parse_val_para_tab(){
    int num = 0;
    std::vector<SIG_SYM> temp_list;
    if(symbol_p != RPARENT){
        temp_list.push_back(parse_expression());

        newIntermediateCode = ICodesStack.top();
        ICodesStack.pop();
        newIntermediateCode._paraRegNum.clear();
        if(ifExpIsCon == 1){
            ifExpIsCon = 0;
            newIntermediateCode._paraRegNum.push_back(Value(2, expValue));
        } else {
            newIntermediateCode._paraRegNum.push_back(Value(1, expRegNum));
        }

        ICodesStack.push(newIntermediateCode);

        if(temp_list[temp_list.size() - 1] != NONETYPE){
            num++;
        }
        while(symbol_p == COMMA){
            get_next_token();
            temp_list.push_back(parse_expression());
            num++;
            newIntermediateCode = ICodesStack.top();
            ICodesStack.pop();
            if(ifExpIsCon == 1){
                ifExpIsCon = 0;
                newIntermediateCode._paraRegNum.push_back(Value(2, expValue));
            } else {
                newIntermediateCode._paraRegNum.push_back(Value(1, expRegNum));
            }
            ICodesStack.push(newIntermediateCode);
        }
    }
    newIntermediateCode = ICodesStack.top();
    ICodesStack.pop();
    newIntermediateCode._paraType = temp_list;

    if(num == globalSigTab[myTolower(call_func_name.top())]._para_tab.size()){
        for(int i = 0; i < num; i++){
            if(temp_list[i] != NONETYPE &&
               !isSameType1(temp_list[i], globalSigTab[myTolower(call_func_name.top())]._para_tab[i])){
                std::cout<<call_func_name.top()<<"---"<<i<<std::endl;
                addError("e", line_p);
                break;
            }
        }
    } else {
        addError("d", line_p);
    }
    myPrint("<值参数表>");
}