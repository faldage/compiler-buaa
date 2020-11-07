//
// Created by h on 2020/9/23.
//
#include <iostream>
#include <string>
#include "myVar.h"
#include "getToken.h"
#include "parse.h"

using namespace std;

void error_print(){
    for(Error error: errors){
        output_error <<error._line<<" " << error._error_type << std::endl;
    }
}

void printINTER(){
    std::cout<<"stringList"<<endl;
    for(std::string str : stringList){
        std::cout<<str<<endl;
    }
    std::cout<<"______________"<<endl;
    for(IntermediateCode i : intermediateCodes){
        if(i._interSym == I_CON){
            std::cout<<"CON ";
            if(i._vcType == INT){
                std::cout<<"INT "<<i._name<<" " << i._intValue<<endl;
            } else if(i._vcType == CHAR){
                std::cout<<"CHAR "<<i._name<<" "<<i._chValue<<endl;
            }
        }  else if(i._interSym == I_VAR){
            std::cout<<"VAR ";
            if(i._vcType == INT){
                std::cout<<"INT "<<i._name<<" " << i._intValue<<endl;
            } else if(i._vcType == CHAR){
                std::cout<<"CHAR "<<i._name<<" "<<i._chValue<<endl;
            }
        } else if(i._interSym == I_SCANF){
            std::cout<<"SCANF "<<i._scName;
            if(i._scType == INT){
                std::cout<<" INT"<<endl;
            } else if(i._scType == CHAR){
                std::cout<<" CHAR"<<endl;
            }
        } else if(i._interSym == I_PRINTF){
            std::cout<<"PRINTF ";
            if(i._symProperty == 1){
                std::cout<<i._priStr<<endl;
            } else {
                std::cout<<"exp"<<" t"<<i._priExpResReg<<endl;
            }
        } else if(i._interSym == I_ASSIGN){
            std::cout<<"ASSIGN "<<i._assName<< " t"<<i._assExpResReg <<endl;
        } else if(i._interSym == I_CAL){
            if(i._symProperty == 1){
                std::cout<<"ADD ";
            } else if(i._symProperty == 2){
                std::cout<<"SUB ";
            } else if(i._symProperty == 3){
                std::cout<<"MUL ";
            } else {//==4
                std::cout<<"DIV ";
            }
            std::cout<<"t"<<i._resRegNum<<",";
            if(i._cal1Type == 1){
                std::cout<<"t"<<i._reg1<<",";
            } else if(i._cal1Type == 2){
                std::cout<<"(iden)"<<i._iden1<<",";
            } else if(i._cal1Type == 3){
                std::cout<<"(char)"<<i._ch1<<",";
            } else {
                std::cout<<"(int)"<<i._int1<<",";
            }

            if(i._cal2Type == 1){
                std::cout<<"t"<<i._reg2<<endl;
            } else if(i._cal2Type == 2){
                std::cout<<"(iden)"<<i._iden2<<endl;
            } else if(i._cal2Type == 3){
                std::cout<<"(char)"<<i._ch2<<endl;
            } else {
                std::cout<<"(int)"<<i._int2<<endl;
            }
        }else{
            std::cout<<"???"<<endl;
        }
    }
}

int main() {
    initial();
    std::ifstream t("testfile.txt");
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    ch = &str[0];
    line = 1;
    while(*ch != '\0'){
        getChar();
        system();
    }
    //t2<<str<<endl;
    //for(int i = 0; i < words.size(); i++){
    //    t2<<names[words[i]._symbol] << " "<< words[i]._name<<": line"<<words[i]._line<<endl;
    //}
    parse();
    //error_print();
    if(errors.size() != 0){
        printf("some errors!\n");
    } else {
        printINTER();
    }
    return 0;
}
