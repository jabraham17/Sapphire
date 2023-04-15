#include "ast-visitor.h"

#define VISIT(name) void ast::visitor::VisitAll::visitImpl(node::name* arg)

VISIT(NodeList) {
  for(auto a : *arg)
    a->accept(this);
}
VISIT(FunctionPrototype) {
  arg->parameters()->accept(this);
  arg->type()->accept(this);
}
VISIT(FunctionDefinition) {
  arg->functionPrototype()->accept(this);
  arg->body()->accept(this);
}
VISIT(ExternDefinition) { arg->functionPrototype()->accept(this); }
VISIT(InitDefinition) {
  if(arg->isInit()) arg->parameters()->accept(this);

  arg->body()->accept(this);
}
VISIT(Scope) { arg->statements()->accept(this); }
VISIT(Expression) { arg->type()->accept(this); }
VISIT(ExpressionStatement) { arg->expression()->accept(this); }
VISIT(DefExpression) {
  if(arg->hasInitialValue()) arg->assignValue()->accept(this);
}
VISIT(ClassDefinition) {
  arg->classType()->accept(this);
  arg->variables()->accept(this);
  arg->functions()->accept(this);
  arg->initializers()->accept(this);
  arg->deinitializer()->accept(this);
}
VISIT(OperatorDefinition) {
  arg->parameters()->accept(this);
  arg->body()->accept(this);
}

VISIT(ArrayType) { arg->elementType()->accept(this); }
VISIT(TupleType) { arg->elementTypes()->accept(this); }
VISIT(CallableType) {
  arg->parameterTypes()->accept(this);
  arg->returnType()->accept(this);
}
VISIT(IfStatement) {
  arg->expr()->accept(this);
  arg->ifBody()->accept(this);
  if(arg->hasElseIf()) arg->elseIfBody()->accept(this);
  else if(arg->hasPlainElse()) arg->plainElseBody()->accept(this);
}
VISIT(WhileStatement) {
  arg->expr()->accept(this);
  arg->body()->accept(this);
}
VISIT(ForStatement) {
  arg->variable()->accept(this);
  arg->expr()->accept(this);
  arg->body()->accept(this);
}
VISIT(ReturnStatement) { arg->expression()->accept(this); }
VISIT(Closure) {
  arg->parameters()->accept(this);
  arg->body()->accept(this);
}
VISIT(CallExpression) { arg->operands()->accept(this); }
