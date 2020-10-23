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
        output <<error._line<<" " << error._error_type << std::endl;
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
    error_print();
    return 0;
}
