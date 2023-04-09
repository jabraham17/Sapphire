#include "dump-ast.h"

namespace ast {
namespace visitor {

void DumpAST::visitNodeListImpl(NodeList* arg) {
  bool notFirstOne = false;
  for(auto a : *arg) {
    if(notFirstOne) strm << strm.nl();
    notFirstOne = true;
    a->accept(this);
  }
}
void DumpAST::visitParameterImpl(Parameter* arg) {
  strm << arg->symbol()->toString();
}
void DumpAST::visitFunctionPrototypeImpl(FunctionPrototype* arg) {
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
void DumpAST::visitFunctionDefinitionImpl(FunctionDefinition* arg) {
  arg->functionPrototype()->accept(this);
  strm << " {" << strm.nl();
  strm.increaseIndent();
  arg->body()->accept(this);
  strm.decreaseIndent();
  strm << strm.nl() << "}";
}
void DumpAST::visitExternDefinitionImpl(ExternDefinition* arg) {
  strm << "extern ";
  arg->functionPrototype()->accept(this);
}
void DumpAST::visitInitDefinitionImpl(InitDefinition* arg) {
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
void DumpAST::visitScopeImpl(Scope* arg) { arg->statements()->accept(this); }
void DumpAST::visitExpressionStatementImpl(ExpressionStatement* arg) {
  arg->expression()->accept(this);
}

void DumpAST::visitDefExpressionImpl(DefExpression* arg) {
  strm << arg->symbol()->toString();
  if(arg->hasInitialValue()) {
    strm << " = ";
    arg->assignValue()->accept(this);
  }
}
void DumpAST::visitClassDefinitionImpl(ClassDefinition* arg) {
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
void DumpAST::visitOperatorDefinitionImpl(OperatorDefinition* arg) {
  strm << "unimp op def";
}
void DumpAST::visitPrimitiveTypeImpl(PrimitiveType* arg) {
  strm << Type::getTypeString(arg);
}
void DumpAST::visitArrayTypeImpl(ArrayType* arg) {
  strm << Type::getTypeString(arg);
}
void DumpAST::visitTupleTypeImpl(TupleType* arg) {
  strm << Type::getTypeString(arg);
}
void DumpAST::visitCallableTypeImpl(CallableType* arg) {
  strm << Type::getTypeString(arg);
}
void DumpAST::visitClassTypeImpl(ClassType* arg) {
  strm << Type::getTypeString(arg);
}
void DumpAST::visitIfStatementImpl(IfStatement* arg) {

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
void DumpAST::visitWhileStatementImpl(WhileStatement* arg) {
  strm << "while ";
  arg->expr()->accept(this);
  strm << " {" << strm.nl();
  strm.increaseIndent();
  arg->body()->accept(this);
  strm.decreaseIndent();
  strm << strm.nl() << "}";
}
void DumpAST::visitForStatementImpl(ForStatement* arg) {
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
void DumpAST::visitReturnStatementImpl(ReturnStatement* arg) {
  strm << "return ";
  arg->expression()->accept(this);
}
void DumpAST::visitClosureImpl(Closure* arg) { strm << "unimp closure"; }
void DumpAST::visitOperatorImpl(Operator* arg) {
  strm << ast::getOperatorTypeString(arg->opType());
}
void DumpAST::visitCallExpressionImpl(CallExpression* arg) {
  strm << "(";
  arg->op()->accept(this);
  for(auto e : *arg->operands()) {
    strm << ", ";
    e->accept(this);
  }
  strm << ")";
}
void DumpAST::visitUseExpressionImpl(UseExpression* arg) {
  strm << arg->symbol()->name();
}
void DumpAST::visitIntExpressionImpl(IntExpression* arg) {
  strm << arg->value();
}
void DumpAST::visitUIntExpressionImpl(UIntExpression* arg) {
  strm << arg->value();
}
void DumpAST::visitRealExpressionImpl(RealExpression* arg) {
  strm << arg->value();
}
void DumpAST::visitStringExpressionImpl(StringExpression* arg) {
  strm << "\"" << arg->value() << "\"";
}
void DumpAST::visitNilImpl(Nil* arg) {
  if(arg->isUserSpecified()) strm << "nil";
}

} // namespace visitor
} // namespace ast
