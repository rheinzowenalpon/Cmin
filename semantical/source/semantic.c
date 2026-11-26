#include "scan.c"

//Reference functions

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
    if (currToken == 4) currentLine++;
}

void panic(char syncToken) {
    if((syncToken != currToken) || (currToken != -1) || (currToken != 4)){
        getToken();
        panic(syncToken);
    } else return;
}

//PROGRAM CLASS
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
//BLOCK
void block() {
    statement();
    if (currToken == 15 || currToken == 29 || currToken == 16) {
        getToken();
        if (currToken == 4) {
                return;
        } else {
            printf("syntax error: invalid line terminator (line %d)\n", currentLine);
            getToken();
            panic(4);
        }
    }
    if (currToken == 63) {
        return;
        } else {
            printf("syntax error: no closing bracket found for block (line %d)\n", currentLine); 
            getToken();
            panic(63);
    } //else statement();
}
//STATEMENT
void statement() {
    if (currToken == 14) {
        getToken();
        loop();
    } else if (currToken == 12) {
        getToken();
        conditionals();
    } else if (currToken == 17) {
        getToken();
        error();
    } else if (currToken == 4) {
        getToken();
        return;
    } else {
        getToken();
        line();
    }
}
//LINE
void line() {
    if (currToken == 19) {
        getToken();
        declaration();
    } else if (currToken == 20) {
        getToken();
        if (currToken == 19) {
            getToken();
            declaration();
        } else declaration();
    } else if ((currToken > 20 && currToken < 27) && 
        currToken != 24 || currToken == 72) {
        getToken();
        lower_declaration();
    } else if (currToken == 10) {
        getToken();
        variable();
    } else {
        getToken();
        factor();
        if (currToken == 4) {
            getToken();
            return;
        } else {
            printf("syntax error: invalid line terminator (line %d)", currentLine);
            getToken();
            panic(4);
        }
    }
}
//FACTOR
void factor() {
    if (currToken == 10) {
        variable();
    } else if (currToken == 11) {
        getToken();
        if (currToken == 4 || currToken == -1) {
            printf("SUCESS");
        } else printf("ERROR");
    } else if (currToken == 27 || currToken == 28) {
        getToken();
        if (currToken == 4 || currToken == -1) {
            printf("SUCESS");
        } else printf("ERROR");
    } else if (currToken == 65) {
        getToken();
        if (currToken == 10) {
            getToken();
            if (currToken == 65) {
                printf("SUCCESS");
            } else printf("ERROR");
        } else printf("ERROR");
        if (currToken == 4 || currToken == -1) {
            printf("SUCESS");
        } else printf("ERROR");
    } else if (currToken == 24) {
        printf("SUCESS");
    } else if (currToken == 66) {
        getToken();
        if (currToken == 10) {
            getToken();
            if (currToken == 66) {
                printf("SUCCESS");
            } else printf("ERROR");
        } else printf("ERROR");
    } else if (currToken == 62) {
        getToken();
        array();
    } else if (currToken == 70) {
        getToken();
        interpolation();
    } else {
        getToken();
        expression();
    }
}
//INTERPOLATION
void interpolation(){
    getToken();
    if (currToken == 10) {
        return;
    } else {
        printf("syntax error: expected an identifier to interpolate (line %d)", currentLine);
        getToken();
        panic(10);
    }
}
//LOOP
void loop() {
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
                } else {
                    printf("syntax error: define new line before defining the body of while (line %d)\n", currentLine);
                    getToken();
                    panic(4);
                }
            } else {
                printf("syntax error: '{' is missing (line %d)\n", currentLine);
                getToken();
                panic(62);
            }
        } else {
            printf("syntax error: ')' is missing (line %d)\n", currentLine);
            getToken();
            panic(59);
        }
    } else {
        printf("syntax error: '(' is missing (line %d)\n", currentLine);
        getToken();
        panic(58);
    }
}
//CONDITIONALS
void conditionals() {
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
                    if (currToken == 13) {
                        getToken();
                        if (currToken == 12) {
                            getToken();
                            conditionals();
                        } else if (currToken == 63) {
                            getToken();
                            block();
                        } else {
                            printf("syntax error: expected a '{' (line %d)\n", currentLine);
                            getToken();
                            panic(4);
                        }
                    } else return;
                } else {
                    printf("syntax error: must enter a newline after '{' before defining a conditional body (line %d)\n", currentLine);
                    getToken();
                    panic(4);
                }
            } else {
                printf("syntax error: expected a '{' (line %d)\n", currentLine);
                getToken();
                panic(4);
            }
        } else {
            printf("syntax error: ')' is expected for conditional clause (line %d)\n", currentLine);
            getToken();
            panic(4);
        }
    } else {
        printf("syntax error: '(' is expected for conditional clause (line %d)\n", currentLine);
        getToken();
        panic(4);
    }
}
//ERROR
void error() {
    if (currToken == 62) {
        getToken();
        if (currToken == 4) {
            getToken();
            block();
            if (currToken == 18) {
                getToken();
                if (currToken == 62) {
                    getToken();
                    if (currToken == 4) {
                        getToken();
                        block();
                    } else {
                        printf("syntax error: must enter a newline after '{' before defining an error handling body (line %d)\n", currentLine);
                        getToken();
                        panic(4);
                    }
                } else {
                    printf("syntax error: expected a '{' (line %d)\n", currentLine);
                    getToken();
                    panic(62);
                }
            } else {
                printf("syntax error: 'but' block for error handling is missing (line %d)\n", currentLine);
                getToken();
                panic(14);
            }
        } else {
            printf("syntax error: must enter a newline after '{' before defining an error handling body (line %d)\n", currentLine);
            getToken();
            panic(4);
        }
    } else {
        printf("syntax error: expected a '{' (line %d)\n", currentLine);
        getToken();
        panic(62);
    }
}
//MAIN CLASS
void main_() {
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
                        if (currToken == 63)
                            return;
                        else {
                            printf("syntax error: '}' is expected to close main (line %d)\n", currentLine);
                            getToken();
                            panic(63);
                        }
                    } else {
                        printf("syntax error: must enter a newline after '{' before defining the function body (line %d)\n", currentLine);
                        getToken();
                        panic(4);
                    }
                }
            } else {
                printf("syntax error: ')' is expected for main's clause (line %d)\n", currentLine);
                getToken();
                panic(59);
            }
        } else {
            printf("syntax error: '(' is expected for main's clause (line %d)\n", currentLine);
            getToken();
            panic(58);
        }
    } else {
        printf("syntax error: program no entry point. please define main\n");
        getToken();
        panic(31);
    }
}
//OUTER CLASS
void outers() {
    if (currToken == 4) {
        getToken();
        outers();
    } else if (currToken == -1)
        return;
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
//ARRAY
void array() {
    expression();
    getToken();
    if (currToken == 64) {
        getToken();
        array();
    } else if (currToken == 63) {
        getToken();
        if (nextToken == 4 || nextToken == -1) {
            return;
        } else {
            printf("syntax error: invalid line terminator (line %d)", currentLine);
            getToken();
            panic(4);
        }
    } else return;
}
//DATA_TYPE
void data_type() {
    if (currToken == 60) {
        getToken();
        if (currToken == 61) {
            getToken();
            data_type();
        } else {
            printf("syntax error: invalid data type (line %d)\n", currentLine);
            getToken();
            panic(61);
        }
    } else return;
}
//FUNCTION
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
                block();
                return;
            } else {
                printf("syntax error: must enter a newline after '{' before defining the function body (line %d)\n", currentLine);
                getToken();
                panic(4);
            }
        } else if (currToken == 4) {
            return;
        } else {
            printf("syntax error: must enter a '{' or terminate with newline (line %d)\n", currentLine);
            getToken();
            panic(4);
        }
    } printf("syntax error: invalid function identifier (line %d)\n", currentLine);
}
//FUNCTION-DECLARATION
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
                factor();
            } else if (currToken == 4)
                return;
            else {
                printf("syntax error: invalid syntax; must either be a function or a declaration (line %d)\n", currentLine);
                getToken();
                panic(4);
            }
        } else {
            printf("syntax error: invalid identifier (line %d)\n", currentLine);
            getToken();
            panic(10);
        }
    } else {
        printf("syntax error: invalid data type (line %d)\n", currentLine);
        getToken();
        panic(4);
    }
}
//STRUCT_BLOCK
void struct_block() {
    if (currToken == 10) {
        getToken();
        if (currToken == 62) {
            getToken();
            if (currToken == 4) {
                getToken();
                struct_();
            } else {
                printf("syntax error: must enter a newline after '{' before defining the struct body (line %d)\n", currentLine);
                getToken();
                panic(4);
            }
        } else {
            printf("syntax error: no opening bracket found for stuct (line %d)\n", currentLine);
            getToken();
            panic(62);
        }
    } else {
        printf("syntax error: invalid struct identifier (line %d)\n", currentLine);
        getToken();
        panic(10);
    }
}
//STRUCT
void struct_() {
    if (currToken == 63) {
        return;
    } else if (((currToken > 20 && currToken < 27) &&
        currToken != 24) || currToken == 72) {
        getToken();
        data_type();
        if (currToken == 10) {
            getToken();
            if (currToken == 4) {
                getToken();
                struct_();
            } else {
                printf("syntax error: invalid line termination (line %d)\n", currentLine);
                getToken();
                panic(4);
            }
        } else {
            printf("syntax error: invalid struct identifier (line %d)\n", currentLine);
            getToken();
            panic(10);
        }
    } else {
        printf("syntax error: no closing bracket found for struct (line %d)\n", currentLine); 
        getToken();
        panic(63);
    }
}
//LOWER DECLARATION
void lower_declaration() {
    data_type();
    if (currToken == 10) {
        getToken();
        if (currToken == 45) {
            getToken();
            factor();
        } else if (currToken == 4)
            return;
        else {
            printf("syntax error: invalid initializer for declaration (line %d)\n", currentLine);
            getToken();
            panic(4);           
        }
    } else {
        printf("syntax error: invalid declaration identifier (line %d)\n", currentLine);
        getToken();
        panic(10);
    }
}
//DECLARATION
void declaration() {
    if ((currToken > 20 && currToken < 27) && 
        currToken != 24 || currToken == 72) {
        getToken();
        data_type();
        if (currToken == 10) {
            getToken();
            if (currToken == 45) {
                getToken();
                factor();
            } else if (currToken == 4)
                return;
            else {
                printf("syntax error: invalid initializer for declaration (line %d)\n", currentLine);
                getToken();
                panic(4);           
            }
        } else {
            printf("syntax error: invalid declaration identifier (line %d)\n", currentLine);
            getToken();
            panic(10);
        }
    } else {
        printf("syntax error: invalid data type (line %d)\n", currentLine);
        getToken();
        panic(4);
    }
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
                    return;
                else {
                    printf("syntax error: invalid line termination (line %d)\n", currentLine);
                    getToken();
                    panic(4);
                }
            } else {
                printf("syntax error: invalid include file extension identifier (line %d)\n", currentLine);
                getToken();
                panic(10);
            }
        } else {
            printf("syntax error: no include file extension identifier found (line %d)\n", currentLine);
            getToken();
            panic(2);
        }
    } else {
        printf("syntax error: invalid include file name identifier found (line %d)\n", currentLine);
        getToken();
        panic(10);
    }
}
//LINE
void line() {

}
//EXPRESSION
void expression() {
    if (currToken == 58) {
        getToken();
        lower_assignment();
        if (currToken == 59) {
            return;
        } else {
            printf("syntax error: ')' is missing (line %d)\n", currentLine);
            getToken();
            panic(59);
        }
    } else lower_assignment();
}
//OPERATOR PRECEDENCE
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
            return;
        } else if (currToken == 58) {
            getToken();
            call();
        } else {
            printf("syntax error: missing identifier\n", currentLine);
            getToken();
            panic(10);
        }
    } else factor();
}