#include "ast-dump-tree.h"

#include "ast/node/nodes.h"
#include "ast/symbol/function-symbol.h"
#include "ast/symbol/symbol.h"

namespace ast {
namespace visitor {

#define VISIT(type)                                                            \
  void ASTDumpTree::visitImpl([[maybe_unused]] node::type* arg)

VISIT(ASTNode) {
  strm << "<ast>" << strm.nl();
  strm.increaseIndent();
  for(auto c : arg->children()) {
    c->accept(this);
  }
  strm.decreaseIndent();
}
VISIT(Parameter) {
  strm << "<parameter:" << arg->symbol()->toString() << ">" << strm.nl();
}
VISIT(FunctionPrototype) {
  strm << "<func-proto>" << strm.nl();
  strm.increaseIndent();
  strm << "<func:" << arg->name() << ">" << strm.nl();
  strm.increaseIndent();
  for(auto p : arg->parameters()) {
    p->accept(this);
    strm << strm.nl();
  }
  strm.decreaseIndent();
  arg->type()->accept(this);
  strm.decreaseIndent();
}
VISIT(FunctionDefinition) {
  strm << "<func-def>" << strm.nl();
  strm.increaseIndent();
  arg->functionPrototype()->accept(this);
  arg->body()->accept(this);
  strm.decreaseIndent();
}
VISIT(ExternDefinition) {
  strm << "<extern-def>" << strm.nl();
  strm.increaseIndent();
  arg->functionPrototype()->accept(this);
  strm.decreaseIndent();
}
VISIT(InitDefinition) {
  strm << "<init-def>" << strm.nl();
  strm.increaseIndent();
  if(arg->isDeinit()) {
    strm << "<deinit>" << strm.nl();
  } else {
    strm << "<init>" << strm.nl();
    strm.increaseIndent();
    for(auto p : arg->parameters()) {
      p->accept(this);
      strm << strm.nl();
    }
    strm.decreaseIndent();
  }
  arg->body()->accept(this);
  strm.decreaseIndent();
}
VISIT(Scope) {
  strm << "<scope>" << strm.nl();
  strm.increaseIndent();
  for(auto s : arg->statements())
    s->accept(this);
  strm.decreaseIndent();
}
VISIT(ExpressionStatement) {
  strm << "<expr-stmt>" << strm.nl();
  strm.increaseIndent();
  arg->expression()->accept(this);
  strm.decreaseIndent();
}
VISIT(Statement) { strm << "<stmt>" << strm.nl(); }
VISIT(Expression) {
  strm << "<expr>" << strm.nl();
  strm.increaseIndent();
  arg->type()->accept(this);
  strm.decreaseIndent();
}

VISIT(DefExpression) {
  strm << "<def-expr:" << arg->symbol()->toString() << ">" << strm.nl();
  strm.increaseIndent();
  if(arg->hasInitialValue()) {
    arg->initialValue()->accept(this);
  }
  strm.decreaseIndent();
}
VISIT(ClassDefinition) {
  strm << "<class>" << strm.nl();
  strm.increaseIndent();
  arg->classType()->accept(this);

  strm << "<variables>" << strm.nl();
  strm.increaseIndent();
  for(auto v : arg->variables())
    v->accept(this);
  strm.decreaseIndent();

  strm << "<initializers>" << strm.nl();
  strm.increaseIndent();
  for(auto v : arg->initializers())
    v->accept(this);
  strm.decreaseIndent();

  strm << "<deinitializer>" << strm.nl();
  strm.increaseIndent();
  arg->deinitializer()->accept(this);
  strm.decreaseIndent();

  strm << "<functions>" << strm.nl();
  strm.increaseIndent();
  for(auto v : arg->functions())
    v->accept(this);
  strm.decreaseIndent();

  strm.decreaseIndent();
}
VISIT(OperatorDefinition) {
  strm << "<op-def>" << strm.nl();
  strm.increaseIndent();
  strm << "<op:" << node::getOperatorTypeString(arg->opType()) << ">"
       << strm.nl();
  strm.increaseIndent();
  for(auto p : arg->parameters()) {
    p->accept(this);
    strm << strm.nl();
  }
  strm.decreaseIndent();
  arg->body()->accept(this);
  strm.decreaseIndent();
}
VISIT(Type) { strm << "<type>" << strm.nl(); }
VISIT(PrimitiveType) {
  strm << "<prim-type:" << arg->toString() << ">" << strm.nl();
}
VISIT(ArrayType) {
  strm << "<array-type:" << arg->toString() << ">" << strm.nl();
}
// VISIT(TupleType) {
//   strm << "<tuple-type:" << arg->toString() << ">" << strm.nl();
// }
VISIT(CallableType) {
  strm << "<callable-type:" << arg->toString() << ">" << strm.nl();
}
VISIT(ClassType) {
  strm << "<class-type:" << arg->toString() << ">" << strm.nl();
}
VISIT(IfStatement) {
  strm << "<if>" << strm.nl();
  strm.increaseIndent();
  arg->expr()->accept(this);
  arg->ifBody()->accept(this);
  strm << "<else>" << strm.nl();
  if(arg->hasElseIf()) {
    arg->elseIfBody()->accept(this);
  } else if(arg->hasPlainElse()) {
    arg->plainElseBody()->accept(this);
  }
  strm.decreaseIndent();
}
VISIT(WhileStatement) {
  strm << "<while>" << strm.nl();
  strm.increaseIndent();
  arg->expr()->accept(this);
  arg->body()->accept(this);
  strm.decreaseIndent();
}
VISIT(ForStatement) {
  strm << "<for>" << strm.nl();
  strm.increaseIndent();
  arg->variable()->accept(this);
  arg->expr()->accept(this);
  arg->body()->accept(this);
  strm.decreaseIndent();
}
VISIT(ReturnStatement) {
  strm << "<return>" << strm.nl();
  strm.increaseIndent();
  arg->expression()->accept(this);
  strm.decreaseIndent();
}
// VISIT(Closure) {
//   strm << "<closure>" << strm.nl();
//   strm.increaseIndent();
//   arg->parameters()->accept(this);
//   arg->body()->accept(this);
//   strm.decreaseIndent();
// }
VISIT(CallExpression) {
  strm << "<call-expr>" << strm.nl();
  strm.increaseIndent();
  strm << "<op:" << node::getOperatorTypeString(arg->opType()) << ">"
       << strm.nl();
  strm.increaseIndent();
  for(auto o : arg->operands()) {
    o->accept(this);
    strm << strm.nl();
  }
  strm.decreaseIndent();
  strm.decreaseIndent();
}
VISIT(UseExpression) {
  strm << "<use-expr:(" << arg->symbol()->name() << ":"
       << arg->symbol()->type()->toString() << ")>" << strm.nl();
}
VISIT(IntExpression) {
  strm << "<int-expr:" << arg->value() << ">" << strm.nl();
}
VISIT(UIntExpression) {
  strm << "<uint-expr:" << arg->value() << ">" << strm.nl();
}
VISIT(RealExpression) {
  strm << "<real-expr:" << arg->value() << ">" << strm.nl();
}
VISIT(StringExpression) {
  strm << "<string-expr:\"" << arg->value() << "\">" << strm.nl();
}
VISIT(Nil) { strm << "<nil>" << strm.nl(); }

} // namespace visitor
} // namespace ast
