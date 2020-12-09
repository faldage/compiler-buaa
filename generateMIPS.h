#ifndef COMPLIER_GENERATEMIPS_H
#define COMPLIER_GENERATEMIPS_H
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "myVar.h"

class VarLoc{
public:
    int loc_type;
    //1:reg 2:mem 3:gp 4:sp;

    int num;

    VarLoc(){
        loc_type = 0;
        num = 0;
    }

    VarLoc(int _type, int _num){
        loc_type = _type;
        num = _num;
    }
};


extern INTER_SYM sym_g;
extern int loc_g;
extern int symPro_g;
extern IntermediateCode code;
extern std::ofstream output_mips;

void get_next_code();
void generate();
void replaceAll(std::string& str, const std::string& from, const std::string& to);
void assString();

void initialize();

void g_assign();
void g_printf();
void g_scanf();
void g_var();
void g_const();
void g_cal();

void g_while();
void g_for();

void g_if();
void g_j();
void g_jr_ra();
void g_func_def();
void g_func_def_end();
void g_func_call();
void g_func_call_end();
void g_label();
void g_switch();
void g_switch_if();
void g_arr_get();
void g_arr_ass();
void g_code();

VarLoc assignNewLoc();
int assignTempReg();
void freeTempReg(std::vector<int> toFree);
void freeVarLoc(std::vector<VarLoc> varLocList);
void moveToTemp(int tempNum, VarLoc varLoc);
void moveFromTemp(VarLoc varLoc, int tempNum);
void move(VarLoc from, VarLoc to);

extern std::map<std::string, VarLoc>globalLocAssign;//变量名对应位置 全局+变量
extern std::map<std::string, VarLoc>funcLocAssign;//函数中变量名对应的位置

extern std::map<int, int>regAssign;//寄存器已分配的情况
extern int gpCount;//gp分配位置
extern int spCount_g;
extern std::string generateFuncName;

#endif