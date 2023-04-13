
#include "alloc.h"
#include "string.h"
#include "types.h"

#include <string.h>
extern spp_int spp_main(spp_str_array arg);

int main(int argc, const char** argv, const char** envp) {

  int nStrings = argc - 1;
  spp_str_array arg;
  spp_str_ptr strings = alloc(nStrings * sizeof(*strings));
  for(int i = 0; i < nStrings; i++) {
    strings[i].str = (char*)argv[i + 1];
    strings[i].len = strlen(argv[i + 1]);
  }
  arg.elements = strings;
  arg.len = nStrings;

  return (int)spp_main(arg);
}
