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
            printf("%d\n", currToken);
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

/*
    Value = exp|constant|bool|string|char|ident|null|arr
            N/A|19      |26  |N/A   |72  |10   |24  |N/A
    ""
*/
void string() {
    getToken();
    if (currToken == 10) {
        string();
    } else if (currToken == 66) {
        printf("SUCCESS");
    } else {
        printf("ERROR");
    }
}

// {-1, +2, -3}

void postfix() {
    if (currToken == 32 || currToken == 33) {
        value();
    } else {
        factor();
        call();
    }
}

void array() {
    getToken();
    if (currToken == 62) {
        array();
    } else if (currToken == 32 || currToken == 33 || currToken == 39 || currToken == 73) { //comma
        postfix();
    } else if (currToken == 63) {
        printf("SUCCESS");
    } else {
        printf("ERROR");
    }
}

void value() {

    if (currToken == 19) {
        printf("SUCESS");
    } else if (currToken == 26) {
        printf("SUCESS");
    } else if (currToken == 72) {
        printf("SUCESS");
    } else if (currToken == 10) {
        printf("SUCESS");
    } else if (currToken == 24) {
        printf("SUCESS");         
    } else if (currToken == 66) {
        string();
    } else if (currToken == 62) {
        array();
    } else {
        expression();
    }

}

void return_(){
    getToken();
    if (currToken == 29){
        expression();
    } else {
        printf("ERROR"); // panic recovery temporarily a print statment
    }
}

//Alpon.number

//Not Allowed:
//CONST GLOBAL
//Allowed:
//GLOBAL
//CONST
//GLOBAL CONST