#include "ast-dump.h"

namespace ast {
namespace visitor {

#define VISIT(type) void ASTDump::visitImpl([[maybe_unused]] node::type* arg)

VISIT(NodeList) {
  bool notFirstOne = false;
  for(auto a : *arg) {
    if(notFirstOne) strm << strm.nl();
    notFirstOne = true;
    a->accept(this);
  }
}
VISIT(Parameter) { strm << arg->symbol()->toString(); }
VISIT(FunctionPrototype) {
  strm << arg->name() << "(";
  std::string sep;
  for(auto p : *arg->parameters()) {
    strm << sep;
    p->accept(this);
    sep = ", ";
  }
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
    std::string sep;
    for(auto p : *arg->parameters()) {
      strm << sep;
      p->accept(this);
      sep = ", ";
    }
    strm << ")";
  }
  strm << " {" << std::endl;
  strm.increaseIndent();
  arg->body()->accept(this);
  strm.decreaseIndent();
  strm << strm.nl() << "}";
}
VISIT(Scope) { arg->statements()->accept(this); }
VISIT(ExpressionStatement) { arg->expression()->accept(this); }

VISIT(DefExpression) {
  strm << arg->symbol()->toString();
  if(arg->hasInitialValue()) {
    strm << " = ";
    arg->assignValue()->accept(this);
  }
}
VISIT(ClassDefinition) {
  strm << "class ";
  arg->classType()->accept(this);
  strm << " {" << strm.nl();
  strm.increaseIndent();

  arg->variables()->accept(this);
  strm << strm.nl();
  arg->initializers()->accept(this);
  strm << strm.nl();
  arg->deinitializer()->accept(this);
  strm << strm.nl();
  arg->functions()->accept(this);

  strm.decreaseIndent();
  strm << strm.nl() << "}";
}
VISIT(OperatorDefinition) { strm << "unimp op def"; }
VISIT(PrimitiveType) { strm << Type::getTypeString(arg); }
VISIT(ArrayType) { strm << Type::getTypeString(arg); }
VISIT(TupleType) { strm << Type::getTypeString(arg); }
VISIT(CallableType) { strm << Type::getTypeString(arg); }
VISIT(ClassType) { strm << Type::getTypeString(arg); }
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
VISIT(Closure) { strm << "unimp closure"; }
VISIT(CallExpression) {
  strm << "(";
  strm << ast::getOperatorTypeString(arg->opType());
  for(auto e : *arg->operands()) {
    strm << ", ";
    e->accept(this);
  }
  strm << ")";
}
VISIT(UseExpression) {
  strm << "(" << arg->symbol()->name() << ":"
       << Type::getTypeString(arg->symbol()->type()) << ")";
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
