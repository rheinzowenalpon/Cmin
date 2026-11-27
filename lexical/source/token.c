#ifndef _SET
#define _SET
#include "../header/set.h"
#endif
 
#include <string.h>
 
/* ======================== KEYWORDS ======================== */
 
typedef struct {
    const char *word;
    int         token;
} Keyword;
 
static const Keyword keywords[] = {
    { "bool",     BOOL     },
    { "break",    BREAK    },
    { "but",      BUT      },
    { "char",     CHAR_    },
    { "const",    CONST_   },
    { "continue", CONTINUE },
    { "dec",      DEC      },
    { "else",     ELSE     },
    { "false",    FALSE_   },
    { "global",   GLOBAL   },
    { "if",       IF       },
    { "int",      INT_     },
    { "main",     MAIN     },
    { "null",     NUL      },
    { "return",   RETURN   },
    { "string",   STRING   },
    { "struct",   STRUCT   },
    { "true",     TRUE_    },
    { "try",      TRY      },
    { "while",    WHILE    }
};
 
/* Classify the identifier-shaped lexeme currently in `lexeme`. */
int keyword(void) {
    size_t i;
    for (i = 0; i < sizeof keywords / sizeof keywords[0]; i++)
        if (strcmp(lexeme, keywords[i].word) == 0)
            return keywords[i].token;
    return IDENT;
}
 
/* ======================== OPERATORS ======================== */
 
/* One-character operators / punctuation: looks at lexeme[0] only. */
int opNoPrefix(void) {
    switch (lexeme[0]) {
    case '\'': return S_QUOTE;
    case '"':  return D_QUOTE;
    case ',':  return COMMA;
    case '%':  return MODULO;
    case '{':  return O_SQRL;
    case '}':  return C_SQRL;
    case '[':  return O_BRACK;
    case ']':  return C_BRACK;
    case '(':  return O_PAREN;
    case ')':  return C_PAREN;
    case '.':  return DOT;
    case '$':  return INTER;
    case '^':  return BITXOR;
    case '~':  return BITNOT;
    case '+':  return ADD_OP;
    case '-':  return SUB_OP;
    case '*':  return MULT_OP;
    case '/':  return DIV_OP;
    case '&':  return BITAND;
    case '|':  return BITOR;
    case '!':  return NEGATE;
    case '<':  return LESS;
    case '>':  return GREAT;
    case '=':  return EQL;
    default:   return UNKNOWN;
    }
}
 
/* Two-character operators: looks at lexeme[0] and lexeme[1].
   Returns UNKNOWN when the pair is not a two-character operator
   (the caller then keeps the one-character token). Does not modify lexeme. */
int opWPrefix(void) {
    char a = lexeme[0];
    char b = lexeme[1];
 
    switch (a) {
    case '+':
        if (b == '+') return INCREM;
        if (b == '=') return PLSEQL;
        break;
    case '-':
        if (b == '-') return DECREM;
        if (b == '=') return MINEQL;
        break;
    case '*':
        if (b == '/') return C_COMME;
        if (b == '=') return TIMEQL;
        break;
    case '/':
        if (b == '*') return O_COMME;
        if (b == '/') return S_COMME;
        if (b == '=') return DIVEQL;
        break;
    case '&':
        if (b == '&') return COMAND;
        break;
    case '|':
        if (b == '|') return COMOR;
        break;
    case '!':
        if (b == '=') return NOTEQL;
        break;
    case '<':
        if (b == '<') return BITLFT;
        if (b == '=') return LSSEQL;
        break;
    case '>':
        if (b == '>') return BITRGHT;
        if (b == '=') return GRTEQL;
        break;
    case '=':
        if (b == '=') return EQLEQL;
        break;
    }
    return UNKNOWN;
}
