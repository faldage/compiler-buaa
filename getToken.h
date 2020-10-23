#ifndef BY2_GETTOKEN_H
#define BY2_GETTOKEN_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "myVar.h"

extern char* ch;
extern char myCh;
extern std::string token;

extern int num;
extern int line;


void clearToken();
void retract();

void catToken();

void getChar();

SYMBOL reserver();

void error();

bool isSpace();
bool isNewLine();
bool isTab();
bool isLetter();
bool isDigit();
bool isCharCon();
bool isStrCon();
bool isPlus();
bool isMinu();
bool isMult();
bool isDiv();
bool isLss();
bool isEql();
bool isGre();
bool isNeq();
bool isColon();
bool isSemicn();
bool isComma();
bool isLparent();
bool isRparent();
bool isLBRACK();
bool isRBRACK();
bool isLBRACE();
bool isRBRACE();

void system();
void initial();

#endif
