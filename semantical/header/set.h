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

//Classes
void program(void);
void block(void);
void block_contents(void);
void statement(void);
void line(void);
void variable(void);
void factor(void);
void interpolation(void);
void loop(void);
void conditionals(void);
void error(void);
void main_(void);
void outers(void);
void array(void);
void data_type(void);
void declaration_parameter(void);
void function(void);
void func_decl(void);
void struct_block(void);
void struct_(void);
void lower_declaration(void);
void declaration(void);
void include(void);
void expression(void);
void lower_assignment(void);
void logical_or(void);
void logical_and(void);
void binary_or(void);
void binary_xor(void);
void binary_and(void);
void symmetric_equality_comparison(void);
void assymmetric_equality_comparison(void);
void binary_bitwise(void);
void upper_arithmetic(void);
void lower_arithmetic(void);
void prefix(void);
void postfix(void);
void func_call(void);
void input(void);

//Utilities
bool isFirstToken = 1;
int currentLine = 1;
int currToken;
void panic(char syncToken);