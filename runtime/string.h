#ifndef SAPPHIRE_RUNTIME_STRING_H_
#define SAPPHIRE_RUNTIME_STRING_H_

struct spp_str_ {
    char* str;
    long long len;
};
typedef struct spp_str_* spp_str;


#endif
