#include "scan.c"

void main() {
    symbolTableSearch();
    if ((fptr = fopen(fileName, "r")) == NULL) {
        printf("ERROR - no file found\n");
        printf("%s\n", fileName);
    } else {
        if (!debugVal) {
            printf("ERROR - symbol_table.txt not found\n");
            fptrErr = fopen("../bin/err.txt", "w");
            fprintf(fptrErr, "ERROR - unknown file extension found\n");
            fclose(fptrErr);
            return;
        }
        do {
            getToken();
            semantic();
        } while (currToken != -1);
        return;
    }
}

void getToken() {
    currToken = 0;
    if (isFirstToken) {
        fseek(fptr, 79, SEEK_SET);
        isFirstToken = 0;
    } else fseek(fptr, 45, SEEK_CUR);
    char temp[2];
    if ((temp[0] = fgetc(fptr)) == '-')
        currToken = -1;
    else {
        currToken = (temp[0] -= '0');
        if (!isspace(temp[1] = fgetc(fptr))) {
            currToken *= 10;
            currToken += (temp[1]) - '0';
        }
    }
}

void match(int expectedToken) {
    if (currToken == expectedToken) {
        getToken();
    } else {
        printf("SYNTAX ERROR: Expected token %s (%d), got %s (%d)\n",
               symTab(expectedToken), expectedToken, symTab(currToken), currToken);
        fptrErr = fopen("../bin/err.txt", "w");
        fprintf(fptrErr, "SYNTAX ERROR: Expected %s, got %s\n",
                symTab(expectedToken), symTab(currToken));
        fclose(fptrErr);
        exit(1);
    }
}

int parseSpecial() {
    switch (currToken) {
        case ESCCHAR:
            match(ESCCHAR);
            return 1;
        case NEWLINE:
            match(NEWLINE);
            return 1;
        case TAB:
            match(TAB);
            return 1;
        case S_QUOTE:
            match(S_QUOTE);
            return 1;
        case D_QUOTE:
            match(D_QUOTE);
            return 1;
        default:
            printf("SYNTAX ERROR in Special: Unexpected token %s (%d)\n",
                   symTab(currToken), currToken);
            return 0;
    }
}


void semantic() {
    if (currToken == ESCCHAR || currToken == NEWLINE || currToken == TAB ||
        currToken == S_QUOTE || currToken == D_QUOTE) {
        parseSpecial();
    } else if (currToken != EOF_) {
        getToken();
    }
}