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
                std::cout<<"INT "<<i._name<<i._intValue<<endl;
            } else if(i._vcType == CHAR){
                std::cout<<"CHAR "<<i._name<<i._chValue<<endl;
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
                std::cout<<"exp"<<endl;
            }
        } else if(i._interSym == I_ASSIGN){
            std::cout<<"ASSIGN "<<i._assName<<endl;
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
