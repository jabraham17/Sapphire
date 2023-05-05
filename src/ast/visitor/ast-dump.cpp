#include "ast-dump.h"

#include "ast/node/nodes.h"
#include "ast/symbol/function-symbol.h"
#include "ast/symbol/symbol.h"

namespace ast {
namespace visitor {

#define VISIT(type) void ASTDump::visitImpl([[maybe_unused]] node::type* arg)

VISIT(ASTNode) {
  acceptWithSep(arg->children_begin(), arg->children_end(), strm.nl());
}
VISIT(Parameter) { strm << arg->symbol()->toString(true); }
VISIT(FunctionPrototype) {
  strm << arg->symbol()->basename() << "(";
  auto p = arg->parameters();
  acceptWithSep(p.begin(), p.end(), ", ");
  strm << "): ";
  arg->type()->accept(this);
}
VISIT(FunctionDefinition) {
  arg->functionPrototype()->accept(this);
  strm << " {" << strm.nl();
  strm.increaseIndent();
  arg->body()->accept(this);
  strm.decreaseIndent();
  strm << strm.nl() << "}";
}
VISIT(ExternDefinition) {
  strm << "extern ";
  arg->functionPrototype()->accept(this);
}
VISIT(InitDefinition) {
  if(arg->isDeinit()) {
    strm << "deinit";
  } else {
    strm << "init(";
    auto p = arg->parameters();
    acceptWithSep(p.begin(), p.end(), ", ");
    strm << ")";
  }
  strm << " {" << std::endl;
  strm.increaseIndent();
  arg->body()->accept(this);
  strm.decreaseIndent();
  strm << strm.nl() << "}";
}
VISIT(Scope) {
  auto s = arg->statements();
  acceptWithSep(s.begin(), s.end(), strm.nl());
}
VISIT(ExpressionStatement) { arg->expression()->accept(this); }

VISIT(DefExpression) {
  strm << "var " << arg->symbol()->toString(true);
  if(arg->hasInitialValue()) {
    strm << " = ";
    arg->initialValue()->accept(this);
  }
}
VISIT(ClassDefinition) {
  strm << "class ";
  arg->classType()->accept(this);
  strm << " {" << strm.nl();
  strm.increaseIndent();

  auto v = arg->variables();
  acceptWithSep(v.begin(), v.end(), strm.nl());

  strm << strm.nl();

  auto i = arg->initializers();
  acceptWithSep(i.begin(), i.end(), strm.nl());

  strm << strm.nl();

  arg->deinitializer()->accept(this);

  strm << strm.nl();

  auto f = arg->functions();
  acceptWithSep(f.begin(), f.end(), strm.nl());

  strm.decreaseIndent();
  strm << strm.nl() << "}";
}
VISIT(OperatorDefinition) { strm << "unimp op def"; }
VISIT(PrimitiveType) { strm << arg->toString(); }
VISIT(ArrayType) { strm << arg->toString(); }
// VISIT(TupleType) { strm << arg->toString(); }
VISIT(CallableType) { strm << arg->toString(); }
VISIT(ClassType) { strm << arg->toString(); }
VISIT(IfStatement) {

  strm << "if ";
  arg->expr()->accept(this);
  strm << " {" << strm.nl();
  strm.increaseIndent();
  arg->ifBody()->accept(this);
  strm.decreaseIndent();
  strm << strm.nl() << "}";

  if(arg->hasElseIf()) {
    strm << " else ";
    arg->elseIfBody()->accept(this);
  } else if(arg->hasPlainElse()) {
    strm << " else {" << strm.nl();
    strm.increaseIndent();
    arg->plainElseBody()->accept(this);
    strm.decreaseIndent();
    strm << strm.nl() << "}";
  }
}
VISIT(WhileStatement) {
  strm << "while ";
  arg->expr()->accept(this);
  strm << " {" << strm.nl();
  strm.increaseIndent();
  arg->body()->accept(this);
  strm.decreaseIndent();
  strm << strm.nl() << "}";
}
VISIT(ForStatement) {
  strm << "for ";
  arg->variable()->accept(this);
  strm << " = ";
  arg->expr()->accept(this);
  strm << " {" << strm.nl();
  strm.increaseIndent();
  arg->body()->accept(this);
  strm.decreaseIndent();
  strm << strm.nl() << "}";
}
VISIT(ReturnStatement) {
  strm << "return ";
  arg->expression()->accept(this);
}
// VISIT(Closure) { strm << "unimp closure"; }
VISIT(CallExpression) {
  strm << "(";
  strm << node::getOperatorTypeString(arg->opType());
  for(auto e : arg->operands()) {
    strm << ", ";
    e->accept(this);
  }
  strm << "):" + arg->type()->toString();
}
VISIT(UseExpression) {
  strm << "(" << arg->symbol()->basename() << ":"
       << arg->symbol()->type()->toString() << ")";
}
VISIT(IntExpression) { strm << arg->value(); }
VISIT(UIntExpression) { strm << arg->value(); }
VISIT(RealExpression) { strm << arg->value(); }
VISIT(StringExpression) { strm << "\"" << arg->value() << "\""; }
VISIT(Nil) {
  if(arg->isUserSpecified()) strm << "nil";
}

} // namespace visitor
} // namespace ast
