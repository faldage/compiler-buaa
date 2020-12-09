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

std::map<std::string, VarLoc>globalLocAssign;//变量名对应位置 全局+变量
std::map<std::string, VarLoc>funcLocAssign;//函数中变量名对应的位置

std::map<int, int>regAssign;//寄存器已分配的情况
int gpCount = 0;//gp分配位置
int spCount_g = 4;
std::string generateFuncName = ":";

void get_next_code(){
    loc_g++;
    sym_g = intermediateCodes[loc_g]._interSym;
    symPro_g = intermediateCodes[loc_g]._symProperty;
    code = intermediateCodes[loc_g];
}

void generate(){
    initialize();
    assString();
    output_mips<<".text"<< "# at " << __LINE__ << std::endl;
    loc_g = -1;
    get_next_code();
    while(loc_g < intermediateCodes.size()){
        //output_mips<<"==========================="<<std::endl;
        if(sym_g == I_VAR){
            //output_mips<<"var"<< "# at " << __LINE__ << std::endl;
            g_var();
        } else if(sym_g == I_CON){
            //output_mips<<"con"<< "# at " << __LINE__ << std::endl;
            g_const();
        } else if(sym_g == I_SCANF){
            //output_mips<<"scanf"<< "# at " << __LINE__ << std::endl;
            g_scanf();
        } else if(sym_g == I_PRINTF){
            //output_mips<<"print"<< "# at " << __LINE__ << std::endl;
            g_printf();
        } else if(sym_g == I_ASSIGN){
            //output_mips<<"assign"<< "# at " << __LINE__ << std::endl;
            g_assign();
        } else if(sym_g == I_CAL){
            //output_mips<<"cal"<< "# at " << __LINE__ << std::endl;
            g_cal();
        } else if(sym_g == I_WHILE){
            //output_mips<<"while"<< "# at " << __LINE__ << std::endl;
            g_while();
        } else if(sym_g == I_FOR){
            //output_mips<<"for"<< "# at " << __LINE__ << std::endl;
            g_for();
        } else if(sym_g == I_IF){
            //output_mips<<"if"<< "# at " << __LINE__ << std::endl;
            g_if();
        } else if(sym_g == I_J){
            //output_mips<<"j"<< "# at " << __LINE__ << std::endl;
            g_j();
        }  else if(sym_g == I_JR_RA){
            //output_mips<<"jr ra"<< "# at " << __LINE__ << std::endl;
            g_jr_ra();
        } else if(sym_g == I_FUNC_DEF){
            //output_mips<<"def"<< "# at " << __LINE__ << std::endl;
            g_func_def();
        } else if(sym_g == I_FUNC_DEF_END){
            //output_mips<<"func_def_end"<< "# at " << __LINE__ << std::endl;
            g_func_def_end();
        } else if(sym_g == I_FUNC_CALL){
            //output_mips<<"func_call"<< "# at " << __LINE__ << std::endl;
            g_func_call();
        } else if(sym_g == I_FUNC_CALL_END){
            //output_mips<<"func_call_end"<< "# at " << __LINE__ << std::endl;
            g_func_call_end();
        } else if(sym_g == I_LABEL){
            //output_mips<<"label"<< "# at " << __LINE__ << std::endl;
            g_label();
        } else if(sym_g == I_SWITCH){
            //output_mips<<"switch"<< "# at " << __LINE__ << std::endl;
            g_switch();
        } else if(sym_g == I_SWITCH_IF){
            //output_mips<<"switch_if"<< "# at " << __LINE__ << std::endl;
            g_switch_if();
        } else if(sym_g == I_ARR_GET){
            //output_mips<<"arrget"<< "# at " << __LINE__ << std::endl;
            g_arr_get();
        } else if(sym_g == I_ARR_ASS){
            //output_mips<<"arrass"<< "# at " << __LINE__ << std::endl;
            g_arr_ass();
        } else if(sym_g == I_CODE){
            //output_mips<<"code"<< "# at " << __LINE__ << std::endl;
            g_code();
        } else {
           std::cout<<"error in generate mips!"<< "# at " << __LINE__ << std::endl;
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
    output_mips<<".data"<< "# at " << __LINE__ << std::endl;
    output_mips<<"space: .asciiz \" \""<< "# at " << __LINE__ << std::endl;
    output_mips<<"enter: .asciiz \"\\n\""<< "# at " << __LINE__ << std::endl;
    for(auto &v : stringList){
        output_mips<<"str"<<v.second<<":" <<".asciiz ";
        std::string temp = v.first;
        replaceAll(temp, "\\", "\\\\");
        output_mips<<"\""<<temp<<"\""<< "# at " << __LINE__ << std::endl;
    }
}

void g_assign(){
    std::string nameRes = ":"+ std::to_string(code._assExpResReg);

    if(funcLocAssign.count(code._assName) != 0){
        move(funcLocAssign[nameRes], funcLocAssign[code._assName]);
    } else {
        if(generateFuncName == ":"){
            move(globalLocAssign[myTolower(nameRes)], globalLocAssign[myTolower(code._assName)]);
        } else {
            move(funcLocAssign[myTolower(nameRes)], globalLocAssign[myTolower(code._assName)]);
        }
    }

}

void g_printf(){
    if(symPro_g == 1 || symPro_g == 3){
        int strNum = stringList[code._priStr];
        output_mips<<"la $a0 str"<<strNum<< "# at " << __LINE__ << std::endl;
        output_mips<<"li $v0 4"<< "# at " << __LINE__ << std::endl;
        output_mips<<"syscall"<< "# at " << __LINE__ << std::endl;
    } else {
        std::string temp = ":" + std::to_string(code._priExpResReg);
        VarLoc varLoc;

        if(generateFuncName == ":"){
            varLoc = globalLocAssign[myTolower(temp)];
        } else {
            varLoc = funcLocAssign[temp];
        }


        if(varLoc.loc_type == 1){
            output_mips<<"move $a0 $"<<varLoc.num<< "# at " << __LINE__ << std::endl;
        } else if(varLoc.loc_type == 3){
            output_mips<<"lw $a0 ";
            output_mips<<varLoc.num<<"($gp)"<< "# at " << __LINE__ << std::endl;
        } else if(varLoc.loc_type == 4){
            output_mips<<"lw $a0 ";
            output_mips<<varLoc.num<<"($sp)"<< "# at " << __LINE__ << std::endl;
        }

        output_mips<<"li $v0 ";
        if(code._priExpType == INT){
            output_mips<<"1"<< "# at " << __LINE__ << std::endl;
        } else if(code._priExpType == CHAR){
            output_mips<<"11"<< "# at " << __LINE__ << std::endl;
        } else {
            output_mips<<"else type!"<< "# at " << __LINE__ << std::endl;
        }
        output_mips<<"syscall"<< "# at " << __LINE__ << std::endl;
    }
    if(symPro_g != 3){
        output_mips<<"la $a0 enter"<< "# at " << __LINE__ << std::endl;
        output_mips<<"li $v0 4"<< "# at " << __LINE__ << std::endl;
        output_mips<<"syscall"<< "# at " << __LINE__ << std::endl;
    }
}

void g_scanf(){
    output_mips<<"li $v0 ";
    if(code._scType == INT){
        output_mips<<"5"<< "# at " << __LINE__ << std::endl;
    } else if(code._scType == CHAR){
        output_mips<<"12"<< "# at " << __LINE__ << std::endl;
    }
    output_mips<<"syscall"<< "# at " << __LINE__ << std::endl;

    VarLoc varLoc;
    if(funcLocAssign.count(code._scName) != 0){
        varLoc = funcLocAssign[code._scName];
    } else if(globalLocAssign.count(code._scName) != 0){
        varLoc = globalLocAssign[code._scName];
    } else {
        output_mips<<"scanf not found"<< "# at " << __LINE__ << std::endl;
    }

    if(varLoc.loc_type == 1){
        output_mips<<"move $"<<varLoc.num<<" $v0"<< "# at " << __LINE__ << std::endl;
    } else if(varLoc.loc_type == 3){
        output_mips<<"sw $v0 "<<varLoc.num<<"($gp)"<< "# at " << __LINE__ << std::endl;
    } else if(varLoc.loc_type == 4){
        output_mips<<"sw $v0 "<<varLoc.num<<"($sp)"<< "# at " << __LINE__ << std::endl;
    }
}

void g_var(){
    VarLoc varLoc = assignNewLoc();
    if(generateFuncName == ":"){
        globalLocAssign[code._name] = varLoc;
    } else {
        funcLocAssign[code._name] = varLoc;
    }
    std::cout<<code._name<<" 's loc is in "<<varLoc.num<<", and type is"<<varLoc.loc_type<<std::endl;

    int tempC;
    if(code._symProperty == 0) {
        tempC = 1;
    } else if(code._symProperty == 1){
        tempC = code._dem1;
    } else if(code._symProperty == 2){
        tempC = code._dem1 * code._dem2;
    } else {
        output_mips<<"error in var arr dem =2 "<< "# at " << __LINE__ << std::endl;
    }

    for(int i = 0; i < tempC - 1; i++){
        assignNewLoc();
    }

    if(code._initialized == 1){
        if(varLoc.loc_type == 1){
            output_mips<<"li $"<<varLoc.num<<" ";
            if(code._vcType == INT){
                output_mips<<code._intValue[0]<< "# at " << __LINE__ << std::endl;
            } else{
                output_mips<<int(code._chValue[0])<< "# at " << __LINE__ << std::endl;
            }
        } else if(varLoc.loc_type == 3 || varLoc.loc_type == 4){
            int temp = assignTempReg();
            std::vector<int>toFree;
            toFree.push_back(temp);
            for(int i = 0; i < tempC; i++){
                output_mips<<"li $"<<temp<<" ";
                if(code._vcType == INT){
                    output_mips<<code._intValue[i]<< "# at " << __LINE__ << std::endl;
                } else{
                    output_mips<<int(code._chValue[i])<< "# at " << __LINE__ << std::endl;
                }
                moveFromTemp(VarLoc(varLoc.loc_type, varLoc.num + i * 4), temp);
            }
            freeTempReg(toFree);
        }
    }

}

void g_const(){
    VarLoc varLoc = assignNewLoc();
    if(generateFuncName == ":"){
        globalLocAssign[code._name] = varLoc;
    } else {
        funcLocAssign[code._name] = varLoc;
    }

    if(varLoc.loc_type == 1){
        output_mips<<"li $"<<varLoc.num<<" ";
        if(code._vcType == INT){
            output_mips<<code._intValue[0]<< "# at " << __LINE__ << std::endl;
        } else{
            output_mips<<int(code._chValue[0])<< "# at " << __LINE__ << std::endl;
        }
    } else if(varLoc.loc_type == 3 || varLoc.loc_type == 4){
        int temp = assignTempReg();
        std::vector<int>toFree;
        toFree.push_back(temp);
        output_mips<<"li $"<<temp<<" ";
        if(code._vcType == INT){
            output_mips<<code._intValue[0]<< "# at " << __LINE__ << std::endl;
        } else{
            output_mips<<int(code._chValue[0])<< "# at " << __LINE__ << std::endl;
        }
        moveFromTemp(varLoc, temp);
        freeTempReg(toFree);
    }
}

void g_cal(){
    std::string nameRes = ":"+ std::to_string(code._resRegNum);
    if(funcLocAssign.count(nameRes) == 0){
        funcLocAssign[nameRes] = assignNewLoc();
    }

    std::vector<int> toFree;

    int tempReg1;
    int tempReg2;
    if(code._cal1Type == 5){
        tempReg1 = code._sReg1;
    } else {
        tempReg1 = assignTempReg();
        toFree.push_back(tempReg1);

    }
    if(code._cal2Type == 5){
        tempReg2 = code._sReg2;
    } else {
        tempReg2 = assignTempReg();
        toFree.push_back(tempReg2);
    }
    int tempResReg;

    if(code._resType == 1){
        tempResReg = assignTempReg();
        toFree.push_back(tempResReg);
    } else if(code._resType == 2){
        tempResReg = code._resReg2;
    } else {
        output_mips<<"wrong in cal regReg!"<< "# at " << __LINE__ << std::endl;
    }

    std::string name1, name2;
    if(code._cal1Type == 1 || code._cal1Type == 2){
        if(code._cal1Type == 1){
            name1 = ":" + std::to_string(code._reg1);
        } else if(code._cal1Type == 2){
            name1 = code._iden1;
        }

        if(generateFuncName == ":"){
            moveToTemp(tempReg1, globalLocAssign[name1]);
        } else {
            if(code._cal1Type == 1){
                moveToTemp(tempReg1, funcLocAssign[name1]);
            } else {
                if(funcLocAssign.count(name1) != 0){
                    moveToTemp(tempReg1, funcLocAssign[name1]);
                } else if(globalLocAssign.count(name1) != 0){
                    moveToTemp(tempReg1, globalLocAssign[name1]);
                } else {
                    output_mips<<"!!!!!!!!!!!!!!!!!"<< "# at " << __LINE__ << std::endl;
                }
            }
        }

    }

    if(code._cal2Type == 1 || code._cal2Type == 2){
        if(code._cal2Type == 1){
            name2 = ":" + std::to_string(code._reg2);
        } else if(code._cal2Type == 2){
            name2 = code._iden2;
        }
        if(generateFuncName == ":"){
            moveToTemp(tempReg2, globalLocAssign[name2]);
        } else {
            if(code._cal2Type == 1){
                moveToTemp(tempReg2, funcLocAssign[name2]);
            } else {
                if(funcLocAssign.count(name2) != 0){
                    moveToTemp(tempReg2, funcLocAssign[name2]);
                } else if(globalLocAssign.count(name2) != 0){
                    moveToTemp(tempReg2, globalLocAssign[name2]);
                } else {
                    output_mips<<"!!!!!!!!!!!!!!!!!22222"<< "# at " << __LINE__ << std::endl;
                }
            }
        }

    }

    //if(locAssign.count())
    if(code._symProperty == 1){
        output_mips<<"addu $"<<tempResReg;
    } else if(code._symProperty == 2){
        output_mips<<"subu $"<<tempResReg;
    } else if(code._symProperty == 3){
        output_mips<<"mul $"<<tempResReg;
    } else if(code._symProperty == 4){
        output_mips<<"div $"<<tempResReg;
    } else {
        output_mips<<"wrong cal type!"<<std::endl;
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
            output_mips<< " "<<int(code._ch1)<< "# at " << __LINE__ << std::endl;
        } else {
            output_mips<< " "<<code._int1<< "# at " << __LINE__ << std::endl;
        }
    } else {
        if(code._cal1Type == 4){
            output_mips<<" $0";
        } else {
            output_mips<< " $"<<tempReg1;
        }

        if(code._cal2Type == 1 || code._cal2Type == 2){
            output_mips<< " $"<<tempReg2<< "# at " << __LINE__ << std::endl;
        } else if(code._cal2Type == 3){
            output_mips<< " "<<int(code._ch2)<< "# at " << __LINE__ << std::endl;
        } else {
            output_mips<< " "<<code._int2<< "# at " << __LINE__ << std::endl;
        }
    }

    if(code._resType == 1){
        if(generateFuncName == ":"){
            moveFromTemp(globalLocAssign[nameRes], tempResReg);
        } else {
            moveFromTemp(funcLocAssign[nameRes], tempResReg);
        }
    } else if(code._resType == 2){
        output_mips<<"move $" <<code._resReg2<<" $"<<tempResReg<< "# at " << __LINE__ << std::endl;
    }

    freeTempReg(toFree);
}

void g_while(){

    int tempReg1 = assignTempReg();
    int tempReg2 = assignTempReg();
    std::vector<int> toFree;
    toFree.push_back(tempReg1);
    toFree.push_back(tempReg2);
    std::string name1, name2;
    name1 = ":" + std::to_string(code._judgeReg1);
    name2 = ":" + std::to_string(code._judgeReg2);

    if(generateFuncName == ":"){
        moveToTemp(tempReg1, globalLocAssign[name1]);
        moveToTemp(tempReg2, globalLocAssign[name2]);
    } else{
        moveToTemp(tempReg1, funcLocAssign[name1]);
        moveToTemp(tempReg2, funcLocAssign[name2]);
    }


    if(code._judgeType == 1){ //>=:continue else: _labelEnd
        output_mips<<"blt $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else if(code._judgeType == 2){//>
        output_mips<<"ble $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else if(code._judgeType == 3){//<=
        output_mips<<"bgt $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else if(code._judgeType == 4){//<
        output_mips<<"bge $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else if(code._judgeType == 5){//==
        output_mips<<"bne $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else if(code._judgeType == 6){//!=
        output_mips<<"beq $"<<tempReg1<<" $"<<tempReg2<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else{
        output_mips<<"wrong judge type in while!"<< "# at " << __LINE__ << std::endl;
    }
    freeTempReg(toFree);
}

void g_for(){
    std::vector<int> toFree;
    //todo
    int tempReg3 = assignTempReg();
    int tempReg4 = assignTempReg();
    toFree.push_back(tempReg3);
    toFree.push_back(tempReg4);
    std::string name3 = ":" + std::to_string(code._judgeReg1);
    std::string name4 = ":" + std::to_string(code._judgeReg2);

    if(generateFuncName == ":"){
        moveToTemp(tempReg3, globalLocAssign[name3]);
        moveToTemp(tempReg4, globalLocAssign[name4]);
    } else{
        moveToTemp(tempReg3, funcLocAssign[name3]);
        moveToTemp(tempReg4, funcLocAssign[name4]);
    }


    if(code._judgeType == 1){ //>=:continue else: _labelEnd
        output_mips<<"blt $"<<tempReg3<<" $"<<tempReg4<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else if(code._judgeType == 2){//>
        output_mips<<"ble $"<<tempReg3<<" $"<<tempReg4<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else if(code._judgeType == 3){//<=
        output_mips<<"bgt $"<<tempReg3<<" $"<<tempReg4<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else if(code._judgeType == 4){//<
        output_mips<<"bge $"<<tempReg3<<" $"<<tempReg4<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else if(code._judgeType == 5){//==
        output_mips<<"bne $"<<tempReg3<<" $"<<tempReg4<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else if(code._judgeType == 6){//!=
        output_mips<<"beq $"<<tempReg3<<" $"<<tempReg4<<" label"<<code._labelEnd<< "# at " << __LINE__ << std::endl;
    } else{
        output_mips<<"wrong judge type in for!"<< "# at " << __LINE__ << std::endl;
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

    if(generateFuncName == ":"){
        moveToTemp(tempReg1, globalLocAssign[name1]);
        moveToTemp(tempReg2, globalLocAssign[name2]);
    } else{
        moveToTemp(tempReg1, funcLocAssign[name1]);
        moveToTemp(tempReg2, funcLocAssign[name2]);
    }

    if(code._judgeType == 1){ //>=:continue else: _labelEnd
        output_mips<<"blt ";
    } else if(code._judgeType == 2){//>
        output_mips<<"ble ";
    } else if(code._judgeType == 3){//<=
        output_mips<<"bgt ";
    } else if(code._judgeType == 4){//<
        output_mips<<"bge ";
    } else if(code._judgeType == 5){//==
        output_mips<<"bne ";
    } else if(code._judgeType == 6){//!=
        output_mips<<"beq ";
    } else{
        output_mips<<"wrong judge type in if! = "<<code._judgeType<< "# at " << __LINE__ << std::endl;
    }

    output_mips<<"$"<<tempReg1<<" $"<<tempReg2<<" label";
    if(code._symProperty == 0){
        output_mips<<code._endLabel<< "# at " << __LINE__ << std::endl;
    } else if (code._symProperty == 1){
        output_mips<<code._elseLabel<< "# at " << __LINE__ << std::endl;
    }
    freeTempReg(toFree);
}

void g_j(){
    output_mips<<"j label"<<std::to_string(code._jLabelNum)<< "# at " << __LINE__ << std::endl;
}

void g_jr_ra(){
    output_mips<<"jr $ra"<< "# at " << __LINE__ << std::endl;
}

void g_func_def(){
    funcLocAssign.clear();
    spCount_g = 4;

    generateFuncName = code._funcDefName;
    int length = globalSigTab[myTolower(generateFuncName)]._spaceCount * 4;
    length += 4;
    output_mips<<generateFuncName<<":"<< "# at " << __LINE__ << std::endl;
    output_mips<<"subu $sp $sp "<<length<< "# at " << __LINE__ << std::endl;
    output_mips<<"sw $ra 0($sp)"<< "# at " << __LINE__ << std::endl;

    std::vector<SIG_SYM> temp1 = globalSigTab[myTolower(generateFuncName)]._para_tab;
    std::vector<std::string >temp2 = globalSigTab[myTolower(generateFuncName)]._para_tab_name;
    for(int i = 0; i < temp1.size(); i++){
        funcLocAssign[temp2[i]] = VarLoc(4, length + i * 4);
    }
}

void g_func_def_end(){
    int length = globalSigTab[myTolower(generateFuncName)]._spaceCount * 4;
    length+=4;
    output_mips<<"lw $ra 0($sp)"<< "# at " << __LINE__ << std::endl;
    output_mips<<"addu $sp $sp "<<length<< "# at " << __LINE__ << std::endl;
    output_mips<<"jr $ra"<<std::endl;
}

void g_func_call(){
    int temp = globalSigTab[myTolower(code._funcCallName)]._para_tab.size();
    std::vector<std::string> tempNameTeb = globalSigTab[myTolower(code._funcCallName)]._para_tab_name;
    output_mips<<"subu $sp $sp "<<temp * 4<< "# at " << __LINE__ << std::endl;

    int needSpace = globalSigTab[myTolower(code._funcCallName)]._spaceCount * 4;

    std::cout<<"int func "<<code._funcCallName<<"needSpace = "<<needSpace<<std::endl;

    int tempReg = assignTempReg();
    std::vector<int> toFree;
    toFree.push_back(tempReg);
    for(int i = 0; i < temp; i++){
        std::string name = ":" + std::to_string(code._paraRegNum[i]);
        VarLoc tempVarLoc;
        if(generateFuncName == ":"){
            tempVarLoc = globalLocAssign[name];
        } else {
            tempVarLoc = funcLocAssign[name];
        }

        if(tempVarLoc.loc_type == 4){
            tempVarLoc.num += temp * 4;
        }

        moveToTemp(tempReg, tempVarLoc);
        VarLoc varLoc = VarLoc(4, i * 4);
        moveFromTemp(varLoc, tempReg);
    }

    output_mips<<"jal "<<code._funcCallName<< "# at " << __LINE__ << std::endl;
    freeTempReg(toFree);
}

void g_func_call_end(){
    int temp = globalSigTab[myTolower(code._funcCallName)]._para_tab.size();
    output_mips<<"addu $sp $sp "<<temp * 4<< "# at " << __LINE__ << std::endl;
}

void g_label(){
    output_mips<<"label"<<code._labelName<<":"<< "# at " << __LINE__ << std::endl;
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

    if(generateFuncName == ":"){
        moveToTemp(tempReg1, globalLocAssign[name1]);
    } else{
        moveToTemp(tempReg1, funcLocAssign[name1]);
    }

    if(code._swType == INT){
        output_mips<<"bne $"<<tempReg1<<" "<<code._intValue[0]<<" label"<<code._nextIfLabel<< "# at " << __LINE__ << std::endl;
    } else {
        output_mips<<"bne $"<<tempReg1<<" "<<int(code._chValue[0])<<" label"<<code._nextIfLabel<< "# at " << __LINE__ << std::endl;
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
    if(generateFuncName == ":"){
        varLoc0 = globalLocAssign[myTolower(name0)];
        varLoc1 = globalLocAssign[myTolower(name1)];
        varLoc2 = globalLocAssign[myTolower(name2)];
        if(globalLocAssign.count(name3) == 0){
            globalLocAssign[name3] = assignNewLoc();
        }
        varLoc3 = globalLocAssign[name3];
    } else {
        if(funcLocAssign.count(name0) != 0){
            varLoc0 = funcLocAssign[myTolower(name0)];
        } else if(globalLocAssign.count(name0) != 0){
            varLoc0 = globalLocAssign[myTolower(name0)];
        } else {
            std::cout<<name0<<":"<<"1231111111111"<< "# at " << __LINE__ << std::endl;
        }
        varLoc1 = funcLocAssign[myTolower(name1)];
        varLoc2 = funcLocAssign[myTolower(name2)];
        if(globalLocAssign.count(name3) == 0){
            funcLocAssign[myTolower(name3)] = assignNewLoc();
        }
        varLoc3 = funcLocAssign[name3];
    }

    moveToTemp(tempReg1, varLoc1);
    moveToTemp(tempReg2, varLoc2);

    if(code._symProperty == 2){
        output_mips<<"mul $"<<tempReg3<<" $"<<tempReg1<<" "<<code._length<< "# at " << __LINE__ << std::endl;
        //mul t2 t1 length
        output_mips<<"addu $"<<tempReg3<<" $"<<tempReg3<<" $"<<tempReg2<< "# at " << __LINE__ << std::endl;
    } else if(code._symProperty == 1){
        output_mips<<"move $"<<tempReg3<<" $"<<tempReg1<<std::endl;
    } else{
        output_mips<<"wrong sym in arr get!!!"<<std::endl;
    }


    output_mips<<"sll $"<<tempReg3<<" $"<<tempReg3<<" "<<2 << std::endl;
    output_mips<<"addu $"<<tempReg3<<" $"<<tempReg3<<" "<<varLoc0.num<< "# at " << __LINE__ << std::endl;

    if(funcLocAssign.count(name0) != 0){
        output_mips<<"addu $"<<tempReg3<<" $"<<tempReg3<<" $sp"<< "# at " << __LINE__ << std::endl;
    } else if(globalLocAssign.count(name0) != 0){
        output_mips<<"addu $"<<tempReg3<<" $"<<tempReg3<<" $gp"<< "# at " << __LINE__ << std::endl;
    } else {
        output_mips<<"!!!!!!!!!!!!!!!!!!!!1"<< "# at " << __LINE__ << std::endl;
    }
    output_mips<<"lw $"<<tempReg1<<" 0($"<<tempReg3<<")"<< "# at " << __LINE__ << std::endl;

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

    std::string name0 = code._assName;
    std::string name1 = ":" + std::to_string(code._loc1);
    std::string name2 = ":" + std::to_string(code._loc2);
    std::string name3 = ":" + std::to_string(code._arr_ass_regNum);

    VarLoc varLoc0, varLoc1, varLoc2, varLoc3;
    if(generateFuncName == ":"){
        varLoc0 = globalLocAssign[myTolower(name0)];
        varLoc1 = globalLocAssign[myTolower(name1)];
        varLoc2 = globalLocAssign[myTolower(name2)];
        if(globalLocAssign.count(name3) == 0){
            globalLocAssign[name3] = assignNewLoc();
        }
        varLoc3 = globalLocAssign[name3];
    } else {
        if(funcLocAssign.count(name0) != 0){
            varLoc0 = funcLocAssign[myTolower(name0)];
        } else if(globalLocAssign.count(name0) != 0){
            varLoc0 = globalLocAssign[myTolower(name0)];
        } else {
            output_mips<<"12322222222222@"<<name0<<"@"<< "# at " << __LINE__ << std::endl;
        }
        varLoc1 = funcLocAssign[myTolower(name1)];
        varLoc2 = funcLocAssign[myTolower(name2)];
        if(funcLocAssign.count(name3) == 0){
            funcLocAssign[name3] = assignNewLoc();
        }
        varLoc3 = funcLocAssign[name3];
    }

    moveToTemp(tempReg1, varLoc1);
    moveToTemp(tempReg2, varLoc2);

    if(code._symProperty == 2) {
        output_mips << "mul $" << tempReg3 << " $" << tempReg1 << " " << code._length << "# at " << __LINE__
                    << std::endl;
        //mul t2 t1 length
        output_mips<<"addu $"<<tempReg3<<" $"<<tempReg3<<" $"<<tempReg2<< "# at " << __LINE__ << std::endl;
    } else if(code._symProperty == 1){
        output_mips<<"move $"<<tempReg3<<" $"<<tempReg1<<std::endl;
    } else{
        output_mips<<"wrong sym in arr ass!!!"<<std::endl;
    }

    output_mips<<"sll $"<<tempReg3<<" $"<<tempReg3<<" "<<2<< "# at " << __LINE__ << std::endl;
    output_mips<<"addu $"<<tempReg3<<" $"<<tempReg3<<" "<<varLoc0.num<< "# at " << __LINE__ << std::endl;

    if(funcLocAssign.count(name0) != 0){
        output_mips<<"addu $"<<tempReg3<<" $"<<tempReg3<<" $sp"<< "# at " << __LINE__ << std::endl;
    } else if(globalLocAssign.count(name0) != 0){
        output_mips<<"addu $"<<tempReg3<<" $"<<tempReg3<<" $gp"<< "# at " << __LINE__ << std::endl;
    } else {
        output_mips<<"!!!!!!!!!!!!!!!!!!!!1"<< "# at " << __LINE__ << std::endl;
    }

    moveToTemp(tempReg1, varLoc3);
    output_mips<<"sw $"<<tempReg1<<" 0($"<<tempReg3<<")"<< "# at " << __LINE__ << std::endl;
    freeTempReg(toFree);
}

void g_code(){
    output_mips<<code._code<< "# at " << __LINE__ << std::endl;
}


VarLoc assignNewLoc(){
    VarLoc varLoc = VarLoc();
    if(generateFuncName == ":"){
        if(gpCount != 0 || code._symProperty != 0){
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
        spCount_g+=4;
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
        output_mips<<"move $" <<tempNum<<" $"<<varLoc.num<< "# at " << __LINE__ << std::endl;
    } else if(varLoc.loc_type == 3){
        output_mips<<"lw $"<<tempNum<<" "<<varLoc.num<<"($gp)"<< "# at " << __LINE__ << std::endl;
    } else if(varLoc.loc_type == 4){
        output_mips<<"lw $"<<tempNum<<" "<<varLoc.num<<"($sp)"<< "# at " << __LINE__ << std::endl;
    }
}

void moveFromTemp(VarLoc varLoc, int tempNum){
    if(varLoc.loc_type == 1){
        output_mips<<"move $" <<varLoc.num<<" $"<<tempNum<< "# at " << __LINE__ << std::endl;
    } else if(varLoc.loc_type == 3){
        output_mips<<"sw $"<<tempNum<<" "<<varLoc.num<<"($gp)"<< "# at " << __LINE__ << std::endl;
    } else if(varLoc.loc_type == 4){
        output_mips<<"sw $"<<tempNum<<" "<<varLoc.num<<"($sp)"<< "# at " << __LINE__ << std::endl;
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



