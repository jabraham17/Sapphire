#include "string.h"

#include "alloc.h"

#include <stdio.h>
#include <stdlib.h>

// spp_str new_string(spp_int size) {
//   spp_str str = alloc(sizeof(*str));
//   str->len = size;
//   str->str = alloc(str->len * sizeof(*str->str));
//   return str;
// }
// #define STRING_MAX_SIZE 32
// spp_str strFromInt(spp_int x) {
//   spp_str str = new_string(STRING_MAX_SIZE);
//   sprintf(str->str, "%lld", x);
//   return str;
// }
