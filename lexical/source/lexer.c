#ifndef _TOKEN
#define _TOKEN
#include "../source/token.c"
#endif
#ifndef _SOURCE
#define _SOURCE
#include "../source/scan.c"
#endif

void main() {
    fileSearch();
    if ((fptr = fopen(fileName, "r")) == NULL)
        printf("ERROR - file not found\n");
    else {
        lexemeFile = fopen("../bin/token.lxmf", "w");
        fprintf(lexemeFile, "%-15s%-15s\n\n", "Token Type", "Lexeme");
        getChar();
        do {
            analyzer();
        } while(nextToken != EOF);
    }
    fclose(lexemeFile);
    fclose(fptr);
}

void getChar() {
    if ((nextChar = getc(fptr)) != EOF) {
        if (isalpha(nextChar)) 
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
    } else
        charClass = EOF;
}

void addChar() {
    if (lexLen <= 1024) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else
        printf("ERROR - lexeme too long\n");
}

void getNonBlank() {
    while(nextChar == ' ')
        getChar();
}

int analyzer() {
    lexLen = 0;
    getNonBlank();
    switch(charClass) {
        case LETTER:
        do {
            addChar();
            getChar();
        } while (charClass == LETTER || charClass == DIGIT);
        nextToken = keyword();
        break;
        case ESCCHAR:
            addChar();
            getChar();
            if (nextChar != 'n' || nextChar != 't' || nextChar != '\\')
                nextToken = UNKNOWN;
            else addChar();
            break;
        case DIGIT:
        int dotCounter = 0;
        do {
            addChar();
            getChar();
            if (nextChar == '.' && !dotCounter)
                dotCounter = 1;
            else if (nextChar == '.' && dotCounter)
                break;
        } while (charClass == DIGIT || nextChar == '.');
        nextToken = CONSTANT;
        break;
        case NEWLINE:
        addChar();
        getChar();
        nextToken = NEWLINE;
        lexeme[0] = '\\';
        lexeme[1] = 'n';
        break;
        case HASH:
        do {
            addChar();
            getChar();
        } while (charClass == LETTER);
        if (lexeme[1] == 'i' && lexeme[2] == 'n' &&
            lexeme[3] == 'c' && lexeme[4] == 'l' &&
            lexeme[5] == 'u' && lexeme[6] == 'd' &&
            lexeme[7] == 'e') {
            nextToken = INCLUDE;
        } else {
            nextToken = UNKNOWN;
        } break;
        case UNKNOWN:
        addChar();
        if ((nextToken = opNoPrefix()) == UNKNOWN) {
            getChar();
            if (nextChar != 32)
                addChar();
            nextToken = opWPrefix();
        }
        getChar();
        break;
        case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        break;
    }
    printf("Next token is : %d, next lexeme is %s\n", nextToken, lexeme);
    fprintf(lexemeFile, "%-15s%-15s", symTab(nextToken), lexeme);
    for (size_t i = 0; i < lexLen; i++){
        lexeme[i] = '\0';
    }
    fprintf(lexemeFile, "\n");
    return nextToken;
}