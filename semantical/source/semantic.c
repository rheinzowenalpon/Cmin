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
            outers();
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

/*

void main_() {
    getToken();
    if ((currToken > 20 && currToken < 27 && 
        currToken != 24) || currToken == 72) {
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
                            return; //BLOCK
                        } else printf("ERROR1");
                    }
                } else printf("ERROR1");
            } else printf("ERROR2");
        } else printf("ERROR3");
    } else printf("ERROR4");
}
*/

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

void outers() {
    getToken();
    if (currToken == -4) outers();
    if (currToken == -1) return;
    else if (currToken == 30) {
        getToken();
        include();
    } else if (currToken == 25) {
        getToken();
        struct_block();
    } else if (currToken == 19) {
        getToken();
        declaration();
    } else if (currToken == 20) {
        getToken();
        if (currToken == 19) {
            getToken();
            declaration();
        } else declaration();
    } else if (currToken == 10) {
        function();
    } else func_decl();
}

void func_decl() {
    if ((currToken > 20 && currToken < 27) && 
        currToken != 24 || currToken == 72) {
        getToken();
        data_type();
        if (currToken == 10) {
            getToken();
            if (currToken == 58) {
                getToken();
                function();
            } else if (currToken == 45) {
                getToken();
                value();
            } else if (currToken == 4)
                printf("SUCCESS");
            else printf("ERROR");
        } else printf("ERROR");
    } else printf("ERROR");
}

void function() {
    if (((currToken > 20 && currToken < 27) && 
        currToken != 24) || currToken == 72) {
        getToken();
        declaration_parameter();
    }
    if (currToken == 59) {
        getToken();
        if (currToken == 62) {
            getToken();
            if (currToken == 4) {
                printf("SUCCESS"); //Will be removed later
                return;
            } else printf("ERROR");
        }
    }
}

void struct_() {
    if (currToken == 63) {
        printf("SUCCESS");
    } else if (((currToken > 20 && currToken < 27) &&
        currToken != 24) || currToken == 72) {
        getToken();
        data_type();
        if (currToken == 10) {
            getToken();
            if (currToken == 4) {
                getToken();
                struct_();
            } else printf("ERROR");
        } else printf("ERROR");
    } else printf("ERROR");
}

void struct_block() {
    if (currToken == 10) {
        getToken();
        if (currToken == 62) {
            getToken();
            if (currToken == 4) {
                getToken();
                struct_();
            } else printf("ERROR");
        } else printf("ERROR");
    } else printf("ERROR");
}

void declaration_parameter() {
    data_type();
    if (currToken == 10) {
        getToken();
        if (currToken == 64) {
            getToken();
            if (((currToken > 20 && currToken < 27) && 
                currToken != 24) || currToken == 72) {
                getToken();
                declaration_parameter();
            } else printf("ERROR");
            declaration_parameter();
        } else return;
    } else printf("ERROR");
}

void declaration() {
    if ((currToken > 20 && currToken < 27) && 
        currToken != 24 || currToken == 72) {
        getToken();
        data_type();
        if (currToken == 10) {
            getToken();
            if (currToken == 45)
                printf("SUCCESS\n"); //should be value
            else if (currToken == 4)
                printf("SUCCESS\n");
            else printf("ERROR\n");
        } else printf("ERROR\n");
    } else printf("ERROR\n");
}

void data_type() {
    if (currToken == 60) {
        getToken();
        if (currToken == 61) {
            getToken();
            data_type();
        } else printf("ERROR");
    } else printf("SUCCESS");
}

//INCLUDE

void include() {
    if (currToken == 10) {
        getToken();
        if (currToken == 2) {
            getToken();
            if (currToken == 10) {
                getToken();
                if (currToken == 4 || currToken == -1)
                    printf("SUCCESS");
                else printf("ERROR");
            } else printf("ERROR");
        } else printf("ERROR");
    } else printf("ERROR");
}