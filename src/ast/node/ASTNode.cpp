#include "ASTNode.h"

namespace ast {
namespace node {
ASTNode::ASTNode() : line_(-1), parent_(nullptr){};
long ASTNode::line() { return this->line_; }
void ASTNode::setLine(long line) { this->line_ = line; }
} // namespace node
} // namespace ast
