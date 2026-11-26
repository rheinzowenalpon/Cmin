#include "scan.c"

//Reference functions
void outers();
void data_type();
void func_decl();
void function();
void struct_();
void struct_block();
void declaration_parameter();
void declaration();
void include();
void value();
void program();
void main_();
void block();
void loop();
void panic();

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
            program();
        } while (currToken != -1);
        return;
    }
}

void panic(char syncToken) {
    if((syncToken != currToken) && (currToken != -1)){
        getToken();
        panic(syncToken);
    } else return;
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

/*
    Value = exp|constant|bool|string|char|ident|null|arr
            N/A|19      |26  |N/A   |72  |10   |24  |N/A
    ""

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



void expression() {

}

void data_type();
void modifier();
*/

//FUNCTION

void program() {
    getToken();
    if ((currToken > 20 && currToken < 27 && 
        currToken != 24) || currToken == 72) {
        getToken();
        main_();
    } else {
        outers();
    }
}

void main_() {
    getToken();
    data_type();
    if (currToken == 31) {
        getToken();
        if (currToken == 58) {
            getToken();
            if (currToken == 59) {
                getToken();
                if (currToken == 62) {
                    getToken();
                    if (currToken == 4) {
                        getToken();
                        block();
                        getToken();
                        if (currToken == 63)
                            printf("SUCCESS");
                        else printf("ERROR");
                    } else printf("ERROR1");
                }
            } else printf("ERROR");
        } else printf("ERROR");
    } else printf("ERROR");
}

void block() {
    
}

void variable() {
    if (currToken == 10) {
        getToken();
        if (getToken == 60) {
            expression();
            if (getToken == 61) {
                getToken();
                if (getToken == 2) {
                    getToken();
                    variable();
                } else if (currToken == 4 || currToken == -1) {
                    printf("SUCCESS");
                } else printf("ERROR");
            } else printf("ERROR");
        } else if (currToken == 4 || currToken == -1) {
            printf("SUCCESS");
        } else printf("ERRROR");
    } printf("SUCCESS");
}

void expression() {
    if (currToken == 58) {
        getToken();
        lower_assignment();
        if (currToken == 59) {
            printf("SUCCESS");
        } else printf("ERROR");
    } else lower_assignment();
}

void lower_assignment() {
    if (currToken == 10) {
        getToken();
        if (currToken > 45 && currToken < 50) {
            getToken();
        } else logical_or();
    } else logical_or();
}

void logical_or() {
    logical_or();
    if (currToken == 56) {
        getToken();
        logical_and();
    } else logical_and();
}

void logical_and() {
    logical_and();
    if (currToken == 55) {
        getToken();
        binary_or();
    } else binary_or();
}

void binary_or() {
    binary_or();
    if (currToken == 41) {
        getToken();
        binary_xor();
    } else binary_xor();
}

void binary_xor() {
    binary_xor();
    if (currToken == 42) {
        getToken();
        binary_and();
    } else binary_and();
}

void binary_and() {
    binary_and();
    if (currToken == 40) {
        getToken();
        equaltiy_comparison();
    } else equaltiy_comparison();
}

void symmetric_equality_comparison() {
    symmetric_equality_comparison();
    if (currToken == 54 || currToken == 57) {
        getToken();
        assymmetric_equality_comparison();
    } else symmetric_equality_comparison();
}

void assymmetric_equality_comparison() {
    assymmetric_equality_comparison();
    if (currToken > 49 && currToken < 54) {
        getToken();
        binary_bitwise();
    } else binary_bitwise();
}

void binary_bitwise() {
    binary_bitwise();
    if (currToken == 43 || currToken == 44) {
        getToken();
        upper_arithmetic();
    } else upper_arithmetic();
}

void upper_arithmetic() {
    upper_arithmetic();
    if (currToken == 32 || currToken == 33) {
        getToken();
        lower_arithmetic();
    } else lower_arithmetic();
}

void lower_arithmetic() {
    lower_arithmetic();
    if (currToken > 33 && currToken < 37) {
        getToken();
        prefix();
    } else prefix();
}

void prefix() {
    if (currToken == 73 || currToken < 39) {
        getToken();
        logical_or();
    } else if (currToken == 37 || currToken == 38) {
        getToken();
        variable();
    } else postfix();
}

void postfix() {
    if (currToken == 10) {
        if (currToken == 37 || currToken == 38) {
            printf("SUCCESS");
        } else if (currToken == 58) {
            getToken();
            call();
        } else printf("ERROR");
    } else factor();
}



void loop() {
    if (currToken == 14) {
        getToken();
        if (currToken == 58) {
            getToken();
            expression();
            if (currToken == 59) {
                getToken();
                if (currToken == 62) {
                    getToken();
                    if (currToken == 4) {
                        getToken();
                        block();
                    } else printf("ERROR");
                } else printf("ERROR");
            } else printf("ERROR");
        } else printf("ERROR");
    } else printf("ERROR");
}

void value() {
    if (currToken == 11) {
        getToken();
        if (currToken == 4 || currToken == -1) {
            printf("SUCESS");
        } else printf("ERROR");
    } else if (currToken == 26) {
        printf("SUCESS");
    } else if (currToken == 72) {
        printf("SUCESS");
    } else if (currToken == 10) {
        printf("SUCESS");
    } else if (currToken == 24) {
        printf("SUCESS");         
    } else if (currToken == 66) {
        //string();
    } else if (currToken == 62) {
        //array();
    } else {
        printf("ERROR");
        //expression();
    }

}

//Alpon.number

//Not Allowed:
//CONST GLOBAL
//Allowed:
//GLOBAL
//CONST
//GLOBAL CONST