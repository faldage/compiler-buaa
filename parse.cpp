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
    funcInitialSpaceForSp();

    newIntermediateCode._interSym = I_CODE;
    newIntermediateCode._code = "jal main";
    addToICodes();

    newIntermediateCode._interSym = I_CODE;
    newIntermediateCode._code = "j labelend";
    addToICodes();

    while(words[loc_f_p + 1]._symbol == IDENFR && words[loc_f_p + 2]._symbol == LPARENT){
        if(symbol_p == VOIDTK){
            parse_func_no_return_def();
        } else if(symbol_p == INTTK || symbol_p == CHARTK){
            parse_func_with_return_def();
        } else error_parse();

        clearFuncSigTab();
        clearSig();

        finishCountSpace();
    }
    parse_main();
    clearFuncSigTab();
    finishCountSpace();

    newIntermediateCode._interSym = I_LABEL;
    newIntermediateCode._labelName = "end";
    addToICodes();
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
    newIntermediateCode._interSym = I_FUNC_DEF;
    newIntermediateCode._funcDefName = "main";
    addToICodes();
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

    if(funcSigTab.count(myTolower(name_p)) != 0){
        if(funcSigTab[myTolower(name_p)]._type == INT){
            newIntermediateCode._assType = INT;
        } else {
            newIntermediateCode._assType = CHAR;
        }
    } else if(globalSigTab.count(myTolower(name_p)) != 0){
        if(globalSigTab[myTolower(name_p)]._type == INT){
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
        newIntermediateCode._interSym = I_ARR_ASS;
        newIntermediateCode._symProperty = 1;
        ICodesStack.push(newIntermediateCode);

        SIG_SYM temp = parse_expression();

        newIntermediateCode = ICodesStack.top();
        ICodesStack.pop();
        newIntermediateCode._loc1 = expRegNum;

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

            newIntermediateCode._symProperty = 2;
            ICodesStack.push(newIntermediateCode);

            temp = parse_expression();

            newIntermediateCode = ICodesStack.top();
            ICodesStack.pop();
            newIntermediateCode._loc2 = expRegNum;

            std::cout<<"@"<<newIntermediateCode._assName<<" # ";
            if(funcSigTabMap[myTolower(func_name)].count(myTolower(newIntermediateCode._assName)) != 0) {
                newIntermediateCode._length = funcSigTabMap[myTolower(func_name)][myTolower(newIntermediateCode._assName)]._dem_num2;
            } else if(globalSigTab.count(myTolower(newIntermediateCode._assName)) != 0){
                newIntermediateCode._length = globalSigTab[myTolower(newIntermediateCode._assName)]._dem_num2;
            } else {
                std::cout<<"wrong when find length! in ass"<<std::endl;
            }
            std::cout<<"length = "<<newIntermediateCode._length<<std::endl;
            ICodesStack.push(newIntermediateCode);

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

            newIntermediateCode = ICodesStack.top();
            ICodesStack.pop();
            newIntermediateCode._arr_ass_regNum = expRegNum;
            addToICodes();
        } else if(symbol_p == ASSIGN){
            ICodesStack.push(newIntermediateCode);

            get_next_token();
            parse_expression();

            newIntermediateCode = ICodesStack.top();
            ICodesStack.pop();
            newIntermediateCode._arr_ass_regNum = expRegNum;
            addToICodes();
        } else error_parse();
    } else error_parse();
    myPrint("<赋值语句>");
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

    if(funcSigTab.count(myTolower(name_p)) != 0){
        if(funcSigTab[myTolower(name_p)]._type == INT){
            newIntermediateCode._scType = INT;
        } else {
            newIntermediateCode._scType = CHAR;
        }
    } else if(globalSigTab.count(myTolower(name_p)) != 0){
        if(globalSigTab[myTolower(name_p)]._type == INT){
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
        for(int i = loc_f_p; i < words.size(); i++){
            if(words[i]._symbol == SEMICN){
                newIntermediateCode._symProperty = 1;
                break;
            }
            if(words[i]._symbol== COMMA){
                newIntermediateCode._symProperty = 3;
                break;
            }
        }
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
        newIntermediateCode._interSym = I_CAL;
        newIntermediateCode._cal1Type = 1;
        newIntermediateCode._reg1 = expRegNum;
        newIntermediateCode._cal2Type = 4;
        newIntermediateCode._int2 = 0;
        newIntermediateCode._resType = 2;
        newIntermediateCode._resReg2 = 2;
        newIntermediateCode._symProperty = 1;
        addToICodes();

    } else {
        newIntermediateCode._interSym = I_CODE;
        newIntermediateCode._code = "move $2, $0";
        addToICodes();
        if(globalSigTab[myTolower(func_name)]._returnType != NONETYPE){
            addError("h", line_p);
        }
    }
    newIntermediateCode._interSym = I_FUNC_DEF_END;
    newIntermediateCode._funcDefName = func_name;
    addToICodes();
    myPrint("<返回语句>");
}

void parse(){
    loc_f_p = 0;
    symbol_p = words[0]._symbol;
    parse_program();
}