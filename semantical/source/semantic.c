#include "scan.c"

void main() {
    symbolTableSearch();
    if ((fptr = fopen(fileName, "r")) == NULL) {
        printf("ERROR - symbol_table missing\n");
        printf("%s\n", fileName);
    } else {
        do {
            getToken(&isFirstToken);
            semantic(currToken);
        } while (currToken != -1);
        return;
    }
}

void getToken(bool* start) {
    currToken = 0;
    if (*start) {
        fseek(fptr, 79, SEEK_SET);
        *start = 0;
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

void semantic(int token) {
    
}