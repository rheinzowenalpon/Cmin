#ifndef _SET
#define _SET
#include "../header/set.h"
#endif

#include <string.h>
#include <stdlib.h>
#include "../header/dirent.h"

static int parseExt(const struct dirent* dir){
    if(!dir) return 0;
    if(dir->d_type == DT_REG) {
        const char *ext = strrchr(dir->d_name,'.');
        if((!ext) || (ext == dir->d_name))
           return 0;
        else {
            if(strcmp(ext, ".cmin") == 0)
                return 1;
        }
    }
    return 0;
}

void fileSearch(){
    struct dirent **fileToBeAnalyzed;
    int n = scandir("../../", &fileToBeAnalyzed, parseExt, alphasort);
    if (n < 0)
        *fileName = *"\0";
    else {
        short length = strlen(fileToBeAnalyzed[0]->d_name);
        sprintf(fileName, "%s%s", workPath, fileToBeAnalyzed[0]->d_name);
        free(fileToBeAnalyzed[0]);
        free(fileToBeAnalyzed);
    }
}