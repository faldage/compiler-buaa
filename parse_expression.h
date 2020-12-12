#ifndef COMPILER_PARSE_EXPRESSION_H
#define COMPILER_PARSE_EXPRESSION_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <map>
#include <stack>

class Temp{
public:
    int _cal;//1:+ 2:- 3:* 4:/
    int _type;//1:regNum 2:con

    int _regNum;
    int _con;

    Temp(){};

    Temp(int cal, int type, int arg2){
        _type = type;
        _cal = cal;
        _regNum = arg2;
        _con = arg2;
    }
};
extern int itemReg;
extern int factorReg;
extern int ifItemIsCon;
extern int itemValue;
extern int ifFactorIsCon;
extern int factorValue;

extern std::vector<Temp> Items;

extern std::stack<std::vector<Temp>>ItemsStack;

#endif