#ifndef _SET
#define _SET
#include "../../lexical/header/set.h"
#endif

#include "../../lexical/header/dirent.h"
#include <stdlib.h>
#include <ctype.h>

void getToken();
void semantic();
void match(int expectedToken);
int parseSpecial();

//Class Function references

void declaration_parameter();
void struct_block();
void declaration();
void data_type();
void func_decl();
void function();
void program();
void include();
void struct_();
void outers();
void value();
void main_();
void block();
void loop();

//Utilities
bool isFirstToken = 1;
int currentLine = 1;
int currToken;
void panic();