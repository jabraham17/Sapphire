#ifndef SAPPHIRE_RUNTIME_TYPES_H_
#define SAPPHIRE_RUNTIME_TYPES_H_

typedef long long spp_int;
typedef unsigned long long spp_uint;
typedef double spp_real;
typedef unsigned char spp_byte;
typedef unsigned char spp_bool;
typedef void spp_nil;

typedef struct {
  char* str;
  spp_int len;
} spp_str, *spp_str_ptr;

typedef struct {
  spp_str_ptr elements;
  spp_int len;
} spp_str_array, *spp_str_array_ptr;

#endif
