
#include <vector>
#include <string>
#include <algorithm>
#include "getToken.h"
#include "myVar.h"

char* ch;
char myCh;
std::string token;

int num;
int line;

void clearToken(){
    token = "";
}
void retract(){
    ch--;
}

void catToken() {
    token += myCh;
}

void getChar(){
    myCh = *ch;
    ch++;
}

SYMBOL reserver() {
    transform(token.begin(),token.end(),token.begin(),::tolower);
    if(token == "const"){
        return CONSTTK;
    } else if(token == "int"){
        return INTTK;
    } else if(token == "char"){
        return CHARTK;
    } else if(token == "void"){
        return VOIDTK;
    } else if(token == "main"){
        return MAINTK;
    } else if(token == "if"){
        return IFTK;
    } else if(token == "else"){
        return ELSETK;
    } else if(token == "switch"){
        return SWITCHTK;
    } else if(token == "case"){
        return CASETK;
    } else if(token == "default"){
        return DEFAULTTK;
    } else if(token == "while"){
        return WHILETK;
    } else if(token == "for"){
        return FORTK;
    } else if(token == "scanf"){
        return SCANFTK;
    } else if(token == "printf"){
        return PRINTFTK;
    } else if(token == "return"){
        return RETURNTK;
    } else{
        return IDENFR;
    }
}

int transNum() {
    int res = 0;
    int i;
    int length = token.length();
    for(i = 0; i < length; i++){
        res = res * 10 + (token[i] - '0');
    }
    return res;
}
void error(){
    std::cout<<myCh<<std::endl;
    std::cout<<*ch<<std::endl;
    std::cout<<token<<std::endl;
    std::cout<<"failed!"<<std::endl;
}

bool isSpace(){
    return myCh == ' ';
}
bool isNewLine(){
    return myCh == '\n';//??????????????????????????????
}
bool isTab(){
    return myCh == '\t';
}
bool isLetter(){
    return (myCh >= 'a' && myCh <= 'z') || (myCh >= 'A' && myCh <= 'Z');
}
bool isDigit(){
    return myCh >= '0' && myCh <= '9';
}
bool isCharCon(){
    return myCh == '\'';
}
bool isStrCon(){
    return myCh == '\"';
}
bool isPlus(){
    return myCh == '+';
}
bool isMinu(){
    return myCh == '-';
}
bool isMult(){
    return myCh == '*';
}
bool isDiv(){
    return myCh == '/';
}
bool isLss(){
    return myCh == '<';
}
bool isEql(){
    return myCh == '=';
}
bool isGre(){
    return myCh == '>';
}
bool isNeq(){
    return myCh == '!';
}
bool isColon(){
    return myCh == ':';
}
bool isSemicn(){
    return myCh == ';';
}
bool isComma(){
    return myCh == ',';
}
bool isLparent(){
    return myCh == '(';
}
bool isRparent(){
    return myCh == ')';
}
bool isLBRACK(){
    return myCh == '[';
}
bool isRBRACK(){
    return myCh == ']';
}
bool isLBRACE(){
    return myCh == '{';
}
bool isRBRACE(){
    return myCh == '}';
}

void system(){
    clearToken();
    Word newWord = Word();
    while(isSpace() || isNewLine() || isTab() || myCh == '\r'){
        if(isNewLine()){
            line++;
        }
        getChar();
    }
    if(myCh == '\0'){
        return ;
    }

    if(isLetter()){
        while(isLetter() || isDigit() || myCh == '_'){
            catToken();
            getChar();
        }
        retract();
        std::string temp = token;
        SYMBOL symbol = reserver();
        newWord.getNewWord(temp, symbol, line);
    }else if(myCh ==  '_'){
        while(isLetter() || isDigit() || myCh == '_'){
            catToken();
            getChar();
        }
        retract();
        newWord.getNewWord(token, IDENFR, line);
    }else if(isDigit()){
        while(isDigit()){
            catToken();
            getChar();
        }
        retract();
        newWord.getNewWord(token, INTCON, line);
    }else if(isCharCon()){
        getChar();
        catToken();
        getChar();
        if(!isCharCon()){
            error();
        }
        newWord.getNewWord(token, CHARCON, line);
    }else if(isStrCon()){
        getChar();
        while(!isStrCon() && myCh >= 32 && myCh <= 126){
            catToken();
            getChar();
        }
        if(!isStrCon()){
            error();
        }
        newWord.getNewWord(token, STRCON, line);
    }else if (isPlus()){
        newWord.getNewWord("+", PLUS, line);
    }else if (isMinu()){
        newWord.getNewWord("-", MINU, line);
    }else if (isMult()){
        newWord.getNewWord("*", MULT, line);
    }else if (isDiv()){
        newWord.getNewWord("/", DIV, line);
    }else if (isLss()){
        getChar();
        if(isEql()) {
            newWord.getNewWord("<=", LEQ, line);
        } else {
            retract();
            newWord.getNewWord("<", LSS, line);
        }
    }else if (isGre()){
        getChar();
        if(isEql()) {
            newWord.getNewWord(">=", GEQ, line);
        } else {
            retract();
            newWord.getNewWord(">", GRE, line);
        }
    }else if (isEql()){
        getChar();
        if(isEql()) {
            newWord.getNewWord("==", EQL, line);
        } else {
            retract();
            newWord.getNewWord("=", ASSIGN, line);
        }
    }else if(isNeq()){
        getChar();
        if(isEql()) {
            newWord.getNewWord("!=", NEQ, line);
        } else {
            retract();
            error();
        }
    }else if(isColon()){
        newWord.getNewWord(":", COLON, line);
    }else if(isSemicn()){
        newWord.getNewWord(";", SEMICN, line);
    }else if(isComma()){
        newWord.getNewWord(",", COMMA, line);
    }else if(isLparent()){
        newWord.getNewWord("(", LPARENT, line);
    }else if(isRparent()){
        newWord.getNewWord(")", RPARENT, line);
    }else if(isLBRACK()){
        newWord.getNewWord("[", LBRACK, line);
    }else if(isRBRACK()){
        newWord.getNewWord("]", RBRACK, line);
    }else if(isLBRACE()){
        newWord.getNewWord("{", LBRACE, line);
    }else if(isRBRACE()){
        newWord.getNewWord("}", RBRACE, line);
    }else{
        error();
    }

    words.push_back(newWord);
    //words[countWords] = newWord;
    //countWords++;
}
void initial(){
    names[IDENFR]="IDENFR";
    names[INTCON]="INTCON";
    names[CHARCON]="CHARCON";
    names[STRCON]="STRCON";
    names[CONSTTK]="CONSTTK";
    names[INTTK]="INTTK";
    names[CHARTK]="CHARTK";

    names[VOIDTK]="VOIDTK";
    names[MAINTK]="MAINTK";
    names[IFTK]="IFTK";
    names[ELSETK]="ELSETK";
    names[SWITCHTK]="SWITCHTK";
    names[CASETK]="CASETK";
    names[DEFAULTTK]="DEFAULTTK";
    names[WHILETK]="WHILETK";

    names[FORTK]="FORTK";
    names[SCANFTK]="SCANFTK";
    names[PRINTFTK]="PRINTFTK";
    names[RETURNTK]="RETURNTK";
    names[PLUS]="PLUS";
    names[MINU]="MINU";
    names[MULT]="MULT";
    names[DIV]="DIV";
    names[LSS]="LSS";
    names[LEQ]="LEQ";

    names[GRE]="GRE";
    names[GEQ]="GEQ";
    names[EQL]="EQL";
    names[NEQ]="NEQ";
    names[COLON]="COLON";
    names[ASSIGN]="ASSIGN";
    names[SEMICN]="SEMICN";
    names[COMMA]="COMMA";
    names[LPARENT]="LPARENT";
    names[RPARENT]="RPARENT";

    names[LBRACK]="LBRACK";
    names[RBRACK]="RBRACK";
    names[LBRACE]="LBRACE";
    names[RBRACE]="RBRACE";
}