#pragma once
void echo(char* info,...);
void warning( char * info,...);
void error( char * info,...);
void success(char * info , ...);
void message(char * info , ...);
void startup(char* game,char*version);
struct ErrorLevel {
    int value;
    ErrorLevel(int var){

        this->value=var;
    }
    void setvalue(int var){
        this->value=var;
    }
};
extern ErrorLevel level;
/*
-1: none
0: error
1: success
2: warning
3: message
4: echo
*/







