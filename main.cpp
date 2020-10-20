//
// Created by h on 2020/9/23.
//
#include <iostream>
#include <string>
#include "getToken.h"
#include "parse.h"

using namespace std;

int main() {
    initial();
    std::ifstream t("testfile.txt");
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    ch = &str[0];
    while(*ch != '\0'){
        getChar();
        system();
    }
    //t2<<str<<endl;
    // for(int i = 0; i < countWords; i++){
    //    t2<<names[words[i]._symbol] << " "<< words[i]._name<<endl;
    //}
    parse();
    return 0;
}
