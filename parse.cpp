#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include "parse.h"
#include "myVar.h"

int loc_f_p;
SYMBOL symbol_p;
std::string name_p;
int line_p;
std::map<std::string, SYMBOL> func_tab;
Signal newSig = Signal();
SIG_SYM prev_type;
bool isGlobal = true;
std::string func_name;//def
std::stack<std::string> call_func_name;
bool hasReturn = false;

bool isSameType1(SIG_SYM sig1, SIG_SYM sig2){
    if(sig1 == INT || sig1 == CONST_INT){
        if(sig2 == INT || sig2 == CONST_INT){
            return true;
        }
        return false;
    }
    if(sig1 == CHAR || sig1 == CONST_CHAR){
        if(sig2 == CHAR || sig2 == CONST_CHAR){
            return true;
        }
        return false;
    }
}


std::string myTolower(std::string myStr){
    std::string temp = myStr;
    transform(temp.begin(),temp.end(),temp.begin(),::tolower);
    return temp;
}

void clearSig(){
    newSig = Signal();
}

void addError(std::string error_type, int temp_line){
    errors.push_back(Error(error_type, temp_line));
}

void addToTab(){
    std::string temp =  myTolower(newSig._name);
    if (isGlobal) {
        if(globalSigTab.count(temp) == 0) {
            globalSigTab[temp] = newSig;
        } else {
            addError("b", newSig._line);
        }
    } else if(newSig._type == FUNC_NO_RETURN || newSig._type == FUNC_WITH_RETURN) {
        if(globalSigTab.count(temp) == 0) {
            globalSigTab[temp] = newSig;
        } else {
            addError("b", newSig._line);
        }
        funcSigTab[temp] = newSig;
    } else {
        if(funcSigTab.count(temp) == 0) {
            funcSigTab[temp] = newSig;
        } else {
            addError("b", newSig._line);
        }
    }
    clearSig();
}

void clearFuncSigTab(){
    funcSigTab.clear();
}

bool isDefined(std::string str_name){
    if(globalSigTab.count(myTolower(str_name)) == 0 && funcSigTab.count(myTolower(str_name)) == 0){
        return false;
    }
    return true;
}

void myPrint(std::string str){
    if(0) {
        output << str << std::endl;
    }
}

void error_parse(){
    myPrint("error in parse!");
}
void get_next_token(){
    loc_f_p++;
    symbol_p = words[loc_f_p]._symbol;
    name_p = words[loc_f_p]._name;
    line_p = words[loc_f_p]._line;
    myPrint(names[words[loc_f_p - 1]._symbol] + " "+ words[loc_f_p - 1]._name);
}

void parse_iden(){
    if(!isDefined(name_p)){
        addError("c", line_p);
    }
    if(symbol_p != IDENFR)error_parse();
    get_next_token();
}

void parse_new_iden(){
    if(symbol_p != IDENFR)error_parse();
    newSig._name = name_p;
    get_next_token();
}

void parse_plus(){
    if(symbol_p != PLUS && symbol_p != MINU)error_parse();
    get_next_token();
}
void parse_mult(){
    if(symbol_p != MULT && symbol_p != DIV)error_parse();
    get_next_token();
}
void parse_relation(){
    if(symbol_p == LSS || symbol_p == LEQ || symbol_p == GRE || symbol_p == GEQ
    || symbol_p == EQL || symbol_p == NEQ){
        get_next_token();
        return;
    }
    error_parse();
}
void parse_char(){
    if(symbol_p != CHARCON)error_parse();
    get_next_token();
}
void parse_string() {
    if (symbol_p != STRCON)error_parse();
    get_next_token();
    myPrint("<字符串>");
}
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
void parse_const_explain(){
    //t2<<"parse_const_explain"<<std::endl;
    if(symbol_p != CONSTTK)error_parse();
    while(symbol_p == CONSTTK){
        get_next_token();
        parse_const_def();
        if(symbol_p != SEMICN) {
            addError("k", line_p);
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
int parse_unsigned_int(){
    if(symbol_p != INTCON)error_parse();
    int res = transNum(name_p);
    get_next_token();
    myPrint("<无符号整数>");
    return res;
}
void parse_int(){
    if(symbol_p == PLUS || symbol_p == MINU){
        parse_plus();
    }
    parse_unsigned_int();
    myPrint("<整数>");
}

void parse_statement_head(){
    if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
    SYMBOL temp_type = symbol_p;

    get_next_token();

    func_tab[words[loc_f_p]._name] = temp_type;
    func_name = words[loc_f_p]._name;

    parse_new_iden();
    myPrint("<声明头部>");
}
SIG_SYM parse_constant(){
    //t2<<"parse_constant"<<std::endl;
    SIG_SYM res = NONETYPE;
    if(symbol_p == CHARCON){
        res = CHAR;
        parse_char();
    } else if(symbol_p == INTCON || ((symbol_p == PLUS || symbol_p == MINU) && (words[loc_f_p + 1]._symbol == INTCON))){
        parse_int();
        res = INT;
    }else error_parse();
    myPrint("<常量>");
    return res;
}
void parse_var_explain(){
    //t2<<"parse_var_explain"<<std::endl;
    parse_var_def();
    if(symbol_p != SEMICN) {
        addError("k", line_p);
    } else {
        get_next_token();
    }
    while((symbol_p == INTTK || symbol_p == CHARTK) && words[loc_f_p + 1]._symbol == IDENFR
          && words[loc_f_p + 2]._symbol != LPARENT){
        parse_var_def();
        if(symbol_p != SEMICN) {
            addError("k", line_p);
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
            addError("m", line_p);
        } else {
            get_next_token();
        }
    }
    if(symbol_p == LBRACK){
        newSig._dem = 2;
        get_next_token();
        parse_unsigned_int();
        if(symbol_p != RBRACK) {
            addError("m", line_p);
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
                addError("m", line_p);
            } else {
                get_next_token();
            }
        }
        if(symbol_p == LBRACK){
            newSig._dem = 2;
            get_next_token();
            parse_unsigned_int();
            if(symbol_p != RBRACK) {
                addError("m", line_p);
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
            addError("m", line_p);
        } else {
            get_next_token();
        }

        if(symbol_p == LBRACK){
            newSig._dem = 2;
            get_next_token();
            newSig._dem_num2 = parse_unsigned_int();
            if(symbol_p != RBRACK) {
                addError("m", line_p);
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
void parse_type_iden(){
    if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
    get_next_token();
}
void parse_func_with_return_def(){
    newSig._type = FUNC_WITH_RETURN;
    newSig._line = line_p;
    if(symbol_p == INTTK){
        newSig._returnType = INT;
    } else if(symbol_p == CHARTK) {
        newSig._returnType = CHAR;
    }

    parse_statement_head();
    addToTab();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_para_tab();

    if(symbol_p != RPARENT) {
        addError("l", line_p);
    } else {
        get_next_token();
    }

    if(symbol_p != LBRACE)error_parse();
    get_next_token();

    parse_compound_sent();

    if(symbol_p != RBRACE)error_parse();
    if(!hasReturn){
        std::cout<<func_name<<"-return wrong 1"<<std::endl;
        addError("h", line_p);
    } else {
        hasReturn = false;
    }
    get_next_token();
    myPrint("<有返回值函数定义>");
}
void parse_func_no_return_def(){
    if(symbol_p != VOIDTK)error_parse();
    get_next_token();

    newSig._type = FUNC_NO_RETURN;
    newSig._returnType = NONETYPE;
    newSig._line = line_p;

    func_tab[words[loc_f_p]._name] = VOIDTK;
    func_name = words[loc_f_p]._name;

    parse_new_iden();
    addToTab();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_para_tab();

    if(symbol_p != RPARENT) {
        addError("l", line_p);
    } else {
        get_next_token();
    }

    if(symbol_p != LBRACE)error_parse();
    get_next_token();

    parse_compound_sent();

    if(symbol_p != RBRACE)error_parse();
    get_next_token();
    clearFuncSigTab();
    myPrint("<无返回值函数定义>");
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
void parse_para_tab(){
    if(symbol_p != RPARENT) {
        if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
        if(symbol_p == INTTK)newSig._type = INT;
        else newSig._type = CHAR;

        newSig._line = line_p;

        globalSigTab[myTolower(func_name)].func_addPara(newSig._type);

        get_next_token();

        parse_new_iden();
        addToTab();


        while(symbol_p == COMMA){
            get_next_token();
            if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
            if(symbol_p == INTTK)newSig._type = INT;
            else newSig._type = CHAR;

            newSig._line = line_p;

            globalSigTab[myTolower(func_name)].func_addPara(newSig._type);

            get_next_token();

            parse_new_iden();
            addToTab();
        }
    }
    myPrint("<参数表>");
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
        addError("l", line_p);
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
            addError("l", line_p);
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
            if(parse_expression() !=INT && parse_expression() != CONST_INT){
                addError("i", line_p);
            }
            if(symbol_p != RBRACK) {
                addError("m", line_p);
            } else {
                get_next_token();
            }
            if(symbol_p == LBRACK){
                get_next_token();
                if(parse_expression() != INT && parse_expression() != CONST_INT){
                    addError("i", line_p);
                }
                if(symbol_p != RBRACK) {
                    addError("m", line_p);
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
void parse_sent(){
    if(symbol_p == WHILETK || symbol_p == FORTK)parse_loop_sent();
    else if(symbol_p == IFTK)parse_cond_sent();
    else if(symbol_p == IDENFR){
        if(words[loc_f_p + 1]._symbol == LPARENT){
            if(func_tab[words[loc_f_p]._name] == VOIDTK){
                parse_func_call_no_return();
            } else parse_func_call_with_return();
        }
        else parse_assign_sent();

        if(symbol_p != SEMICN) {
            addError("k", line_p);
        } else {
            get_next_token();
        }
    }
    else if(symbol_p == SCANFTK){
        parse_read_sent();
        if(symbol_p != SEMICN) {
            addError("k", line_p);
        } else {
            get_next_token();
        }
    }
    else if(symbol_p == PRINTFTK){
        parse_print_sent();
        if(symbol_p != SEMICN) {
            addError("k", line_p);
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
            addError("k", line_p);
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
    if(globalSigTab.count(myTolower(name_p)) != 0 &&
        (globalSigTab[myTolower(name_p)]._type == CONST_INT
            || globalSigTab[myTolower(name_p)]._type == CONST_CHAR)){
        addError("j", line_p);
    } else if(funcSigTab.count(myTolower(name_p)) != 0 &&
              (funcSigTab[myTolower(name_p)]._type == CONST_INT
               || funcSigTab[myTolower(name_p)]._type == CONST_CHAR)){
        addError("j", line_p);
    }
    parse_iden();
    if(symbol_p == ASSIGN){
        get_next_token();
        parse_expression();
    } else if(symbol_p == LBRACK){
        get_next_token();
        if(parse_expression() != INT && parse_expression() != CONST_INT){
            addError("i", line_p);
        }
        if(symbol_p != RBRACK) {
            addError("m", line_p);
        } else {
            get_next_token();
        }
        if(symbol_p == LBRACK){
            get_next_token();
            if(parse_expression() != INT && parse_expression() != CONST_INT){
                addError("i", line_p);
            }
            if(symbol_p != RBRACK) {
                addError("m", line_p);
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
        addError("l", line_p);
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
            addError("l", line_p);
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
            addError("k", line_p);
        } else {
            get_next_token();
        }
        parse_condition();
        if(symbol_p != SEMICN) {
            addError("k", line_p);
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
            addError("l", line_p);
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
        addError("l", line_p);
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
SIG_SYM parse_func_call_with_return(){
    call_func_name.push(name_p);
    parse_iden();
    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_val_para_tab();

    if(symbol_p != RPARENT) {
        addError("l", line_p);
    } else {
        get_next_token();
    }
    myPrint("<有返回值函数调用语句>");
    SIG_SYM temp = globalSigTab[myTolower(call_func_name.top())]._returnType;
    call_func_name.pop();
    return temp;
}
SIG_SYM parse_func_call_no_return(){
    call_func_name.push(name_p);
    parse_iden();
    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_val_para_tab();

    if(symbol_p != RPARENT) {
        addError("l", line_p);
    } else {
        get_next_token();
    }
    myPrint("<无返回值函数调用语句>");
    call_func_name.pop();
    return NONETYPE;
}
void parse_val_para_tab(){
    int num = 0;
    std::vector<SIG_SYM> temp_list;
    if(symbol_p != RPARENT){
        temp_list.push_back(parse_expression());
        if(temp_list[temp_list.size() - 1] != NONETYPE){
            num++;
        }
        while(symbol_p == COMMA){
            get_next_token();
            temp_list.push_back(parse_expression());
            num++;
        }
    }

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
void parse_sent_col(){
    while(symbol_p != RBRACE){
        parse_sent();
    }
    myPrint("<语句列>");
}
void parse_read_sent(){
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
    parse_iden();

    if(symbol_p != RPARENT) {
        addError("l", line_p);
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
        parse_string();
        if(symbol_p == COMMA){
            get_next_token();
            parse_expression();
        }
    }else{
        parse_expression();
    }
    if(symbol_p != RPARENT) {
        addError("l", line_p);
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
            addError("l", line_p);
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