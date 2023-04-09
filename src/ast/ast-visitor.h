#ifndef SAPPHIRE_AST_AST_VISITOR_H_
#define SAPPHIRE_AST_AST_VISITOR_H_

#include "ast.h"

namespace ast {

struct ASTVisitorException : public ASTException {
  ASTVisitorException()
      : ASTVisitorException("Unknown AST Visitor Exception") {}
  ASTVisitorException(std::string message) : ASTException(message) {}
};

class ASTVisitor {
public:
  ASTVisitor() = default;
  virtual ~ASTVisitor() = default;
#define ast_funcs(type)                                                        \
protected:                                                                     \
  virtual void visit##type##Impl(type*) {}                                     \
                                                                               \
public:                                                                        \
  void visit##type(type* a) {                                                  \
    if(!is##type##Node(a)) {                                                   \
      throw ASTVisitorException(                                               \
          "Unexpected " #type " in visitor " + std::string(__FUNCTION__));     \
    }                                                                          \
    visit##type##Impl(a);                                                      \
  }
  ast_node_list(ast_funcs)
#undef ast_funcs
};

} // namespace ast

#endif
