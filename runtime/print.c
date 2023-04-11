#include <stdio.h>
#include "string.h"
void print(spp_str str) {
    fputs(str->str, stdout);
}
