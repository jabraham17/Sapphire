#ifndef SAPPHIRE_AST_SYMBOL_FLAGS_H_
#define SAPPHIRE_AST_SYMBOL_FLAGS_H_

#include <bitset>
#include <string>

namespace ast {
namespace symbol {

#define FLAGS(F) F(RESOLVED)

// TODO: looks like another bug in clang-format with indentation
enum class Flag : std::size_t {
  UNKNOWN = 0,
#define FLAGS_CASE(name) name,
  FLAGS(FLAGS_CASE)
#undef FLAGS_CASE
};

#define COUNTER(name) +1
constexpr std::size_t NumberOfFlags = FLAGS(COUNTER);
#undef COUNTER

using FlagSet = std::bitset<NumberOfFlags>;

std::string getFlagString(FlagSet flags);

} // namespace symbol
} // namespace ast

#endif
