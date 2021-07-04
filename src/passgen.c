#include "passgen.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

__attribute__((constructor))
static void init(void) {
    srand((unsigned)(time(NULL) * 201906270400  + 19810710134500));
}
