#include "string.h"

#include "alloc.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>

spp_str_ptr new_string(spp_int size) {
  spp_str_ptr str = alloc(sizeof(*str));
  str->len = size;
  str->str = alloc((str->len + 1) * sizeof(*str->str));
  str->str[str->len] = 0; // set null
  return str;
}
// #define STRING_MAX_SIZE 32
// spp_str strFromInt(spp_int x) {
//   spp_str str = new_string(STRING_MAX_SIZE);
//   sprintf(str->str, "%lld", x);
//   return str;
// }

spp_str str_concat(spp_str this, spp_str x) {
  // only need 1 \0
  spp_str_ptr s = new_string(this.len + x.len);
  int i;
  for(i = 0; i < this.len; i++) {
    s->str[i] = this.str[i];
  }
  int j;
  for(j = 0; j < x.len; j++) {
    s->str[j + i] = x.str[j];
  }

  return *s;
}

#define BUFFER_SIZE 10
spp_str int_toString(spp_int this) {
  spp_str_ptr s = new_string(BUFFER_SIZE);
  sprintf(s->str, "%lld", this);
  return *s;
}
