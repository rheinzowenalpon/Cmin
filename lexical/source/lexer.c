#ifndef _TOKEN
#define _TOKEN
#include "../source/token.c"
#endif
#ifndef _SOURCE
#define _SOURCE
#include "../source/scan.c"
#endif
 
#include <stdio.h>
#include <ctype.h>
#include <string.h>
 
/* ======================== PROTOTYPES ======================== */
void getChar(void);
void addChar(void);
void getNonBlank(void);
int  analyzer(void);
static void skipLineComment(void);
static void skipBlockComment(void);
 
// The closing token of a block comment is reported on the next call.
static int pendingCloseComment = 0;
 
/* ======================== ENTRY POINT ======================== */
int main(void) {
    fileSearch();
 
    if ((fptr = fopen(fileName, "r")) == NULL) {
        printf("ERROR - file not found\n");
        return 1;
    }
 
    if (!debugVal) {
        printf("ERROR - unknown file extension found\n");
        fptrErr = fopen("../bin/err.txt", "w");
        if (fptrErr) {
            fprintf(fptrErr, "ERROR - unknown file extension found\n");
            fclose(fptrErr);
        }
        fclose(fptr);
        return 1;
    }
 
    remove("../bin/err.txt");
 
    if ((fptr2 = fopen("../bin/symbol_table.txt", "w")) == NULL) {
        printf("ERROR - could not create symbol_table.txt\n");
        fclose(fptr);
        return 1;
    }
    fprintf(fptr2, "%-15s%-15s%-15s\n\n", "Token Type", "Lexeme", "Token Value");
 
    getChar();                          /* prime the first character */
    do {
        analyzer();
    } while (nextToken != EOF_);
 
    fclose(fptr2);
    fclose(fptr);
    return 0;
}
 
/* ======================== CHARACTER HANDLING ======================== */
 
void getChar(void) {
    if ((nextChar = getc(fptr)) != EOF) {
        if (isalpha(nextChar) || nextChar == '_')
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else if (nextChar == '\n')
            charClass = NEWLINE;
        else if (nextChar == '#')
            charClass = HASH;
        else if (nextChar == '\\')
            charClass = ESCCHAR;
        else
            charClass = UNKNOWN;
    } else {
        charClass = EOF_;
    }
}
 
void addChar(void) {
    if (lexLen < 1024) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen]   = '\0';
    } else {
        printf("ERROR - lexeme too long\n");
    }
}
 
void getNonBlank(void) {
    while (nextChar < 33 && nextChar != '\n' && nextChar != EOF)
        getChar();
}
 
static void skipLineComment(void) {             /* leaves the '\n' for its own token */
    while (nextChar != EOF && nextChar != '\n')
        getChar();
}
 
static void skipBlockComment(void) {            // called once the opening token has been read
    int prev = 0;
    while (nextChar != EOF) {
        if (prev == '*' && nextChar == '/') {
            pendingCloseComment = 1;
            getChar();
            return;
        }
        prev = nextChar;
        getChar();
    }
    printf("ERROR - unterminated comment\n");
}
 
/* ======================== LEXICAL ANALYZER ======================== */
 
int analyzer(void) {
    lexLen = 0;
    lexeme[0] = '\0';
 
    if (pendingCloseComment) {
        pendingCloseComment = 0;
        strcpy(lexeme, "*/");
        lexLen    = 2;
        nextToken = C_COMME;
    } else {
        getNonBlank();
 
        switch (charClass) {
 
        case LETTER:                            /* identifier or keyword */
            do {
                addChar();
                getChar();
            } while (charClass == LETTER || charClass == DIGIT);
            nextToken = keyword();
            break;
 
        case ESCCHAR:                           /* backslash + one character, e.g. \n */
            addChar();
            getChar();
            if (nextChar != EOF && nextChar > 32) {
                addChar();
                getChar();
                nextToken = ESCCHAR;
            } else {
                nextToken = UNKNOWN;
            }
            break;
 
        case DIGIT:                             /* 123   12.5 */
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            if (nextChar == '.') {
                addChar();
                getChar();
                while (charClass == DIGIT) {
                    addChar();
                    getChar();
                }
            }
            if (charClass == LETTER || nextChar == '.') {       /* 12abc   1.2.3 */
                while (charClass == LETTER || charClass == DIGIT || nextChar == '.') {
                    addChar();
                    getChar();
                }
                nextToken = UNKNOWN;
            } else if (lexeme[lexLen - 1] == '.') {             /* 1.  */
                nextToken = UNKNOWN;
            } else {
                nextToken = CONSTANT;
            }
            break;
 
        case NEWLINE:
            getChar();
            strcpy(lexeme, "\\n");
            lexLen    = 2;
            nextToken = NEWLINE;
            break;
 
        case HASH:                              /* #include */
            do {
                addChar();
                getChar();
            } while (charClass == LETTER);
            nextToken = (strcmp(lexeme, "#include") == 0) ? INCLUDE : UNKNOWN;
            break;
 
        case EOF_:
            strcpy(lexeme, "EOF");
            lexLen    = 3;
            nextToken = EOF_;
            break;
 
        default: {                              /* operators, punctuation, junk */
            int single;
            addChar();
            single = opNoPrefix();
 
            if (single == UNKNOWN) {            /* unknown run: swallow up to whitespace */
                getChar();
                while (nextChar != EOF && nextChar > 32) {
                    addChar();
                    getChar();
                }
                nextToken = UNKNOWN;
            } else {
                nextToken = single;
                getChar();                      /* peek at the following character */
                if (nextChar != EOF && nextChar > 32) {
                    int two;
                    addChar();                  /* tentatively build a two-character operator */
                    two = opWPrefix();
                    if (two != UNKNOWN) {
                        nextToken = two;
                        getChar();
                    } else {
                        lexLen--;               /* not a pair: undo, keep the single token */
                        lexeme[lexLen] = '\0';
                    }
                }
                if (nextToken == S_COMME)
                    skipLineComment();
                else if (nextToken == O_COMME)
                    skipBlockComment();
            }
            break;
        }
        }
    }
 
    printf("Next token is : %d, next lexeme is %s\n", nextToken, lexeme);
    /* widths are fixed: the parser reads this file by byte offset.
       .14 keeps an over-long lexeme from shifting the token-value column. */
    fprintf(fptr2, "%-15.14s%-15.14s%-15d\n", symTab(nextToken), lexeme, nextToken);
 
    memset(lexeme, 0, (size_t)lexLen + 1);
    return nextToken;
}