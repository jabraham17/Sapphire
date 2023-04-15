#include "ASTNode.h"

namespace ast {
namespace node {
ASTNode::ASTNode() : lineNumber(-1){};
long ASTNode::line() { return lineNumber; }
void ASTNode::setLine(long line) { this->lineNumber = line; }
} // namespace node
} // namespace ast
