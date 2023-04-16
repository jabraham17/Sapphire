#include "parser.h"

namespace parser {

Context::Context() : errorMessage_() {}
void Context::addError(const char* s) { errorMessage_.push_back(s); }
void Context::addError(const std::string& s) { errorMessage_.push_back(s); }
bool Context::hasErrors() { return errorMessage_.size() > 0; }
std::vector<std::string> Context::errors() { return errorMessage_; }

} // namespace parser
