#include "token.c"

void main() {
    if ((fptr = fopen("program.cmin", "r")) == NULL) {
        printf("ERROR - file not found\n");
    } else {
        getChar();
        do {
            analyzer();
        } while(nextToken != EOF);
    }
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

void analyzer() {
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
        case DIGIT:
        bool dotCounter = 0;
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
    for (size_t i = 0; i < lexLen; i++)
        lexeme[i] = '\0';
}