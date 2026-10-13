#include <ctype.h>
#include "set.h"

int keyword() {
    switch(lexeme[0]) {
        case 'b':
        if (lexeme[1] == 'r' &&
            lexeme[2] == 'e' &&
            lexeme[3] == 'a' &&
            lexeme[4] == 'k' &&
            !lexeme[5])
            return BREAK;
        else if (lexeme[1] == 'u' &&
                 lexeme[2] == 't' &&
                 !lexeme[3])
            return BUT;
        else if (lexeme[1] == 'o' &&
                 lexeme[2] == 'o' &&
                 lexeme[3] == 'l' &&
                 !lexeme[4])
            return BOOL;
        return IDENT;
        case 'c':
        if (lexeme[1] == 'o' &&
            lexeme[2] == 'n') {
            if (lexeme[3] == 't' &&
                lexeme[4] == 'i' &&
                lexeme[5] == 'n' &&
                lexeme[6] == 'u' &&
                lexeme[7] == 'e' &&
                !lexeme[8])
                return CONTINUE;
            else if (lexeme[3] == 's' &&
                     lexeme[4] == 't' &&
                     !lexeme[5])
                return CONST;
        } return IDENT;
        case 'd':
        if (lexeme[1] == 'e' &&
            lexeme[2] == 'c' &&
            !lexeme[3])
            return DEC;
        return IDENT;
        case 'e':
        if (lexeme[1] == 'l' &&
            lexeme[2] == 's' &&
            lexeme[3] == 'e' &&
            !lexeme[4])
            return ELSE;
        return IDENT;
        case 'f':
        if (lexeme[1] == 'a' &&
            lexeme[2] == 'l' &&
            lexeme[3] == 's' &&
            lexeme[4] == 'e' &&
            !lexeme[5])
            return FALSE;
        return IDENT;
        case 'g':
        if (lexeme[1] == 'l' &&
            lexeme[2] == 'o' &&
            lexeme[3] == 'b' &&
            lexeme[4] == 'a' &&
            lexeme[5] == 'l' &&
            !lexeme[6])
            return GLOBAL;
        return IDENT;
        case 'i':
        if (lexeme[1] == 'f' &&
            !lexeme[2])
            return IF;
        else if (lexeme[1] == 'n' &&
                 lexeme[2] == 't' &&
                 !lexeme[3])
            return INT;
        return IDENT;
        case 'm':
        if (lexeme[1] == 'a' &&
            lexeme[2] == 'i' &&
            lexeme[3] == 'n' &&
            !lexeme[4])
            return MAIN;
        return IDENT;
        case 'n':
        if (lexeme[1] == 'u' &&
            lexeme[2] == 'l' &&
            lexeme[3] == 'l' &&
            !lexeme[4])
            return NUL;
        return IDENT;
        case 'r':
        if (lexeme[1] == 'e' &&
            lexeme[2] == 't' &&
            lexeme[3] == 'u' &&
            lexeme[4] == 'r' &&
            lexeme[5] == 'n' &&
            !lexeme[6])
            return RETURN;
        return IDENT;
        case 's':
        if (lexeme[1] == 't' &&
            lexeme[2] == 'r') {
            if (lexeme[3] == 'u' &&
                lexeme[4] == 'c' &&
                lexeme[5] == 't' &&
                !lexeme[6])
                return STRUCT;
            else if (lexeme[3] == 'i' &&
                     lexeme[4] == 'n' &&
                     lexeme[5] == 'g' &&
                     !lexeme[6])
                return STRING;
        } return IDENT;
        case 't':
        if (lexeme[1] == 'r') {
            if (lexeme[2] == 'u' &&
                lexeme[3] == 'e' &&
                !lexeme[4])
                return TRUE;
            else if (lexeme[2] == 'y' &&
                     !isalpha(lexeme[3]))
                return TRY;
        } return IDENT;
        case 'w':
        if (lexeme[1] == 'h' &&
            lexeme[2] == 'i' &&
            lexeme[3] == 'l' &&
            lexeme[4] == 'e' &&
            !lexeme[5])
            return WHILE;
        return WHILE;
        default:
        return IDENT;
    }
}

int opNoPrefix() {
    switch(lexeme[0]) {
        case '\'':
        return S_QUOTE;
        case '"':
        return D_QUOTE;
        case ',':
        return COMMA;
        case '%':
        return MODULO;
        case '{':
        return O_SQRL;
        case '}':
        return C_SQRL;
        case '[':
        return O_BRACK;
        case ']':
        return C_BRACK;
        case '(':
        return O_PAREN;
        case ')':
        return C_PAREN;
        case '.':
        return DOT;
        case '$':
        return INTER;
        default:
        return UNKNOWN;
    }
}

int opWPrefix() {
    switch(lexeme[0]) {
        case '+':
        if (!lexeme[1])
            return ADD_OP;
        switch(lexeme[1]) {
            case '+':
            return INCREM;
            case '=':
            return PLSEQL;
            default:
            return UNKNOWN;
        }
        case '-':
        if (!lexeme[1])
            return SUB_OP;
        else
            switch(lexeme[1]) {
                case '-':
                return DECREM;
                case '=':
                return MINEQL;
                default:
                return UNKNOWN;
            }
        case '*':
        if (!lexeme[1])
            return MULT_OP;
        else
            switch(lexeme[1]) {
                case '/':
                return C_COMME;
                case '=':
                return TIMEQL;
                default:
                return UNKNOWN;
            }
        case '/':
        if (!lexeme[1])
            return DIV_OP;
        else
            switch(lexeme[1]) {
                case '*':
                return O_COMME;
                case '/':
                return S_COMME;
                case '=':
                return DIV_OP;
                default:
                return UNKNOWN;
            }
        case '&':
        if (!lexeme[1])
            return BITAND;
        else if (lexeme[1] == '&')
            return COMAND;
        else return UNKNOWN;
        case '|':
        if (!lexeme[1])
            return BITOR;
        else if (lexeme[1] == '|')
            return COMOR;
        else return UNKNOWN;
        case '!':
        if (!lexeme[1])
            return BITNOT;
        else if (lexeme[1] == '=')
            return NOTEQL;
        else return UNKNOWN;
        case '^':
        if (!lexeme[1])
            return BITXOR;
        case '<':
        if (!lexeme[1])
            return LESS;
        else {
            switch(lexeme[1]) {
                case '<':
                return BITLFT;
                case '=':
                return LSSEQL;
                default:
                return UNKNOWN;
            }
        }
        case '>':
        if (!lexeme[1])
            return GREAT;
        else
            switch(lexeme[1]) {
                case '>':
                return BITRGHT;
                case '=':
                return GRTEQL;
                default:
                return UNKNOWN;
            }
        case '=':
        if (!lexeme[1])
            return EQL;
        else if (lexeme[1] == '=')
            return EQLEQL;
        else return UNKNOWN;
    }
}