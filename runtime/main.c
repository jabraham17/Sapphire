
#include "string.h"
#include "types.h"
#include "alloc.h"
#include <string.h>
extern spp_int spp_main(spp_str arg);

int main(int argc, const char** argv, const char** envp) {

  int nStrings = argc-1;
  spp_str strings = alloc(nStrings*sizeof(*strings));
  for(int i = 0; i < nStrings; i++) {
    strings[i].str = (char*)argv[i+1];
    strings[i].len = strlen(argv[i+1]);
  }
  
  return (int)spp_main(strings);
}
