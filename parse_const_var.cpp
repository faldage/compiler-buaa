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
    //t2<<"parse_const_def"<<std::endl;
    if(symbol_p == INTTK){
        newSig._type = CONST_INT;
        prev_type = CONST_INT;
        newSig._line = line_p;

        get_next_token();
        parse_new_iden();
        if(symbol_p != ASSIGN)error_parse();
        get_next_token();
        parse_int();

        addToTab();
        while(symbol_p == COMMA){
            newSig._type = CONST_INT;
            newSig._line = line_p;

            get_next_token();
            parse_new_iden();
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();
            parse_int();

            addToTab();
        }
    } else if(symbol_p == CHARTK){
        newSig._type = CONST_CHAR;
        prev_type = CONST_CHAR;
        newSig._line = line_p;

        get_next_token();
        parse_new_iden();
        if(symbol_p != ASSIGN)error_parse();
        get_next_token();
        parse_char();

        addToTab();
        while(symbol_p == COMMA){
            newSig._type = CONST_CHAR;
            newSig._line = line_p;

            get_next_token();
            parse_new_iden();
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();
            parse_char();

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
    }else {
        newSig._type = CHAR;
        prev_type = CHAR;
    }
    get_next_token();

    newSig._line = line_p;
    parse_new_iden();
    if(symbol_p == LBRACK){
        newSig._dem = 1;
        get_next_token();
        parse_unsigned_int();
        if(symbol_p != RBRACK) {
            addError("m", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
    }
    if(symbol_p == LBRACK){
        newSig._dem = 2;
        get_next_token();
        parse_unsigned_int();
        if(symbol_p != RBRACK) {
            addError("m", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }
    }
    addToTab();
    while(symbol_p == COMMA){
        newSig._type = prev_type;
        newSig._line = line_p;
        get_next_token();
        parse_new_iden();
        if(symbol_p == LBRACK){
            newSig._dem = 1;
            get_next_token();
            parse_unsigned_int();
            if(symbol_p != RBRACK) {
                addError("m", words[loc_f_p - 1]._line);
            } else {
                get_next_token();
            }
        }
        if(symbol_p == LBRACK){
            newSig._dem = 2;
            get_next_token();
            parse_unsigned_int();
            if(symbol_p != RBRACK) {
                addError("m", words[loc_f_p - 1]._line);
            } else {
                get_next_token();
            }
        }
        addToTab();
    }
    myPrint("<变量定义无初始化>");
}
void parse_var_def_and_initial(){
    //t2<<"parse_var_def_and_initial"<<std::endl;
    //parse_type_iden();
    if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
    if(symbol_p == INTTK)newSig._type = INT;
    else newSig._type = CHAR;
    get_next_token();

    newSig._line = line_p;

    parse_new_iden();
    if(symbol_p == LBRACK){
        newSig._dem = 1;
        get_next_token();
        newSig._dem_num1 = parse_unsigned_int();
        if(symbol_p != RBRACK) {
            addError("m", words[loc_f_p - 1]._line);
        } else {
            get_next_token();
        }

        if(symbol_p == LBRACK){
            newSig._dem = 2;
            get_next_token();
            newSig._dem_num2 = parse_unsigned_int();
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
        if(symbol_p != ASSIGN)error_parse();
        get_next_token();
        if(parse_constant() != newSig._type){
            addError("o", line_p);
        }
    }
    addToTab();
    myPrint("<变量定义及初始化>");
}