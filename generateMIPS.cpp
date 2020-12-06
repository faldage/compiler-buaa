//
// Created by h on 2020/11/7.
//
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include "parse.h"
#include "myVar.h"
#include "generateMIPS.h"

INTER_SYM sym_g;
int loc_g = 0;
int symPro_g;
IntermediateCode code;
std::ofstream output_mips("mips.txt");

std::map<std::string, VarLoc>mainLocAssign;//变量名对应位置 全局+变量
std::map<std::string, VarLoc>funcLocAssign;//函数中变量名对应的位置

std::map<int, int>regAssign;//寄存器已分配的情况
int gpCount;//gp分配位置
int spCount_g;
std::string generateFuncName = "main";

void get_next_code(){
    loc_g++;
    sym_g = intermediateCodes[loc_g]._interSym;
    symPro_g = intermediateCodes[loc_g]._symProperty;
    code = intermediateCodes[loc_g];
}

void generate(){
    initialize();
    assString();
    output_mips<<".text"<<std::endl;
    loc_g = -1;
    get_next_code();
    while(loc_g < intermediateCodes.size()){
        if(sym_g == I_VAR){
            g_var();
        } else if(sym_g == I_CON){
            g_const();
        } else if(sym_g == I_SCANF){
            g_scanf();
        } else if(sym_g == I_PRINTF){
            g_printf();
        } else if(sym_g == I_ASSIGN){
            g_assign();
        } else if(sym_g == I_CAL){
            g_cal();
        } else if(sym_g == I_WHILE){
            g_while();
        } else if(sym_g == I_FOR){
            g_for();
        } else if(sym_g == I_IF){
            g_if();
        } else if(sym_g == I_J){
            g_j();
        }  else if(sym_g == I_JR_RA){
            g_jr_ra();
        } else if(sym_g == I_FUNC_DEF){
            g_func_def();
        } else if(sym_g == I_FUNC_CALL){
            g_func_call();
        } else if(sym_g == I_LABEL){
            g_label();
        } else if(sym_g == I_SWITCH){
            g_switch();
        } else if(sym_g == I_SWITCH_IF){
            g_switch_if();
        } else if(sym_g == I_ARR_GET){
            g_arr_get();
        } else if(sym_g == I_ARR_ASS){
            g_arr_ass();
        } else if(sym_g == I_CODE){
            g_code();
        } else {
            output_mips<<"error in generate mips!"<<std::endl;
        }
        get_next_code();
    }
}

void initialize(){
    for(int i = 8; i <= 25; i++){
        regAssign[i] = 0;
    }
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void assString(){
    output_mips<<".data"<<std::endl;
    output_mips<<"space: .asciiz \" \""<<std::endl;
    output_mips<<"enter: .asciiz \"\\n\""<<std::endl;
    for(auto &v : stringList){
        output_mips<<"str"<<v.second<<":" <<".asciiz ";
        std::string temp = v.first;
        replaceAll(temp, "\\", "\\\\");
        output_mips<<"\""<<temp<<"\""<<std::endl;
    }
}

void g_assign(){
    std::string nameRes = "t"+ std::to_string(code._assExpResReg);
    if(generateFuncName == "main"){
        move(mainLocAssign[nameRes], mainLocAssign[code._assName]);
    } else {
        move(funcLocAssign[nameRes], funcLocAssign[code._assName]);
    }
}
void g_printf(){
    if(symPro_g == 1 || symPro_g == 3){
        int strNum = stringList[code._priStr];
        output_mips<<"la $a0 str"<<strNum<<std::endl;
        output_mips<<"li $v0 4"<<std::endl;
        output_mips<<"syscall"<<std::endl;
    } else {
        std::string temp = "t" + std::to_string(code._priExpResReg);
        VarLoc varLoc;
        if(generateFuncName == "main"){
            varLoc = mainLocAssign[temp];
        } else {
            varLoc = funcLocAssign[temp];
        }


        if(varLoc.loc_type == 1){
            output_mips<<"move $a0 $"<<varLoc.num<<std::endl;
        } else if(varLoc.loc_type == 3){
            output_mips<<"lw $a0 ";
            output_mips<<varLoc.num<<"($gp)"<<std::endl;
        }

        output_mips<<"li $v0 ";
        if(code._priExpType == INT){
            output_mips<<"1"<<std::endl;
        } else if(code._priExpType == CHAR){
            output_mips<<"11"<<std::endl;
        } else {
            output_mips<<"else type!"<<std::endl;
        }
        output_mips<<"syscall"<<std::endl;
    }
    if(symPro_g != 3){
        output_mips<<"la $a0 enter"<<std::endl;
        output_mips<<"li $v0 4"<<std::endl;
        output_mips<<"syscall"<<std::endl;
    }
}

void g_scanf(){
    output_mips<<"li $v0 ";
    if(code._scType == INT){
        output_mips<<"5"<<std::endl;
    } else if(code._scType == CHAR){
        output_mips<<"12"<<std::endl;
    }
    output_mips<<"syscall"<<std::endl;

    VarLoc varLoc;
    if(generateFuncName == "main"){
        varLoc = mainLocAssign[code._scName];
    }else{
        varLoc = funcLocAssign[code._scName];
    }

    if(varLoc.loc_type == 1){
        output_mips<<"move $"<<varLoc.num<<" $v0"<<std::endl;
    } else if(varLoc.loc_type == 3){
        output_mips<<"sw $v0 "<<varLoc.num<<"($gp)"<<std::endl;
    }
}

void g_var(){
    VarLoc varLoc = assignNewLoc();

    if(generateFuncName == "main"){
        mainLocAssign[code._name] = varLoc;
    } else {
        funcLocAssign[code._name] = varLoc;
    }

    if(varLoc.loc_type == 1){
        output_mips<<"li $"<<varLoc.num<<" ";
        if(code._vcType == INT){
            output_mips<<code._intValue[0]<<std::endl;
        } else{
            output_mips<<int(code._chValue[0])<<std::endl;
        }
    } else if(varLoc.loc_type == 3){
        int temp = assignTempReg();
        std::vector<int>toFree;
        toFree.push_back(temp);
        output_mips<<"li $"<<temp<<" ";
        if(code._vcType == INT){
            output_mips<<code._intValue[0]<<std::endl;
        } else{
            output_mips<<int(code._chValue[0])<<std::endl;
        }
        moveFromTemp(varLoc, temp);
        freeTempReg(toFree);
    }
}

void g_const(){
    VarLoc varLoc = assignNewLoc();

    if(generateFuncName == "main"){
        mainLocAssign[code._name] = varLoc;
    } else {
        funcLocAssign[code._name] = varLoc;
    }

    if(varLoc.loc_type == 1){
        output_mips<<"li $"<<varLoc.num<<" ";
        if(code._vcType == INT){
            output_mips<<code._intValue[0]<<std::endl;
        } else{
            output_mips<<int(code._chValue[0])<<std::endl;
        }
    } else if(varLoc.loc_type == 3){
        int temp = assignTempReg();
        std::vector<int>toFree;
        toFree.push_back(temp);
        output_mips<<"li $"<<temp<<" ";
        if(code._vcType == INT){
            output_mips<<code._intValue[0]<<std::endl;
        } else{
            output_mips<<int(code._chValue[0])<<std::endl;
        }
        moveFromTemp(varLoc, temp);
        freeTempReg(toFree);
    }
}

void g_cal(){
    std::string nameRes = ":"+ std::to_string(code._resRegNum);
    if(generateFuncName == "main"){
        if(mainLocAssign.count(nameRes) == 0){
            mainLocAssign[nameRes] = assignNewLoc();
        }
    } else{
        if(funcLocAssign.count(nameRes) == 0){
            funcLocAssign[nameRes] = assignNewLoc();
        }
    }

    int tempReg1 = assignTempReg();
    int tempReg2 = assignTempReg();
    int tempResReg;
    std::vector<int> toFree;
    toFree.push_back(tempReg1);
    toFree.push_back(tempReg2);
    if(code._resType == 1){
        tempResReg = assignTempReg();
        toFree.push_back(tempResReg);
    } else if(code._resType == 2){
        tempResReg = code._resReg2;
    } else {
        output_mips<<"wrong in cal regReg!"<<std::endl;
    }

    std::string name1, name2;
    if(code._cal1Type == 1 || code._cal1Type == 2){
        if(code._cal1Type == 1){
            name1 = ":" + std::to_string(code._reg1);
        } else if(code._cal1Type == 2){
            name1 = code._iden1;
        }
        if(generateFuncName == "main"){
            moveToTemp(tempReg1, mainLocAssign[name1]);
        } else {
            moveToTemp(tempReg1, funcLocAssign[name1]);
        }

    }

    if(code._cal2Type == 1 || code._cal2Type == 2){
        if(code._cal2Type == 1){
            name2 = ":" + std::to_string(code._reg2);
        } else if(code._cal2Type == 2){
            name2 = code._iden2;
        }
        if(generateFuncName == "main"){
            moveToTemp(tempReg2, mainLocAssign[name2]);
        } else {
            moveToTemp(tempReg2, funcLocAssign[name2]);
        }

    }

    //if(locAssign.count())
    if(code._symProperty == 1){
        output_mips<<"add $"<<tempResReg;
    } else if(code._symProperty == 2){
        output_mips<<"sub $"<<tempResReg;
    } else if(code._symProperty == 3){
        output_mips<<"mul $"<<tempResReg;
    } else if(code._symProperty == 4){
        output_mips<<"div $"<<tempResReg;
    }

    if(code._cal1Type == 3 || (code._cal1Type == 4 && code._int1 != 0)){
        if(code._cal2Type == 1 || code._cal2Type == 2){
            output_mips<< " $"<<tempReg2;
        } else if(code._cal2Type == 3){
            output_mips<< " "<<int(code._ch2);
        } else {
            output_mips<< " "<<code._int2;
        }
        if(code._cal1Type == 3){
            output_mips<< " "<<int(code._ch1)<<std::endl;
        } else {
            output_mips<< " "<<code._int1<<std::endl;
        }
    } else {
        if(code._cal1Type == 4){
            output_mips<<" $0";
        } else {
            output_mips<< " $"<<tempReg1;
        }

        if(code._cal2Type == 1 || code._cal2Type == 2){
            output_mips<< " $"<<tempReg2<<std::endl;
        } else if(code._cal2Type == 3){
            output_mips<< " "<<int(code._ch2)<<std::endl;
        } else {
            output_mips<< " "<<code._int2<<std::endl;
        }
    }

    if(code._resType == 1){
        if(generateFuncName == "main"){
            moveFromTemp(mainLocAssign[nameRes], tempResReg);
        } else {
            moveFromTemp(funcLocAssign[nameRes], tempResReg);
        }
    }

    freeTempReg(toFree);
}

void g_while(){
    output_mips<<code._labelStart<<":"<<std::endl;

    int tempReg1 = assignTempReg();
    int tempReg2 = assignTempReg();
    std::vector<int> toFree;
    toFree.push_back(tempReg1);
    toFree.push_back(tempReg2);
    std::string name1, name2;
    name1 = ":" + std::to_string(code._judgeReg1);
    name2 = ":" + std::to_string(code._judgeReg2);

    if(generateFuncName == "main"){
        moveToTemp(tempReg1, mainLocAssign[name1]);
        moveToTemp(tempReg2, mainLocAssign[name2]);
    } else{
        moveToTemp(tempReg1, funcLocAssign[name1]);
        moveToTemp(tempReg2, funcLocAssign[name2]);
    }


    if(code._judgeType == 1){ //>=:continue else: _labelEnd
        output_mips<<"blt $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else if(code._judgeType == 2){//>
        output_mips<<"ble $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else if(code._judgeType == 3){//<=
        output_mips<<"bgt $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else if(code._judgeType == 4){//<
        output_mips<<"bge $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else if(code._judgeType == 5){//==
        output_mips<<"bne $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else if(code._judgeType == 6){//!=
        output_mips<<"beq $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else{
        output_mips<<"wrong judge type in while!"<<std::endl;
    }
    freeTempReg(toFree);
}

void g_for(){
    int tempReg1 = assignTempReg();
    int tempReg2 = assignTempReg();
    int tempResReg = assignTempReg();
    std::vector<int> toFree;
    toFree.push_back(tempReg1);
    toFree.push_back(tempReg2);
    toFree.push_back(tempResReg);

    std::string name1  = ":" + std::to_string(code._reg1);
    if(generateFuncName == "main"){
        moveToTemp(tempReg1, mainLocAssign[name1]);
    } else {
        moveToTemp(tempReg1, funcLocAssign[name1]);
    }


    output_mips<<"add $"<<tempResReg<< " $"<<tempReg1<<" $0"<<std::endl;

    if(code._resType == 1){
        if(generateFuncName == "main"){
            moveFromTemp(mainLocAssign[code._loopName1], tempResReg);
        } else {
            moveFromTemp(funcLocAssign[code._loopName1], tempResReg);
        }
    }

    output_mips<<code._labelStart<<":"<<std::endl;

    //todo
    int tempReg3 = assignTempReg();
    int tempReg4 = assignTempReg();
    toFree.push_back(tempReg3);
    toFree.push_back(tempReg4);
    std::string name3 = ":" + std::to_string(code._judgeReg1);
    std::string name4 = ":" + std::to_string(code._judgeReg2);

    if(generateFuncName == "main"){
        moveToTemp(tempReg1, mainLocAssign[name3]);
        moveToTemp(tempReg2, mainLocAssign[name4]);
    } else{
        moveToTemp(tempReg1, funcLocAssign[name3]);
        moveToTemp(tempReg2, funcLocAssign[name4]);
    }


    if(code._judgeType == 1){ //>=:continue else: _labelEnd
        output_mips<<"blt $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else if(code._judgeType == 2){//>
        output_mips<<"ble $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else if(code._judgeType == 3){//<=
        output_mips<<"bgt $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else if(code._judgeType == 4){//<
        output_mips<<"bge $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else if(code._judgeType == 5){//==
        output_mips<<"bne $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else if(code._judgeType == 6){//!=
        output_mips<<"beq $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<<std::endl;
    } else{
        output_mips<<"wrong judge type in for!"<<std::endl;
    }
    freeTempReg(toFree);
}

void g_if(){
    int tempReg1 = assignTempReg();
    int tempReg2 = assignTempReg();
    std::vector<int> toFree;
    toFree.push_back(tempReg1);
    toFree.push_back(tempReg2);
    std::string name1, name2;
    name1 = ":" + std::to_string(code._judgeReg1);
    name2 = ":" + std::to_string(code._judgeReg2);

    if(generateFuncName == "main"){
        moveToTemp(tempReg1, mainLocAssign[name1]);
        moveToTemp(tempReg2, mainLocAssign[name2]);
    } else{
        moveToTemp(tempReg1, funcLocAssign[name1]);
        moveToTemp(tempReg2, funcLocAssign[name2]);
    }

    if(code._judgeType == 1){ //>=:continue else: _labelEnd
        output_mips<<"blt $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._endLabel<<std::endl;
    } else if(code._judgeType == 2){//>
        output_mips<<"ble $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._endLabel<<std::endl;
    } else if(code._judgeType == 3){//<=
        output_mips<<"bgt $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._endLabel<<std::endl;
    } else if(code._judgeType == 4){//<
        output_mips<<"bge $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._endLabel<<std::endl;
    } else if(code._judgeType == 5){//==
        output_mips<<"bne $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._endLabel<<std::endl;
    } else if(code._judgeType == 6){//!=
        output_mips<<"beq $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._endLabel<<std::endl;
    } else{
        output_mips<<"wrong judge type in if! = "<<code._judgeType<<std::endl;
    }
    freeTempReg(toFree);
}

void g_j(){
    output_mips<<"j label"<<std::to_string(code._jLabelNum)<<std::endl;
}

void g_jr_ra(){
    output_mips<<"jr $ra"<<std::endl;
}

void g_func_def(){
    generateFuncName = code._funcDefName;
    int length = globalSigTab[myTolower(generateFuncName)]._spaceCount;
    length+=4;
    output_mips<<generateFuncName<<":"<<std::endl;
    output_mips<<"sub $sp $sp "<<length<<std::endl;
    output_mips<<"sw $ra 0($sp)"<<std::endl;

    int temp = 0;
    std::vector<SIG_SYM> temp1 = globalSigTab[myTolower(generateFuncName)]._para_tab;
    std::vector<std::string >temp2 = globalSigTab[myTolower(generateFuncName)]._para_tab_name;
    for(int i = 0; i < temp1.size(); i++){
        funcLocAssign[temp2[i]] = VarLoc(4, length + temp * 4);
    }
}

void g_func_call(){
    int temp = globalSigTab[myTolower(code._funcCallName)]._para_tab.size();
    output_mips<<"sub $sp $sp "<<temp * 4<<std::endl;

    int tempReg = assignTempReg();
    std::vector<int> toFree;
    toFree.push_back(tempReg);
    for(int i = 0; i < temp; i++){
        std::string name = ":" + std::to_string(code._paraRegNum[i]);
        if(generateFuncName == "main"){
            moveToTemp(tempReg, mainLocAssign[name]);
        } else {
            moveToTemp(tempReg, funcLocAssign[name]);
        }
        VarLoc varLoc = VarLoc(4, i * 4);
        moveFromTemp(varLoc, tempReg);
    }

    output_mips<<"jal "<<code._funcCallName<<std::endl;
    freeTempReg(toFree);
}

void g_label(){
    output_mips<<"label"<<code._labelName<<":"<<std::endl;
}

void g_switch(){
//no operation
}

void g_switch_if(){
    int tempReg1 = assignTempReg();
    std::vector<int> toFree;
    toFree.push_back(tempReg1);
    std::string name1;
    name1 = ":" + std::to_string(code._switchReg);

    if(generateFuncName == "main"){
        moveToTemp(tempReg1, mainLocAssign[name1]);
    } else{
        moveToTemp(tempReg1, funcLocAssign[name1]);
    }

    if(code._swType == INT){
        output_mips<<"bne $"<<tempReg1<<" "<<code._intValue[0]<<" label"<<code._nextIfLabel<<std::endl;
    } else {
        output_mips<<"bne $"<<tempReg1<<" "<<code._chValue[0]<<" label"<<code._nextIfLabel<<std::endl;
    }
    freeTempReg(toFree);
}

void g_arr_get(){
    int tempReg1 = assignTempReg();
    int tempReg2 = assignTempReg();
    int tempReg3 = assignTempReg();

    std::vector<int>toFree;
    toFree.push_back(tempReg1);
    toFree.push_back(tempReg2);
    toFree.push_back(tempReg3);

    std::string name0 = code._name;
    std::string name1 = ":" + std::to_string(code._loc1);
    std::string name2 = ":" + std::to_string(code._loc2);
    std::string name3 = ":" + std::to_string(code._ansNum);

    VarLoc varLoc0, varLoc1, varLoc2, varLoc3;
    if(generateFuncName == "main"){
        varLoc0 = mainLocAssign[myTolower(name0)];
        varLoc1 = mainLocAssign[myTolower(name1)];
        varLoc2 = mainLocAssign[myTolower(name2)];
        if(mainLocAssign.count(name3) == 0){
            mainLocAssign[name3] = assignNewLoc();
        }
        varLoc3 = mainLocAssign[name3];
    } else {
        varLoc0 = funcLocAssign[myTolower(name0)];
        varLoc1 = funcLocAssign[myTolower(name1)];
        varLoc2 = funcLocAssign[myTolower(name2)];
        if(funcLocAssign.count(name3) == 0){
            funcLocAssign[name3] = assignNewLoc();
        }
        varLoc3 = funcLocAssign[name3];
    }

    moveToTemp(tempReg1, varLoc1);
    moveToTemp(tempReg2, varLoc2);

    output_mips<<"mul $"<<tempReg3<<" $"<<tempReg1<<" "<<code._length<<std::endl;
    //mul t2 t1 length

    output_mips<<"add $"<<tempReg3<<" $"<<tempReg3<<" $"<<tempReg2<<std::endl;
    output_mips<<"sll $"<<tempReg3<<" $"<<tempReg3<<" "<<2 << std::endl;
    output_mips<<"add $"<<tempReg3<<" $"<<tempReg3<<" "<<varLoc0.num<<std::endl;

    output_mips<<"add $"<<tempReg3<<" $"<<tempReg3<<" $sp"<<std::endl;

    if(code._vcType == INT){
        output_mips<<"lw $"<<tempReg1<<" 0($"<<tempReg3<<")"<<std::endl;
    } else if(code._vcType == CHAR){
        output_mips<<"lb $"<<tempReg1<<" 0($"<<tempReg3<<")"<<std::endl;
    } else {
        output_mips<<"wrong in ass arr!!!"<<std::endl;
    }

    moveFromTemp(varLoc3, tempReg1);
    freeTempReg(toFree);
}

void g_arr_ass(){
    int tempReg1 = assignTempReg();
    int tempReg2 = assignTempReg();
    int tempReg3 = assignTempReg();

    std::vector<int>toFree;
    toFree.push_back(tempReg1);
    toFree.push_back(tempReg2);
    toFree.push_back(tempReg3);

    std::string name0 = code._name;
    std::string name1 = ":" + std::to_string(code._loc1);
    std::string name2 = ":" + std::to_string(code._loc2);
    std::string name3 = ":" + std::to_string(code._arr_ass_regNum);

    VarLoc varLoc0, varLoc1, varLoc2, varLoc3;
    if(generateFuncName == "main"){
        varLoc0 = mainLocAssign[myTolower(name0)];
        varLoc1 = mainLocAssign[myTolower(name1)];
        varLoc2 = mainLocAssign[myTolower(name2)];
        if(mainLocAssign.count(name3) == 0){
            mainLocAssign[name3] = assignNewLoc();
        }
        varLoc3 = mainLocAssign[name3];
    } else {
        varLoc0 = funcLocAssign[myTolower(name0)];
        varLoc1 = funcLocAssign[myTolower(name1)];
        varLoc2 = funcLocAssign[myTolower(name2)];
        if(funcLocAssign.count(name3) == 0){
            funcLocAssign[name3] = assignNewLoc();
        }
        varLoc3 = funcLocAssign[name3];
    }

    moveToTemp(tempReg1, varLoc1);
    moveToTemp(tempReg2, varLoc2);

    output_mips<<"mul $"<<tempReg3<<" $"<<tempReg1<<" "<<code._length<<std::endl;
    //mul t2 t1 length

    output_mips<<"add $"<<tempReg3<<" $"<<tempReg3<<" $"<<tempReg2<<std::endl;
    output_mips<<"sll $"<<tempReg3<<" $"<<tempReg3<<" "<<2<<std::endl;
    output_mips<<"add $"<<tempReg3<<" $"<<tempReg3<<" "<<varLoc0.num<<std::endl;

    output_mips<<"add $"<<tempReg3<<" $"<<tempReg3<<" $sp"<<std::endl;

    if(code._assType == INT){
        output_mips<<"sw $"<<tempReg1<<" 0($"<<tempReg3<<")"<<std::endl;
    } else if(code._assType == CHAR){
        output_mips<<"sb $"<<tempReg1<<" 0($"<<tempReg3<<")"<<std::endl;
    }

    moveFromTemp(varLoc3, tempReg1);
    freeTempReg(toFree);
}

void g_code(){
    output_mips<<code._code<<std::endl;
}


VarLoc assignNewLoc(){
    VarLoc varLoc = VarLoc();
    if(generateFuncName == "main"){
        if(gpCount != 0){
            varLoc.loc_type = 3;
            varLoc.num = gpCount;
            gpCount+=4;
            return varLoc;
        } else {
            for(int i = 16; i<= 23; i++){
                if(regAssign[i] == 0){
                    regAssign[i] = 1;
                    varLoc.loc_type = 1;
                    varLoc.num = i;
                    return varLoc;
                }
            }
        }
        varLoc.loc_type = 3;
        varLoc.num = gpCount;
        gpCount+=4;
    } else {
        varLoc.loc_type = 4;
        varLoc.num = spCount_g;
        spCount_g++;
    }
    return varLoc;
}

int assignTempReg(){
    for(int i = 8; i <= 15; i++){
        if(regAssign[i] == 0){
            regAssign[i] = 1;
            return i;
        }
    }
    if(regAssign[24] == 0){
        regAssign[24] = 1;
        return 24;
    } else if(regAssign[25] == 0){
        regAssign[25] = 1;
        return 25;
    }
}

void freeTempReg(std::vector<int> toFree){
    for(int i : toFree){
        regAssign[i] = 0;
    }
}

void freeVarLoc(std::vector<VarLoc> varLocList){

}

void moveToTemp(int tempNum, VarLoc varLoc){
    if(varLoc.loc_type == 1){
        output_mips<<"move $" <<tempNum<<" $"<<varLoc.num<<std::endl;
    } else if(varLoc.loc_type == 3){
        output_mips<<"lw $"<<tempNum<<" "<<varLoc.num<<"($gp)"<<std::endl;
    }
}

void moveFromTemp(VarLoc varLoc, int tempNum){
    if(varLoc.loc_type == 1){
        output_mips<<"move $" <<varLoc.num<<" $"<<tempNum<<std::endl;
    } else if(varLoc.loc_type == 3){
        output_mips<<"sw $"<<tempNum<<" "<<varLoc.num<<"($gp)"<<std::endl;
    }
}

void move(VarLoc from, VarLoc to){
    int temp = assignTempReg();

    std::vector<int> toFree;
    toFree.push_back(temp);

    moveToTemp(temp, from);
    moveFromTemp(to, temp);

    freeTempReg(toFree);
}



