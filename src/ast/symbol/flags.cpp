#include "flags.h"

#include <cassert>

namespace ast {
namespace symbol {

std::string getString(Flag flag) {
  switch(flag) {
    case Flag::UNKNOWN: return "UNKNOWN";
#define FLAGS_CASE(name)                                                       \
  case Flag::name: return #name;
      FLAGS(FLAGS_CASE)
#undef FLAGS_CASE
  }
  assert(false && "unknown flag");
  return "";
}

std::string getFlagString(FlagSet flags) {
  std::string s;
  std::string sep;
  for(std::size_t i = 1; i <= flags.size(); i++) {
    if(flags.test(i)) {
      s = sep + getString((Flag)i);
      sep = ",";
    }
  }
  return s;
}
} // namespace symbol
} // namespace ast
