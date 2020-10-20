
#include "parse.h"

int loc_f_p;
SYMBOL symbol_p;
std::ofstream t2("output.txt");
std::map<std::string, SYMBOL> func_tab;

void error_parse(){
    t2<<"error in parse!"<<std::endl;
}
void get_next_token(){
    loc_f_p++;
    symbol_p = words[loc_f_p]._symbol;
    t2<<names[words[loc_f_p - 1]._symbol] << " "<< words[loc_f_p - 1]._name<<std::endl;
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
    t2<<"<字符串>"<<std::endl;
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
    while(words[loc_f_p + 1]._symbol == IDENFR && words[loc_f_p + 2]._symbol == LPARENT){
        if(symbol_p == VOIDTK){
            parse_func_no_return_def();
        } else if(symbol_p == INTTK || symbol_p == CHARTK){
            parse_func_with_return_def();
        } else error_parse();
    }
    parse_main();
    t2<<"<程序>"<<std::endl;
}
void parse_const_explain(){
    //t2<<"parse_const_explain"<<std::endl;
    if(symbol_p != CONSTTK)error_parse();
    while(symbol_p == CONSTTK){
        get_next_token();
        parse_const_def();
        if(symbol_p != SEMICN)error_parse();
        get_next_token();
    }
    t2<<"<常量说明>"<<std::endl;
}
void parse_const_def(){
    //t2<<"parse_const_def"<<std::endl;
    if(symbol_p == INTTK){
        get_next_token();
        parse_iden();
        if(symbol_p != ASSIGN)error_parse();
        get_next_token();
        parse_int();
        while(symbol_p == COMMA){
            get_next_token();
            parse_iden();
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();
            parse_int();
        }
    } else if(symbol_p == CHARTK){
        get_next_token();
        parse_iden();
        if(symbol_p != ASSIGN)error_parse();
        get_next_token();
        parse_char();
        while(symbol_p == COMMA){
            get_next_token();
            parse_iden();
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();
            parse_char();
        }
    } else error_parse();
    t2<<"<常量定义>"<<std::endl;
}
void parse_unsigned_int(){
    if(symbol_p != INTCON)error_parse();
    get_next_token();
    t2<<"<无符号整数>"<<std::endl;
}
void parse_int(){
    if(symbol_p == PLUS || symbol_p == MINU){
        parse_plus();
    }
    parse_unsigned_int();
    t2<<"<整数>"<<std::endl;
}
void parse_iden(){
    if(symbol_p != IDENFR)error_parse();
    get_next_token();
}
void parse_statement_head(){
    if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
    SYMBOL temp_type = symbol_p;

    get_next_token();

    func_tab[words[loc_f_p]._name] = temp_type;
    parse_iden();
    t2<<"<声明头部>"<<std::endl;
}
void parse_constant(){
    //t2<<"parse_constant"<<std::endl;
    if(symbol_p == CHARCON){
        parse_char();
    } else if(symbol_p == INTCON || ((symbol_p == PLUS || symbol_p == MINU) && (words[loc_f_p + 1]._symbol == INTCON))){
        parse_int();
    }else error_parse();
    t2<<"<常量>"<<std::endl;
}
void parse_var_explain(){
    //t2<<"parse_var_explain"<<std::endl;
    parse_var_def();
    if(symbol_p != SEMICN)error_parse();
    get_next_token();
    while((symbol_p == INTTK || symbol_p == CHARTK) && words[loc_f_p + 1]._symbol == IDENFR
          && words[loc_f_p + 2]._symbol != LPARENT){
        parse_var_def();
        if (symbol_p != SEMICN)error_parse();
        get_next_token();
    }
    t2<<"<变量说明>"<<std::endl;
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
    t2<<"<变量定义>"<<std::endl;
}
void parse_var_def_no_initial(){
    //t2<<"parse_var_def_no_initial"<<std::endl;
    parse_type_iden();
    parse_iden();
    if(symbol_p == LBRACK){
        get_next_token();
        parse_unsigned_int();
        if(symbol_p != RBRACK)error_parse();
        get_next_token();
    }
    if(symbol_p == LBRACK){
        get_next_token();
        parse_unsigned_int();
        if(symbol_p != RBRACK)error_parse();
        get_next_token();
    }
    while(symbol_p == COMMA){
        get_next_token();
        parse_iden();
        if(symbol_p == LBRACK){
            get_next_token();
            parse_unsigned_int();
            if(symbol_p != RBRACK)error_parse();
            get_next_token();
        }
        if(symbol_p == LBRACK){
            get_next_token();
            parse_unsigned_int();
            if(symbol_p != RBRACK)error_parse();
            get_next_token();
        }
    }
    t2<<"<变量定义无初始化>"<<std::endl;
}
void parse_var_def_and_initial(){
    //t2<<"parse_var_def_and_initial"<<std::endl;
    parse_type_iden();
    parse_iden();
    if(symbol_p == LBRACK){
        get_next_token();
        parse_unsigned_int();
        if(symbol_p != RBRACK)error_parse();
        get_next_token();

        if(symbol_p == LBRACK){
            get_next_token();
            parse_unsigned_int();
            if(symbol_p != RBRACK)error_parse();
            get_next_token();
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();

            if(symbol_p != LBRACE)error_parse();
            get_next_token();
            if(symbol_p != LBRACE)error_parse();
            get_next_token();

            parse_constant();
            while(symbol_p == COMMA){
                get_next_token();
                parse_constant();
            }
            if(symbol_p != RBRACE)error_parse();
            get_next_token();
            while(symbol_p == COMMA){
                get_next_token();
                if(symbol_p != LBRACE)error_parse();
                get_next_token();

                parse_constant();
                while(symbol_p == COMMA){
                    get_next_token();
                    parse_constant();
                }
                if(symbol_p != RBRACE)error_parse();
                get_next_token();
            }
            if(symbol_p != RBRACE)error_parse();
            get_next_token();
        }else{
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();
            if(symbol_p != LBRACE)error_parse();
            get_next_token();

            parse_constant();
            while(symbol_p == COMMA){
                get_next_token();
                parse_constant();
            }
            if(symbol_p != RBRACE)error_parse();
            get_next_token();
        }
    } else {
        if(symbol_p != ASSIGN)error_parse();
        get_next_token();
        parse_constant();
    }
    t2<<"<变量定义及初始化>"<<std::endl;
}
void parse_type_iden(){
    if(symbol_p != INTTK && symbol_p != CHARTK)error_parse();
    get_next_token();
}
void parse_func_with_return_def(){
    parse_statement_head();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_para_tab();

    if(symbol_p != RPARENT)error_parse();
    get_next_token();

    if(symbol_p != LBRACE)error_parse();
    get_next_token();

    parse_compound_sent();

    if(symbol_p != RBRACE)error_parse();
    get_next_token();
    t2<<"<有返回值函数定义>"<<std::endl;
}
void parse_func_no_return_def(){
    if(symbol_p != VOIDTK)error_parse();
    get_next_token();

    func_tab[words[loc_f_p]._name] = VOIDTK;
    parse_iden();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_para_tab();

    if(symbol_p != RPARENT)error_parse();
    get_next_token();

    if(symbol_p != LBRACE)error_parse();
    get_next_token();

    parse_compound_sent();

    if(symbol_p != RBRACE)error_parse();
    get_next_token();
    t2<<"<无返回值函数定义>"<<std::endl;
}
void parse_compound_sent(){
    if(symbol_p == CONSTTK){
        parse_const_explain();
    }
    if(symbol_p == INTTK || symbol_p == CHARTK){
        parse_var_explain();
    }
    parse_sent_col();
    t2<<"<复合语句>"<<std::endl;
}
void parse_para_tab(){
    if(symbol_p != RPARENT) {
        parse_type_iden();
        parse_iden();
        while(symbol_p == COMMA){
            get_next_token();
            parse_type_iden();
            parse_iden();
        }
    }
    t2<<"<参数表>"<<std::endl;
}
void parse_main(){
    if(symbol_p != VOIDTK)error_parse();
    get_next_token();
    if(symbol_p != MAINTK)error_parse();
    get_next_token();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();
    if(symbol_p != RPARENT)error_parse();
    get_next_token();

    if(symbol_p != LBRACE)error_parse();
    get_next_token();

    parse_compound_sent();

    if(symbol_p != RBRACE)error_parse();
    t2<<names[words[loc_f_p]._symbol] << " "<< words[loc_f_p]._name<<std::endl;
    t2<<"<主函数>"<<std::endl;
}
void parse_expression(){
    if(symbol_p == PLUS || symbol_p == MINU){
        get_next_token();
    }
    parse_item();
    while(symbol_p == PLUS || symbol_p == MINU){
        get_next_token();
        parse_item();
    }
    t2<<"<表达式>"<<std::endl;
}
void parse_item(){
    parse_factor();
    while(symbol_p == MULT || symbol_p == DIV){
        get_next_token();
        parse_factor();
    }
    t2<<"<项>"<<std::endl;
}
void parse_factor(){
    if(symbol_p == INTCON || ((symbol_p == PLUS || symbol_p == MINU) &&words[loc_f_p + 1]._symbol == INTCON)){//++1
        parse_int();
    } else if(symbol_p == CHARCON){
        parse_char();
    } else if(symbol_p == LPARENT){
        get_next_token();
        parse_expression();
        if(symbol_p != RPARENT)error_parse();
        get_next_token();
    } else if(symbol_p == IDENFR){
        if(words[loc_f_p + 1]._symbol == LBRACK){
            get_next_token();
            get_next_token();
            parse_expression();
            if(symbol_p != RBRACK)error_parse();
            get_next_token();
            if(symbol_p == LBRACK){
                get_next_token();
                parse_expression();
                if(symbol_p != RBRACK)error_parse();
                get_next_token();
            }
        } else if(words[loc_f_p + 1]._symbol == LPARENT){
            parse_func_call_with_return();
        } else{
            get_next_token();
        }
    } else error_parse();
    t2<<"<因子>"<<std::endl;
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

        if(symbol_p != SEMICN)error_parse();
        get_next_token();
    }
    else if(symbol_p == SCANFTK){
        parse_read_sent();
        if(symbol_p != SEMICN)error_parse();
        get_next_token();
    }
    else if(symbol_p == PRINTFTK){
        parse_print_sent();
        if(symbol_p != SEMICN)error_parse();
        get_next_token();
    }
    else if(symbol_p == SWITCHTK)parse_case_sent();
    else if(symbol_p == SEMICN){
        get_next_token();
    }
    else if(symbol_p == RETURNTK){
        parse_return_sent();
        if(symbol_p != SEMICN)error_parse();
        get_next_token();
    }
    else if(symbol_p == LBRACE){
        get_next_token();
        parse_sent_col();
        if(symbol_p != RBRACE)error_parse();
        get_next_token();
    }
    else error_parse();
    t2<<"<语句>"<<std::endl;
}
void parse_assign_sent(){
    parse_iden();
    if(symbol_p == ASSIGN){
        get_next_token();
        parse_expression();
    } else if(symbol_p == LBRACK){
        get_next_token();
        parse_expression();
        if(symbol_p != RBRACK)error_parse();
        get_next_token();
        if(symbol_p == LBRACK){
            get_next_token();
            parse_expression();
            if(symbol_p != RBRACK)error_parse();
            get_next_token();
            if(symbol_p != ASSIGN)error_parse();
            get_next_token();
            parse_expression();
        } else if(symbol_p == ASSIGN){
            get_next_token();
            parse_expression();
        } else error_parse();
    } else error_parse();
    t2<<"<赋值语句>"<<std::endl;
}
void parse_cond_sent(){
    //t2<<"parse_cond_sent"<<std::endl;
    if(symbol_p != IFTK)error_parse();
    get_next_token();
    if(symbol_p != LPARENT)error_parse();
    get_next_token();
    parse_condition();
    if(symbol_p != RPARENT)error_parse();
    get_next_token();

    parse_sent();
    if(symbol_p == ELSETK){
        get_next_token();
        parse_sent();
    }
    t2<<"<条件语句>"<<std::endl;
}
void parse_condition(){
    //t2<<"parse_condition"<<std::endl;
    parse_expression();
    parse_relation();
    parse_expression();
    t2<<"<条件>"<<std::endl;
}
void parse_loop_sent() {
    if (symbol_p == WHILETK) {
        get_next_token();
        if (symbol_p != LPARENT)error_parse();
        get_next_token();
        parse_condition();
        if (symbol_p != RPARENT)error_parse();
        get_next_token();
        parse_sent();
    } else if (symbol_p == FORTK) {
        get_next_token();
        if (symbol_p != LPARENT)error_parse();
        get_next_token();
        parse_iden();
        if (symbol_p != ASSIGN)error_parse();
        get_next_token();
        parse_expression();
        if (symbol_p != SEMICN)error_parse();
        get_next_token();
        parse_condition();
        if (symbol_p != SEMICN)error_parse();
        get_next_token();
        parse_iden();
        if (symbol_p != ASSIGN)error_parse();
        get_next_token();
        parse_iden();
        parse_plus();
        parse_step_length();
        if (symbol_p != RPARENT)error_parse();
        get_next_token();
        parse_sent();
    } else error_parse();
    t2<<"<循环语句>"<<std::endl;
}
void parse_step_length(){
    parse_unsigned_int();
    t2<<"<步长>"<<std::endl;
}
void parse_case_sent(){
    if(symbol_p != SWITCHTK)error_parse();
    get_next_token();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_expression();

    if(symbol_p != RPARENT)error_parse();
    get_next_token();

    if(symbol_p != LBRACE)error_parse();
    get_next_token();

    parse_case_tab();
    parse_default();

    if(symbol_p != RBRACE)error_parse();
    get_next_token();
    t2<<"<情况语句>"<<std::endl;
}
void parse_case_tab(){
    if(symbol_p != CASETK)error_parse();
    parse_case_sub_sent();

    while(symbol_p == CASETK){
        parse_case_sub_sent();
    }
    t2<<"<情况表>"<<std::endl;
}
void parse_case_sub_sent(){
    if(symbol_p != CASETK)error_parse();
    get_next_token();

    parse_constant();

    if(symbol_p != COLON)error_parse();
    get_next_token();

    parse_sent();
    t2<<"<情况子语句>"<<std::endl;
}
void parse_default(){
    if(symbol_p != DEFAULTTK)error_parse();
    get_next_token();

    if(symbol_p != COLON)error_parse();
    get_next_token();

    parse_sent();
    t2<<"<缺省>"<<std::endl;
}
void parse_func_call_with_return(){
    parse_iden();
    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_val_para_tab();

    if(symbol_p != RPARENT)error_parse();
    get_next_token();
    t2<<"<有返回值函数调用语句>"<<std::endl;
}
void parse_func_call_no_return(){
    parse_iden();
    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_val_para_tab();

    if(symbol_p != RPARENT)error_parse();
    get_next_token();
    t2<<"<无返回值函数调用语句>"<<std::endl;
}
void parse_val_para_tab(){
    if(symbol_p != RPARENT){
        parse_expression();
        while(symbol_p == COMMA){
            get_next_token();
            parse_expression();
        }
    }
    t2<<"<值参数表>"<<std::endl;
}
void parse_sent_col(){
    while(symbol_p != RBRACE){
        parse_sent();
    }
    t2<<"<语句列>"<<std::endl;
}
void parse_read_sent(){
    if(symbol_p != SCANFTK)error_parse();
    get_next_token();

    if(symbol_p != LPARENT)error_parse();
    get_next_token();

    parse_iden();

    if(symbol_p != RPARENT)error_parse();
    get_next_token();
    t2<<"<读语句>"<<std::endl;
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
    if(symbol_p != RPARENT)error_parse();
    get_next_token();
    t2<<"<写语句>"<<std::endl;
}
void parse_return_sent(){
    if(symbol_p != RETURNTK)error_parse();
    get_next_token();

    if(symbol_p == LPARENT){
        get_next_token();
        parse_expression();
        if(symbol_p != RPARENT)error_parse();
        get_next_token();
    }
    t2<<"<返回语句>"<<std::endl;
}

void parse(){
    loc_f_p = 0;
    symbol_p = words[0]._symbol;
    parse_program();
}