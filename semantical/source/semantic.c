#include "scan.c"
#include <stdio.h>
#include <ctype.h>
 
static long tokenCount = 0;
static int  foundMain  = 0;

/* ---------- prototypes ---------- */
void getToken(void);
static int  expect(int tok, const char *msg);
static void terminator(void);
static int  open_body(const char *what);
static int  isType(int t);
 
int main(void) {
    symbolTableSearch();
    if ((fptr = fopen(fileName, "r")) == NULL) {
        printf("ERROR - no file found\n");
        printf("%s\n", fileName);
        return 1;
    }
    if (!debugVal) {
        printf("ERROR - symbol_table.txt not found\n");
        fptrErr = fopen("../bin/err.txt", "w");
        if (fptrErr) {
            fprintf(fptrErr, "ERROR - unknown file extension found\n");
            fclose(fptrErr);
        }
        fclose(fptr);
        return 1;
    }
    if ((fptrErr2 = fopen("../bin/err.txt", "r")) != NULL) {
        printf("error - unknown token found\n");
        fclose(fptrErr2);
        return 1;
    }
    getToken();      /* load the first lookahead token */
    program();
    fclose(fptr);
    return 0;
}
 
/* ---------- helpers ---------- */
void getToken(void) {
    int c1, c2;
    if (currToken == -1 && !isFirstToken) return;   /* stay at end of input */
 
    if (isFirstToken) {
        fseek(fptr, 79, SEEK_SET);
        isFirstToken = 0;
    } else {
        fseek(fptr, 45, SEEK_CUR);
    }
 
    c1 = fgetc(fptr);
    if (c1 == EOF || c1 == '-') {
        currToken = -1;
    } else {
        currToken = c1 - '0';
        c2 = fgetc(fptr);
        if (c2 != EOF && !isspace(c2)) {
            currToken = currToken * 10 + (c2 - '0');
        }
    }
    tokenCount++;
    printf("Finished reading token %d\n", currToken);
    if (currToken == 4) currentLine++;
}
 
/* Skip ahead to the sync token, a newline, or end of input (does not consume it). */
void panic(char syncToken) {
    while (currToken != syncToken && currToken != 4 && currToken != -1)
        getToken();
}
 
/* Consume tok, or report an error and try to recover at tok.
   Returns 1 if the token was found/recovered, 0 if the caller should give up. */
static int expect(int tok, const char *msg) {
    if (currToken == tok) {
        getToken();
        return 1;
    }
    printf("syntax error: %s (line %d)\n", msg, currentLine);
    panic(tok);
    if (currToken == tok) {
        getToken();
        return 1;
    }
    return 0;
}
 
/* A statement must end with a newline (or end of file). Consumes the newline. */
static void terminator(void) {
    if (currToken == 4) {
        getToken();
    } else if (currToken == -1) {
        return;
    } else {
        printf("syntax error: invalid line terminator (line %d)\n", currentLine);
        panic(4);
        if (currToken == 4) getToken();
    }
}
 
/* '{' followed by a newline, before a body. */
static int open_body(const char *what) {
    if (!expect(62, "'{' is expected")) return 0;
    if (currToken != 4 && currToken != -1) {
        printf("syntax error: must enter a newline after '{' before defining %s (line %d)\n",
               what, currentLine);
        panic(4);
    }
    if (currToken == 4) getToken();
    return 1;
}
 
static int isType(int t) {
    return (t > 20 && t < 27 && t != 24) || t == 72;
}
 
/* ---------- PROGRAM ---------- */
void program(void) {
    while (currToken != -1) {
        long before = tokenCount;
        outers();
        if (tokenCount == before && currToken != -1) getToken();
    }
    if (!foundMain)
        printf("syntax error: program no entry point. please define main\n");
}
 
/* ---------- BLOCK (called after '{' and its newline) ---------- */
void block(void) {
    block_contents();
    if (currToken == 63) {
        getToken();
    } else {
        printf("syntax error: '}' is expected to close the block (line %d)\n", currentLine);
        panic(63);
        if (currToken == 63) getToken();
    }
}
 
void block_contents(void) {
    while (currToken != 63 && currToken != -1) {
        long before = tokenCount;
        statement();
        if (tokenCount == before) getToken();   /* guarantee progress */
    }
}
 
/* ---------- STATEMENT ---------- */
void statement(void) {
    if (currToken == 14) {              /* while */
        getToken();
        loop();
    } else if (currToken == 12) {       /* if */
        getToken();
        conditionals();
    } else if (currToken == 17) {       /* try */
        getToken();
        error();
    } else if (currToken == 4) {        /* blank line */
        getToken();
    } else if (currToken == 15) {       /* return [expr] */
        getToken();
        if (currToken != 4 && currToken != -1) expression();
        terminator();
    } else if (currToken == 16 || currToken == 29) {   /* break / continue */
        getToken();
        terminator();
    } else {
        line();
    }
}
 
/* ---------- LINE ---------- */
void line(void) {
    if (currToken == 19) {
        getToken();
        declaration();
    } else if (currToken == 20) {
        getToken();
        if (currToken == 19) getToken();
        declaration();
    } else if (isType(currToken)) {
        declaration();
    } else {                            /* expression statement: x = 1, x++, f(a) ... */
        factor();
        terminator();
    }
}
 
/* ---------- VARIABLE (suffixes after an identifier: [i]  .member  interpolation) ---------- */
void variable(void) {
    for (;;) {
        if (currToken == 60) {
            getToken();
            expression();
            if (!expect(61, "indexing must close with a ']'")) return;
        } else if (currToken == 2) {
            getToken();
            if (!expect(10, "expected an identifier after '.'")) return;
        } else if (currToken == 70) {
            getToken();
            interpolation();
        } else {
            return;
        }
    }
}
 
/* ---------- FACTOR ---------- */
void factor(void) {
    expression();
}
 
/* ---------- INTERPOLATION (after the interpolation token was consumed) ---------- */
void interpolation(void) {
    expect(10, "expected an identifier to interpolate");
}
 
/* ---------- LOOP (after 'while' consumed) ---------- */
void loop(void) {
    if (!expect(58, "'(' is missing")) return;
    expression();
    if (!expect(59, "')' is missing")) return;
    if (!open_body("the body of while")) return;
    block();
}
 
/* ---------- CONDITIONALS (after 'if' consumed) ---------- */
void conditionals(void) {
    if (!expect(58, "'(' is expected for conditional clause")) return;
    expression();
    if (!expect(59, "')' is expected for conditional clause")) return;
    if (!open_body("a conditional body")) return;
    block();
    if (currToken == 13) {              /* else */
        getToken();
        if (currToken == 12) {          /* else if */
            getToken();
            conditionals();
        } else {
            if (!open_body("an else body")) return;
            block();
        }
    }
}
 
/* ---------- ERROR HANDLING (after 'try' consumed): try { } but { } ---------- */
void error(void) {
    if (!open_body("an error handling body")) return;
    block();
    if (!expect(18, "'but' block for error handling is missing")) return;
    if (!open_body("an error handling body")) return;
    block();
}
 
/* ---------- MAIN (after 'main' consumed) ---------- */
void main_(void) {
    foundMain = 1;
    if (!expect(58, "'(' is expected for main's clause")) return;
    if (!expect(59, "')' is expected for main's clause")) return;
    if (!open_body("the function body")) return;
    block();
}
 
/* ---------- OUTER (top level) ---------- */
void outers(void) {
    if (currToken == 4) {
        getToken();
    } else if (currToken == -1) {
        return;
    } else if (currToken == 30) {
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
        if (currToken == 19) getToken();
        declaration();
    } else {
        func_decl();
    }
}
 
/* ---------- ARRAY (after '{' consumed) ---------- */
void array(void) {
    expression();                       /* nested '{' handled in postfix() */
    while (currToken == 64) {
        getToken();
        expression();
    }
    expect(63, "'}' is expected to close the array");
}
 
/* ---------- DATA TYPE ( [] [] ... ) ---------- */
void data_type(void) {
    while (currToken == 60) {
        getToken();
        if (!expect(61, "invalid data type")) return;
    }
}
 
/* ---------- PARAMETERS (after '(' consumed; may be empty) ---------- */
void declaration_parameter(void) {
    if (currToken == 59) return;
    for (;;) {
        if (!isType(currToken)) {
            printf("syntax error: invalid data type for parameter (line %d)\n", currentLine);
            panic(59);
            return;
        }
        getToken();
        data_type();
        if (!expect(10, "invalid parameter identifier")) return;
        if (currToken == 64) getToken();
        else return;
    }
}
 
/* ---------- FUNCTION (after '(' consumed) ---------- */
void function(void) {
    declaration_parameter();
    if (!expect(59, "')' is expected to close the parameter list")) return;
    if (currToken == 62) {
        if (!open_body("the function body")) return;
        block();
    } else if (currToken == 4 || currToken == -1) {
        terminator();                   /* prototype */
    } else {
        printf("syntax error: must enter a '{' or terminate with newline (line %d)\n", currentLine);
        panic(4);
        terminator();
    }
}
 
/* ---------- FUNCTION / MAIN / GLOBAL DECLARATION (top level, starts at a type) ---------- */
void func_decl(void) {
    if (!isType(currToken)) {
        printf("syntax error: invalid data type (line %d)\n", currentLine);
        panic(4);
        return;
    }
    getToken();
    data_type();
    if (currToken == 31) {              /* main */
        getToken();
        main_();
    } else if (currToken == 10) {
        getToken();
        if (currToken == 58) {
            getToken();
            function();
        } else if (currToken == 45) {
            getToken();
            factor();
            terminator();
        } else if (currToken == 4 || currToken == -1) {
            terminator();
        } else {
            printf("syntax error: invalid syntax; must either be a function or a declaration (line %d)\n",
                   currentLine);
            panic(4);
            terminator();
        }
    } else {
        printf("syntax error: invalid identifier (line %d)\n", currentLine);
        panic(10);
    }
}
 
/* ---------- STRUCT (after 'struct' consumed) ---------- */
void struct_block(void) {
    if (!expect(10, "invalid struct identifier")) return;
    if (!open_body("the struct body")) return;
    struct_();
}
 
void struct_(void) {
    for (;;) {
        if (currToken == 4) {
            getToken();
        } else if (currToken == 63) {
            getToken();
            return;
        } else if (isType(currToken)) {
            getToken();
            data_type();
            if (expect(10, "invalid struct identifier"))
                terminator();
        } else {
            printf("syntax error: no closing bracket found for struct (line %d)\n", currentLine);
            panic(63);
            if (currToken == 63) getToken();
            return;
        }
    }
}
 
/* ---------- DECLARATIONS ---------- */
/* after the type token was consumed */
void lower_declaration(void) {
    data_type();
    if (!expect(10, "invalid declaration identifier")) return;
    if (currToken == 45) {
        getToken();
        factor();
        terminator();
    } else if (currToken == 4 || currToken == -1) {
        terminator();
    } else {
        printf("syntax error: invalid initializer for declaration (line %d)\n", currentLine);
        panic(4);
        terminator();
    }
}
 
/* starts at the type token */
void declaration(void) {
    if (!isType(currToken)) {
        printf("syntax error: invalid data type (line %d)\n", currentLine);
        panic(4);
        return;
    }
    getToken();
    lower_declaration();
}
 
/* ---------- INCLUDE (after include keyword consumed): name.ext ---------- */
void include(void) {
    if (!expect(10, "invalid include file name identifier found")) return;
    if (!expect(2,  "no include file extension identifier found")) return;
    if (!expect(10, "invalid include file extension identifier")) return;
    terminator();
}
 
/* ---------- EXPRESSIONS ---------- */
void expression(void) {
    lower_assignment();
}
 
/* right-associative assignment: target (= += -= ...) expression */
void lower_assignment(void) {
    int startsWithIdent = (currToken == 10);
    logical_or();
    if (currToken >= 45 && currToken <= 49) {
        if (!startsWithIdent)
            printf("syntax error: invalid assignment target (line %d)\n", currentLine);
        getToken();
        lower_assignment();
    }
}
 
/* Binary levels are loops (left-associative). The originals recursed into
   themselves before consuming anything, which never terminates. */
void logical_or(void) {
    logical_and();
    while (currToken == 56) { getToken(); logical_and(); }
}
void logical_and(void) {
    binary_or();
    while (currToken == 55) { getToken(); binary_or(); }
}
void binary_or(void) {
    binary_xor();
    while (currToken == 41) { getToken(); binary_xor(); }
}
void binary_xor(void) {
    binary_and();
    while (currToken == 42) { getToken(); binary_and(); }
}
void binary_and(void) {
    symmetric_equality_comparison();
    while (currToken == 40) { getToken(); symmetric_equality_comparison(); }
}
void symmetric_equality_comparison(void) {
    assymmetric_equality_comparison();
    while (currToken == 54 || currToken == 57) { getToken(); assymmetric_equality_comparison(); }
}
void assymmetric_equality_comparison(void) {
    binary_bitwise();
    while (currToken > 49 && currToken < 54) { getToken(); binary_bitwise(); }
}
void binary_bitwise(void) {
    upper_arithmetic();
    while (currToken == 43 || currToken == 44) { getToken(); upper_arithmetic(); }
}
void upper_arithmetic(void) {
    lower_arithmetic();
    while (currToken == 32 || currToken == 33) { getToken(); lower_arithmetic(); }
}
void lower_arithmetic(void) {
    prefix();
    while (currToken > 33 && currToken < 37) { getToken(); prefix(); }
}
 
void prefix(void) {
    if (currToken == 73 || currToken == 39 || currToken == 32 || currToken == 33) {
        getToken();                     /* unary operator */
        prefix();
    } else if (currToken == 37 || currToken == 38) {   /* ++x  --x */
        getToken();
        if (!expect(10, "expected an identifier after '++'/'--'")) return;
        variable();
    } else {
        postfix();
    }
}
 
void postfix(void) {
    switch (currToken) {
    case 10:                            /* identifier, call, x++ */
        getToken();
        if (currToken == 58) {
            getToken();
            func_call();
        } else {
            variable();
            if (currToken == 37 || currToken == 38) getToken();
        }
        break;
    case 11: case 24: case 27: case 28: /* literals */
        getToken();
        break;
    case 65: case 66: {                 /* quoted literal: ' id '  or  " id " */
        int q = currToken;
        getToken();
        if (expect(10, "invalid literal"))
            expect(q, "unterminated literal");
        break;
    }
    case 58:                            /* ( expression ) */
        getToken();
        expression();
        expect(59, "')' is missing");
        break;
    case 62:                            /* { array literal } */
        getToken();
        array();
        break;
    default:
        printf("syntax error: invalid expression (line %d)\n", currentLine);
        panic(4);
        break;
    }
}
 
/* ---------- CALL (after '(' consumed) ---------- */
void func_call(void) {
    if (currToken != 59) input();
    expect(59, "missing closing parenthesis");
}
 
/* argument list: expr {, expr} */
void input(void) {
    expression();
    while (currToken == 64) {
        getToken();
        expression();
    }
}