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
        } else {
            std::cout<<"not in scale"<<std::endl;
        }
        get_next_code();
    }
}

void initialize(){
    for(int i = 8; i <= 25; i++){
        regAssign[i] = 0;
    }
}

void assString(){
    output_mips<<".data"<<std::endl;
    output_mips<<"space: .asciiz \" \""<<std::endl;
    output_mips<<"enter: .asciiz \"\\n\""<<std::endl;
    for(auto &v : stringList){
        output_mips<<"str"<<v.second<<":" <<".asciiz ";
        output_mips<<"\""<<v.first<<"\""<<std::endl;
    }
}

void g_assign(){
    std::string nameRes = "t"+ std::to_string(code._assExpResReg);
    move(locAssign[nameRes], locAssign[code._assName]);
}
void g_printf(){
    if(symPro_g == 1 || symPro_g == 3){
        int strNum = stringList[code._priStr];
        output_mips<<"la $a0 str"<<strNum<<std::endl;
        output_mips<<"li $v0 4"<<std::endl;
        output_mips<<"syscall"<<std::endl;
    } else {
        std::string temp = "t" + std::to_string(code._priExpResReg);
        VarLoc varLoc = locAssign[temp];


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
    VarLoc varLoc = locAssign[code._scName];
    if(varLoc.loc_type == 1){
        output_mips<<"move $"<<varLoc.num<<" $v0"<<std::endl;
    } else if(varLoc.loc_type == 3){
        output_mips<<"sw $v0 "<<varLoc.num<<"($gp)"<<std::endl;
    }
}

void g_var(){
    VarLoc varLoc = assignNewLoc();
    locAssign[code._name] = varLoc;
    if(varLoc.loc_type == 1){
        output_mips<<"li $"<<varLoc.num<<" ";
        if(code._vcType == INT){
            output_mips<<code._intValue<<std::endl;
        } else{
            output_mips<<int(code._chValue)<<std::endl;
        }
    } else if(varLoc.loc_type == 3){
        int temp = assignTempReg();
        std::vector<int>toFree;
        toFree.push_back(temp);
        output_mips<<"li $"<<temp<<" ";
        if(code._vcType == INT){
            output_mips<<code._intValue<<std::endl;
        } else{
            output_mips<<int(code._chValue)<<std::endl;
        }
        moveFromTemp(varLoc, temp);
        freeTempReg(toFree);
    }
}

void g_const(){
    VarLoc varLoc = assignNewLoc();
    locAssign[code._name] = varLoc;
    if(varLoc.loc_type == 1){
        output_mips<<"li $"<<varLoc.num<<" ";
        if(code._vcType == INT){
            output_mips<<code._intValue<<std::endl;
        } else{
            output_mips<<int(code._chValue)<<std::endl;
        }
    } else if(varLoc.loc_type == 3){
        int temp = assignTempReg();
        std::vector<int>toFree;
        toFree.push_back(temp);
        output_mips<<"li $"<<temp<<" ";
        if(code._vcType == INT){
            output_mips<<code._intValue<<std::endl;
        } else{
            output_mips<<int(code._chValue)<<std::endl;
        }
        moveFromTemp(varLoc, temp);
        freeTempReg(toFree);
    }
}

void g_cal(){
    std::string nameRes = "t"+ std::to_string(code._resRegNum);
    if(locAssign.count(nameRes) == 0){
        locAssign[nameRes] = assignNewLoc();
    }

    int tempReg1 = assignTempReg();
    int tempReg2 = assignTempReg();
    int tempResReg = assignTempReg();
    std::vector<int> toFree;
    toFree.push_back(tempReg1);
    toFree.push_back(tempReg2);
    toFree.push_back(tempResReg);
    std::string name1, name2;
    if(code._cal1Type == 1 || code._cal1Type == 2){
        if(code._cal1Type == 1){
            name1 = "t" + std::to_string(code._reg1);
        } else if(code._cal1Type == 2){
            name1 = code._iden1;
        }
        moveToTemp(tempReg1, locAssign[name1]);
    }

    if(code._cal2Type == 1 || code._cal2Type == 2){
        if(code._cal2Type == 1){
            name2 = "t" + std::to_string(code._reg2);
        } else if(code._cal2Type == 2){
            name2 = code._iden2;
        }
        moveToTemp(tempReg2, locAssign[name2]);
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

    moveFromTemp(locAssign[nameRes], tempResReg);
    freeTempReg(toFree);
}

VarLoc assignNewLoc(){
    VarLoc varLoc = VarLoc();
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



