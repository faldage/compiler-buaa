#ifndef COMPLIER_GENERATEMIPS_H
#define COMPLIER_GENERATEMIPS_H
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "myVar.h"

extern INTER_SYM sym_g;
extern int loc_g;
extern int symPro_g;
extern IntermediateCode code;
extern std::ofstream output_mips;

void get_next_code();
void generate();
void assString();

void initialize();

void g_assign();
void g_printf();
void g_scanf();
void g_var();
void g_const();
void g_cal();
VarLoc assignNewLoc();
int assignTempReg();
void freeTempReg(std::vector<int> toFree);
void freeVarLoc(std::vector<VarLoc> varLocList);
void moveToTemp(int tempNum, VarLoc varLoc);
void moveFromTemp(VarLoc varLoc, int tempNum);
void move(VarLoc from, VarLoc to);

#endif