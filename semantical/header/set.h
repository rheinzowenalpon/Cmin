#ifndef _SET
#define _SET
#include "../../lexical/header/set.h"
#endif

#include "../../lexical/header/dirent.h"
#include <stdlib.h>
#include <ctype.h>

void getToken();
void semantic();
void match(int expectedToken);
int parseSpecial();

bool isFirstToken = 1;
int currToken;