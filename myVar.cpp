//
// Created by h on 2020/10/20.
//
#include <vector>
#include <string>
#include <stack>
#include "myVar.h"


int transNum(std::string str) {
    int res = 0;
    int i;
    int length = str.length();
    for(i = 0; i < length; i++){
        res = res * 10 + (str[i] - '0');
    }
    return res;
}
std::string names[50];
std::vector<Word>words;
std::vector<Error>errors;
std::map<std::string, Signal>globalSigTab;
std::map<std::string, Signal>funcSigTab;

std::ofstream output("error.txt");
