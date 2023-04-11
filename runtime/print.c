#include <stdio.h>
struct spp_str {
    char* str;
    long long len;
};
void print(struct spp_str* str) {
    fputs(str->str, stdout);
}
