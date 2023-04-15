#ifndef SAPPHIRE_COMMON_INDENTED_OSTREAM_H_
#define SAPPHIRE_COMMON_INDENTED_OSTREAM_H_
#include <ostream>

namespace common {
class IndentedOStream {
private:
  std::ostream& os;
  const size_t indentLevel;
  size_t currentIndent;
  bool atLineStart;

public:
  void increaseIndent() { currentIndent += indentLevel; }
  void decreaseIndent() {
    if(currentIndent >= indentLevel) currentIndent -= indentLevel;
    else currentIndent = 0;
  }
  std::string getIndent() { return std::string(currentIndent, ' '); }

public:
  IndentedOStream(std::ostream& os, size_t indentLevel = 2)
      : os(os), indentLevel(indentLevel), currentIndent(0), atLineStart(true) {}
  ~IndentedOStream() = default;
  IndentedOStream(const IndentedOStream& other) = delete;
  IndentedOStream(IndentedOStream&& other) noexcept = default;
  IndentedOStream& operator=(const IndentedOStream& other) = delete;
  IndentedOStream& operator=(IndentedOStream&& other) noexcept = delete;

  template <typename T> IndentedOStream& operator<<(T&& x) {
    if(atLineStart) {
      atLineStart = false;
      os << getIndent();
    }
    os << std::forward<T>(x);
    return *this;
  }

  // special case for ostream inits
  using OStreamType = std::basic_ostream<char, std::char_traits<char>>;
  using OStreamManip = OStreamType& (*)(OStreamType&);
  IndentedOStream& operator<<(OStreamManip manip) {
    manip(os);
    return *this;
  }

  struct nl_;
  friend nl_;
  struct nl_ {
    IndentedOStream* ios;
    nl_(IndentedOStream* ios) : ios(ios) {}
    friend std::ostream& operator<<(std::ostream& o, nl_ x) {
      o << std::endl;
      x.ios->atLineStart = true;
      return o;
    }
  };
  nl_ nl() { return nl_(this); }
};

} // namespace common
#endif
