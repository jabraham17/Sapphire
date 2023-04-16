#include "ast-dump-tree.h"

#include "ast/node/nodes.h"
#include "ast/symbol/function-symbol.h"
#include "ast/symbol/symbol.h"

namespace ast {
namespace visitor {

#define VISIT(type)                                                            \
  void ASTDumpTree::visitImpl([[maybe_unused]] node::type* arg)

VISIT(NodeList) {
  strm << "<node-list>" << strm.nl();
  strm.increaseIndent();
  for(auto a : *arg) {
    a->accept(this);
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
  arg->parameters()->accept(this);
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
    arg->parameters()->accept(this);
  }
  arg->body()->accept(this);
  strm.decreaseIndent();
}
VISIT(Scope) {
  strm << "<scope>" << strm.nl();
  strm.increaseIndent();
  arg->statements()->accept(this);
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
    arg->assignValue()->accept(this);
  }
  strm.decreaseIndent();
}
VISIT(ClassDefinition) {
  strm << "<class>" << strm.nl();
  strm.increaseIndent();
  arg->classType()->accept(this);

  arg->variables()->accept(this);

  arg->initializers()->accept(this);

  arg->deinitializer()->accept(this);

  arg->functions()->accept(this);

  strm.decreaseIndent();
}
VISIT(OperatorDefinition) {
  strm << "<op-def>" << strm.nl();
  strm.increaseIndent();
  strm << "<op:" << node::getOperatorTypeString(arg->opType()) << ">"
       << strm.nl();
  arg->parameters()->accept(this);
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
VISIT(TupleType) {
  strm << "<tuple-type:" << arg->toString() << ">" << strm.nl();
}
VISIT(CallableType) {
  strm << "<callable-type:" << arg->toString() << ">" << strm.nl();
}
VISIT(ClassType) {
  strm << "<class-type:" << arg->toString() << ">" << strm.nl();
}
VISIT(TypeList) {
  strm << "<type-list>" << strm.nl();
  strm.increaseIndent();
  for(auto e : arg->elementTypes())
    e->accept(this);
  strm.decreaseIndent();
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
VISIT(Closure) {
  strm << "<closure>" << strm.nl();
  strm.increaseIndent();
  arg->parameters()->accept(this);
  arg->body()->accept(this);
  strm.decreaseIndent();
}
VISIT(CallExpression) {
  strm << "<call-expr>" << strm.nl();
  strm.increaseIndent();
  strm << "<op:" << node::getOperatorTypeString(arg->opType()) << ">"
       << strm.nl();
  arg->operands()->accept(this);
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
