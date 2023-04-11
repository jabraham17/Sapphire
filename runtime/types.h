#ifndef SAPPHIRE_RUNTIME_TYPES_H_
#define SAPPHIRE_RUNTIME_TYPES_H_

typedef long long spp_int;
typedef unsigned long long spp_uint;
typedef double spp_real;
typedef unsigned char spp_byte;
typedef unsigned char spp_bool;
typedef void spp_nil;

struct spp_str_ {
    char* str;
    long long len;
};
typedef struct spp_str_* spp_str;




#endif
