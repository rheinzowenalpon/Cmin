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

void program();
void block();
void statement();
void line();
void factor();
void interpolation();
void loop();
void conditionals();
void error();
void main_();
void outers();
void array();
void data_type();
void function();
void func_decl();
void func_call();
void struct_block();
void struct_();
void lower_declaration();
void declaration();
void include();
void expression();
void lower_assignment();
void symmetric_equality_comparison();
void declaration_parameter();
void logical_or();
void logical_and();
void binary_or();
void binary_xor();
void binary_and();
void symmetric_equality_comparison();
void assymmetric_equality_comparison();
void declaration_parameter();
void binary_bitwise();
void upper_arithmetic();
void lower_arithmetic();
void prefix();
void postfix();
void variable();
void input();

//Utilities
bool isFirstToken = 1;
int currentLine = 1;
int currToken;
void panic(char syncToken);