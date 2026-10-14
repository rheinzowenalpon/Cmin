#include <stdio.h>

#define MAX_LEXEME 1024

#define LETTER    0
#define DIGIT     1
#define DOT       2
#define HASH      3
#define NEWLINE   4

#define IDENT    10
#define CONSTANT 11
#define IF       12
#define ELSE     13
#define WHILE    14
#define CONTINUE 15
#define BREAK    16
#define TRY      17
#define BUT      18
#define CONST    19
#define GLOBAL   20
#define INT      21
#define DEC      22
#define STRING   23
#define NUL      24
#define STRUCT   25
#define BOOL     26
#define TRUE     27
#define FALSE    28
#define RETURN   29
#define INCLUDE  30
#define MAIN     31
#define ADD_OP   32
#define SUB_OP   33
#define MULT_OP  34
#define DIV_OP   35
#define MODULO   36
#define INCREM   37
#define DECREM   38
#define BITNOT   39
#define BITAND   40
#define BITOR    41
#define BITXOR   42
#define BITLFT   43
#define BITRGHT  44
#define EQL      45
#define PLSEQL   46
#define MINEQL   47
#define TIMEQL   48
#define DIVEQL   49
#define LESS     50
#define GREAT    51
#define LSSEQL   52
#define GRTEQL   53
#define EQLEQL   54
#define COMAND   55
#define COMOR    56
#define NOTEQL   57
#define O_PAREN  58
#define C_PAREN  59
#define O_BRACK  60
#define C_BRACK  61
#define O_SQRL   62
#define C_SQRL   63
#define COMMA    64
#define S_QUOTE  65
#define D_QUOTE  66
#define S_COMME  67
#define O_COMME  68
#define C_COMME  69
#define INTER    70

#define UNKNOWN  99

void getChar();
void addChar();
void getNonBlank();
void analyzer();

char lexeme[MAX_LEXEME];
char nextChar;
short int nextToken, charClass;
unsigned short int lexLen;
FILE *fptr;