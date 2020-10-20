#ifndef BY2_PARSE_H
#define BY2_PARSE_H

#include <string>
#include <vector>
#include <map>
#include "myVar.h"

extern int loc_f_p;
extern SYMBOL symbol_p;
extern std::ofstream t2;
extern std::map<std::string, SYMBOL> func_tab;

void error_parse();
void get_next_token();

void parse_plus();
void parse_mult();
void parse_relation();
void parse_num();
void parse_char();
void parse_string();
void parse_program();
void parse_const_explain();
void parse_const_def();
void parse_unsigned_int();
void parse_int();
void parse_iden();
void parse_statement_head();
void parse_constant();
void parse_var_explain();
void parse_var_def();
void parse_var_def_no_initial();
void parse_var_def_and_initial();
void parse_type_iden();
void parse_func_with_return_def();
void parse_func_no_return_def();
void parse_compound_sent();
void parse_para_tab();
void parse_main();
void parse_expression();
void parse_item();
void parse_factor();
void parse_sent();
void parse_assign_sent();
void parse_cond_sent();
void parse_condition();
void parse_loop_sent();
void parse_step_length();
void parse_case_sent();
void parse_case_tab();
void parse_case_sub_sent();
void parse_default();
void parse_func_call_with_return();
void parse_func_call_no_return();
void parse_val_para_tab();
void parse_sent_col();
void parse_read_sent();
void parse_print_sent();
void parse_return_sent();

void parse();

void myPrint(std::string str);

#endif