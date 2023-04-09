#ifndef SAPPHIRE_AST_VISITORS_DUMP_AST_H_
#define SAPPHIRE_AST_VISITORS_DUMP_AST_H_

#include "ast/ast-visitor.h"
#include "ast/ast.h"

#include <iostream>

namespace ast {

namespace visitor {

class IndentedOStream {
private:
  std::ostream& os;
  const size_t indentLevel;
  size_t currentIndent;
  bool atLineStart;

public:
  void increaseIndent() { currentIndent += indentLevel; }
  void decreaseIndent() {
    if(currentIndent >= indentLevel) currentIndent -= indentLevel;
    else currentIndent = 0;
  }
  std::string getIndent() { return std::string(currentIndent, ' '); }

public:
  IndentedOStream(std::ostream& os, size_t indentLevel = 2)
      : os(os), indentLevel(indentLevel), currentIndent(0), atLineStart(true) {}
  ~IndentedOStream() = default;
  IndentedOStream(const IndentedOStream& other) = delete;
  IndentedOStream(IndentedOStream&& other) noexcept = default;
  IndentedOStream& operator=(const IndentedOStream& other) = delete;
  IndentedOStream& operator=(IndentedOStream&& other) noexcept = delete;

  template <typename T> IndentedOStream& operator<<(T&& x) {
    if(atLineStart) {
      atLineStart = false;
      os << getIndent();
    }
    os << std::forward<T>(x);
    return *this;
  }

  // special case for ostream inits
  using OStreamType = std::basic_ostream<char, std::char_traits<char>>;
  using OStreamManip = OStreamType& (*)(OStreamType&);
  IndentedOStream& operator<<(OStreamManip manip) {
    manip(os);
    return *this;
  }

  struct nl_;
  friend nl_;
  struct nl_ {
    IndentedOStream* ios;
    nl_(IndentedOStream* ios) : ios(ios) {}
    friend std::ostream& operator<<(std::ostream& o, nl_ x) {
      o << std::endl;
      x.ios->atLineStart = true;
      return o;
    }
  };
  nl_ nl() { return nl_(this); }
  //   std::ostream& nl(std::ostream& o) {
  //     o << std::endl << this->getIndent();
  //     return o;
  // }
};

class DumpAST : public ASTVisitor {
private:
  IndentedOStream strm;

public:
  DumpAST(std::ostream& rawStrm, size_t indentLevel = 2)
      : strm(rawStrm, indentLevel) {}
  virtual ~DumpAST() = default;

private:
protected:
  virtual void visitNodeListImpl(NodeList* arg) override;
  virtual void visitParameterImpl(Parameter* arg) override;
  virtual void visitFunctionPrototypeImpl(FunctionPrototype* arg) override;
  virtual void visitFunctionDefinitionImpl(FunctionDefinition* arg) override;
  virtual void visitExternDefinitionImpl(ExternDefinition* arg) override;
  virtual void visitInitDefinitionImpl(InitDefinition* arg) override;
  virtual void visitScopeImpl(Scope* arg) override;
  virtual void visitExpressionStatementImpl(ExpressionStatement* arg) override;
  virtual void visitDefExpressionImpl(DefExpression* arg) override;
  virtual void visitClassDefinitionImpl(ClassDefinition* arg) override;
  virtual void visitOperatorDefinitionImpl(OperatorDefinition* arg) override;
  virtual void visitPrimitiveTypeImpl(PrimitiveType* arg) override;
  virtual void visitArrayTypeImpl(ArrayType* arg) override;
  virtual void visitTupleTypeImpl(TupleType* arg) override;
  virtual void visitCallableTypeImpl(CallableType* arg) override;
  virtual void visitClassTypeImpl(ClassType* arg) override;
  virtual void visitIfStatementImpl(IfStatement* arg) override;
  virtual void visitWhileStatementImpl(WhileStatement* arg) override;
  virtual void visitForStatementImpl(ForStatement* arg) override;
  virtual void visitReturnStatementImpl(ReturnStatement* arg) override;
  virtual void visitClosureImpl(Closure* arg) override;
  virtual void visitOperatorImpl(Operator* arg) override;
  virtual void visitCallExpressionImpl(CallExpression* arg) override;
  virtual void visitUseExpressionImpl(UseExpression* arg) override;
  virtual void visitIntExpressionImpl(IntExpression* arg) override;
  virtual void visitUIntExpressionImpl(UIntExpression* arg) override;
  virtual void visitRealExpressionImpl(RealExpression* arg) override;
  virtual void visitStringExpressionImpl(StringExpression* arg) override;
  virtual void visitNilImpl(Nil* arg) override;
};
} // namespace visitor
} // namespace ast

#endif
