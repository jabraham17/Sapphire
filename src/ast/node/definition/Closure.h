// #ifndef SAPPHIRE_AST_NODE_DEFINITION_CLOSURE_H_
// #define SAPPHIRE_AST_NODE_DEFINITION_CLOSURE_H_
// #include "ast/ast.h"
// #include "ast/node/expression/Expression.h"

// namespace ast {
// namespace node {
// class Closure : public Expression {
//   std::size_t typeIdx_;
//   std::size_t parametersStartIdx_;
//   std::size_t parametersStopIdx_;
//   std::size_t bodyIdx_;

// public:
//   Closure(long line, CallableType* type, const ASTList& parameters, Scope*
//   body); Closure(CallableType* type, const ASTList& parameters, Scope* body);
//   virtual ~Closure() = default;
//   virtual void accept(visitor::ASTVisitor* ast) override;
//   // virtual ASTNode* clone() override;

//   virtual Type* type() override;
//   virtual void setType(Type* type) override;

//   static CallableType* determineClosureType(
//       Type* type_specifier,
//       const ASTList& parameters,
//       Type* returnType);

//   ASTListIteratorPair<Parameter> parameters();
//   Scope* body();
// };
// } // namespace node
// } // namespace ast
// #endif
