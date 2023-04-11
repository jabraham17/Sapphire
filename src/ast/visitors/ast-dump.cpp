#include "ast-dump.h"

namespace ast {
namespace visitor {

void ASTDump::visitImpl(NodeList* arg) {
  bool notFirstOne = false;
  for(auto a : *arg) {
    if(notFirstOne) strm << strm.nl();
    notFirstOne = true;
    a->accept(this);
  }
}
void ASTDump::visitImpl(Parameter* arg) { strm << arg->symbol()->toString(); }
void ASTDump::visitImpl(FunctionPrototype* arg) {
  strm << arg->name() << "(";
  std::string sep;
  for(auto p : *arg->parameters()) {
    strm << sep;
    p->accept(this);
    sep = ", ";
  }
  strm << "): ";
  arg->returnType()->accept(this);
}
void ASTDump::visitImpl(FunctionDefinition* arg) {
  arg->functionPrototype()->accept(this);
  strm << " {" << strm.nl();
  strm.increaseIndent();
  arg->body()->accept(this);
  strm.decreaseIndent();
  strm << strm.nl() << "}";
}
void ASTDump::visitImpl(ExternDefinition* arg) {
  strm << "extern ";
  arg->functionPrototype()->accept(this);
}
void ASTDump::visitImpl(InitDefinition* arg) {
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
void ASTDump::visitImpl(Scope* arg) { arg->statements()->accept(this); }
void ASTDump::visitImpl(ExpressionStatement* arg) {
  arg->expression()->accept(this);
}

void ASTDump::visitImpl(DefExpression* arg) {
  strm << arg->symbol()->toString();
  if(arg->hasInitialValue()) {
    strm << " = ";
    arg->assignValue()->accept(this);
  }
}
void ASTDump::visitImpl(ClassDefinition* arg) {
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
void ASTDump::visitImpl(OperatorDefinition* arg) { strm << "unimp op def"; }
void ASTDump::visitImpl(PrimitiveType* arg) {
  strm << Type::getTypeString(arg);
}
void ASTDump::visitImpl(ArrayType* arg) { strm << Type::getTypeString(arg); }
void ASTDump::visitImpl(TupleType* arg) { strm << Type::getTypeString(arg); }
void ASTDump::visitImpl(CallableType* arg) { strm << Type::getTypeString(arg); }
void ASTDump::visitImpl(ClassType* arg) { strm << Type::getTypeString(arg); }
void ASTDump::visitImpl(IfStatement* arg) {

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
void ASTDump::visitImpl(WhileStatement* arg) {
  strm << "while ";
  arg->expr()->accept(this);
  strm << " {" << strm.nl();
  strm.increaseIndent();
  arg->body()->accept(this);
  strm.decreaseIndent();
  strm << strm.nl() << "}";
}
void ASTDump::visitImpl(ForStatement* arg) {
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
void ASTDump::visitImpl(ReturnStatement* arg) {
  strm << "return ";
  arg->expression()->accept(this);
}
void ASTDump::visitImpl(Closure* arg) { strm << "unimp closure"; }
void ASTDump::visitImpl(Operator* arg) {
  strm << ast::getOperatorTypeString(arg->opType());
}
void ASTDump::visitImpl(CallExpression* arg) {
  strm << "(";
  arg->op()->accept(this);
  for(auto e : *arg->operands()) {
    strm << ", ";
    e->accept(this);
  }
  strm << ")";
}
void ASTDump::visitImpl(UseExpression* arg) { strm << arg->symbol()->name(); }
void ASTDump::visitImpl(IntExpression* arg) { strm << arg->value(); }
void ASTDump::visitImpl(UIntExpression* arg) { strm << arg->value(); }
void ASTDump::visitImpl(RealExpression* arg) { strm << arg->value(); }
void ASTDump::visitImpl(StringExpression* arg) {
  strm << "\"" << arg->value() << "\"";
}
void ASTDump::visitImpl(Nil* arg) {
  if(arg->isUserSpecified()) strm << "nil";
}

} // namespace visitor
} // namespace ast
