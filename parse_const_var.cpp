//
// Created by h on 2020/11/6.
//
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "myVar.h"
#include "parse.h"

void parse_const_explain(){
    //t2<<"parse_const_explain"<<std::endl;
    if(symbol_p != CONSTTK)error_parse();
    while(symbol_p == CONSTTK){
        get_next_token();
        parse_const_def();
        if(symbol_p != SEMICN) {
            addError("k", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
    }
    myPrint("<常量说明>");
}
void parse_const_def(){
    newIntermediateCode._interSym = I_CON;
    //t2<<"parse_const_def"<<std::endl;
    if(symbol_p == INTTK){
        newSig._type = CONST_INT;
        prev_type = CONST_INT;
        newSig._line = line_p;

        newIntermediateCode._vcType = INT;

        get_next_token();

        newIntermediateCode._name = name_p;

        parse_new_iden();
        if(symbol_p != ASSIGN)error_parse();
        get_next_token();

        funcAddSpaceForSp();
        newIntermediateCode._intValue.push_back(parse_int());
        addToICodes();
        addToTab();

        while(symbol_p == COMMA){
            newSig._type = CONST_INT;
            newSig._line = line_p;

            get_next_token();
            newIntermediateCode._name = name_p;
            parse_new_iden();
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();
            newIntermediateCode._intValue.push_back(parse_int());
            funcAddSpaceForSp();

            addToICodes();
            addToTab();
        }
    } else if(symbol_p == CHARTK){
        newSig._type = CONST_CHAR;
        prev_type = CONST_CHAR;
        newSig._line = line_p;

        newIntermediateCode._interSym = I_CON;
        newIntermediateCode._vcType = CHAR;

        get_next_token();

        newIntermediateCode._name = name_p;

        parse_new_iden();
        if(symbol_p != ASSIGN)error_parse();
        get_next_token();
        newIntermediateCode._chValue.push_back(parse_char());
        funcAddSpaceForSp();
        addToICodes();

        addToTab();
        while(symbol_p == COMMA){
            newSig._type = CONST_CHAR;
            newSig._line = line_p;

            get_next_token();
            newIntermediateCode._name = name_p;
            parse_new_iden();
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();
            newIntermediateCode._chValue.push_back(parse_char());
            funcAddSpaceForSp();
            addToICodes();
            addToTab();
        }
    } else error_parse();
    myPrint("<常量定义>");
}

void parse_var_explain(){
    //t2<<"parse_var_explain"<<std::endl;
    parse_var_def();
    if(symbol_p != SEMICN) {
        addError("k", words[loc_f_p - 1]._line);
    } else {
        get_next_token();
    }
    while((symbol_p == INTTK || symbol_p == CHARTK) && words[loc_f_p + 1]._symbol == IDENFR
          && words[loc_f_p + 2]._symbol != LPARENT){
        parse_var_def();
        if(symbol_p != SEMICN) {
            addError("k", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
    }
    myPrint("<变量说明>");
}
void parse_var_def(){
    newIntermediateCode._interSym = I_VAR;
    //t2<<"parse_var_def"<<std::endl;
    int initial = 0;
    int i = loc_f_p;
    while(words[i]._symbol != SEMICN){
        if(words[i]._symbol == ASSIGN){
            initial = 1;
            break;
        }
        i++;
    }
    if(initial == 1){
        parse_var_def_and_initial();
    } else {
        parse_var_def_no_initial();
    }
    myPrint("<变量定义>");
}
void parse_var_def_no_initial(){
    //t2<<"parse_var_def_no_initial"<<std::endl;
    if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
    if(symbol_p == INTTK){
        newSig._type = INT;
        prev_type = INT;
        newIntermediateCode._vcType = INT;
    }else {
        newSig._type = CHAR;
        prev_type = CHAR;
        newIntermediateCode._vcType = CHAR;
    }
    get_next_token();

    newSig._line = line_p;

    newIntermediateCode._initialized = 0;
    newIntermediateCode._name = name_p;
    newIntermediateCode._symProperty = 0;
    parse_new_iden();
    if(symbol_p == LBRACK){
        newSig._dem = 1;
        newIntermediateCode._symProperty = 1;
        get_next_token();
        newIntermediateCode._dem1 = parse_unsigned_int();
        newSig._dem_num1 =  newIntermediateCode._dem1;
        if(symbol_p != RBRACK) {
            addError("m", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }

        if(symbol_p == LBRACK){
            newSig._dem = 2;
            newIntermediateCode._symProperty = 2;
            get_next_token();
            newIntermediateCode._dem2 = parse_unsigned_int();
            newSig._dem_num2 = newIntermediateCode._dem2;
            if(symbol_p != RBRACK) {
                addError("m", words[loc_f_p - 1]._line);
            } else {
                get_next_token();
            }

            for(int i = 0; i < newIntermediateCode._dem1 * newIntermediateCode._dem2; i++){
                newIntermediateCode._intValue.push_back(0);
                newIntermediateCode._chValue.push_back(0);
                funcAddSpaceForSp();
            }
        } else{
            for(int i = 0; i < newIntermediateCode._dem1; i++){
                newIntermediateCode._intValue.push_back(0);
                newIntermediateCode._chValue.push_back(0);
                funcAddSpaceForSp();
            }
        }
    }else{
        newIntermediateCode._intValue.push_back(0);
        newIntermediateCode._chValue.push_back(0);
        funcAddSpaceForSp();
    }


    addToICodes();
    addToTab();
    while(symbol_p == COMMA){
        newSig._type = prev_type;
        newSig._line = line_p;
        get_next_token();

        newIntermediateCode._initialized = 0;
        newIntermediateCode._name = name_p;
        newIntermediateCode._symProperty = 0;
        parse_new_iden();
        if(symbol_p == LBRACK){
            newSig._dem = 1;
            newIntermediateCode._symProperty = 1;
            get_next_token();
            newIntermediateCode._dem1 = parse_unsigned_int();
            if(symbol_p != RBRACK) {
                addError("m", words[loc_f_p - 1]._line);
            } else {
                get_next_token();
            }
        }
        if(symbol_p == LBRACK){
            newSig._dem = 2;
            newIntermediateCode._symProperty = 2;
            get_next_token();
            newSig._dem_num2 = parse_unsigned_int();
            newIntermediateCode._dem2 = newSig._dem_num2;
            if(symbol_p != RBRACK) {
                addError("m", words[loc_f_p - 1]._line);
            } else {
                get_next_token();
            }
        }
        int c;
        if(newIntermediateCode._symProperty == 0){
            c = 1;
        } else if(newIntermediateCode._symProperty == 1){
            c = newIntermediateCode._dem1;
        } else{
            c = newIntermediateCode._dem1 * newIntermediateCode._dem2;
        }
        for(int i = 0; i < c; i++){
            newIntermediateCode._intValue.push_back(0);
            newIntermediateCode._chValue.push_back(0);
            funcAddSpaceForSp();
        }

        addToICodes();
        addToTab();
    }
    myPrint("<变量定义无初始化>");
}
void parse_var_def_and_initial(){
    //t2<<"parse_var_def_and_initial"<<std::endl;
    //parse_type_iden();
    newIntermediateCode._initialized = 1;
    if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
    if(symbol_p == INTTK){
        newSig._type = INT;
        newIntermediateCode._vcType = INT;
    } else {
        newSig._type = CHAR;
        newIntermediateCode._vcType = CHAR;
    }
    get_next_token();

    newSig._line = line_p;

    newIntermediateCode._name = name_p;
    parse_new_iden();
    if(symbol_p == LBRACK){
        newSig._dem = 1;
        newIntermediateCode._symProperty = 1;

        get_next_token();
        newSig._dem_num1 = parse_unsigned_int();
        newIntermediateCode._dem1 = newSig._dem_num1;

        if(symbol_p != RBRACK) {
            addError("m", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }

        if(symbol_p == LBRACK){
            newSig._dem = 2;
            newIntermediateCode._symProperty = 2;

            get_next_token();
            newSig._dem_num2 = parse_unsigned_int();
            newIntermediateCode._dem2 = newSig._dem_num2;

            if(symbol_p != RBRACK) {
                addError("m", words[loc_f_p - 1]._line);
            } else {
                get_next_token();
            }
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();

            if(symbol_p != LBRACE)error_parse();
            get_next_token();
            int myCount1 = 1;

            if(symbol_p != LBRACE)error_parse();
            get_next_token();

            if(parse_constant() != newSig._type){
                addError("o", line_p);
            }
            int myCount2 = 1;
            while(symbol_p == COMMA){
                get_next_token();
                if(parse_constant() != newSig._type){
                    addError("o", line_p);
                }
                myCount2++;
            }
            if(myCount2 != newSig._dem_num2){
                addError("n", line_p);
            }
            myCount2 = 0;
            if(symbol_p != RBRACE)error_parse();
            get_next_token();
            while(symbol_p == COMMA){
                get_next_token();
                if(symbol_p != LBRACE)error_parse();
                get_next_token();

                myCount2 = 1;
                if(parse_constant() != newSig._type){
                    addError("o", line_p);
                }
                while(symbol_p == COMMA){
                    get_next_token();
                    if(parse_constant() != newSig._type){
                        addError("o", line_p);
                    }
                    myCount2++;
                }
                if(myCount2 != newSig._dem_num2){
                    addError("n", line_p);
                }
                myCount2 = 0;

                if(symbol_p != RBRACE)error_parse();
                get_next_token();
                myCount1++;
            }
            if(symbol_p != RBRACE)error_parse();
            if(myCount1 != newSig._dem_num1){
                addError("n", line_p);
            }
            get_next_token();
        }else{
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();
            if(symbol_p != LBRACE)error_parse();
            get_next_token();

            int myCount1 = 0;
            if(parse_constant() != newSig._type){
                addError("o", line_p);
            }
            myCount1++;
            while(symbol_p == COMMA){
                get_next_token();
                if(parse_constant() != newSig._type){
                    addError("o", line_p);
                }
                myCount1++;
            }
            if(myCount1 != newSig._dem_num1){
                addError("n", line_p);
            }
            if(symbol_p != RBRACE)error_parse();
            get_next_token();
        }
    } else {
        newSig._dem = 0;
        newIntermediateCode._symProperty = 0;

        if(symbol_p != ASSIGN)error_parse();
        get_next_token();

        if(parse_constant() != newSig._type) {
            addError("o", line_p);
        }
    }
    int c;
    if(newIntermediateCode._symProperty == 0){
        c = 1;
    } else if(newIntermediateCode._symProperty == 1){
        c = newIntermediateCode._dem1;
    } else{
        c = newIntermediateCode._dem2 * newIntermediateCode._dem1;
    }
    for(int i = 0; i < c; i++){
        funcAddSpaceForSp();
    }
    addToICodes();
    addToTab();
    myPrint("<变量定义及初始化>");
}