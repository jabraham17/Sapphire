
%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  // stuff from flex that bison needs to know about:
  extern int yylex(void);
  extern FILE *yyin;
  extern int line_num;

  #include "ast/ast.h"
  #include "parser/parser.h"
  using namespace ast;
  using namespace ast::node;
  using namespace ast::symbol;

  void yyerror(parser::Context* context, const char *s);
%}

%code requires {
  #include "parser/parser.h"
  #include "ast/node/nodes.h"
  #include "ast/symbol/symbol.h"
  #include "ast/symbol/function-symbol.h"

  struct FunctionPrototypeWrapper {
    const char* name;
    ast::node::ASTList parameters;
    ast::node::Type* returnType;
    ast::node::Type* belongsTo;
    bool isFFI;
    FunctionPrototypeWrapper(
      const char* name,
      ast::node::ASTList parameters,
      ast::node::Type* returnType,
      ast::node::Type* belongsTo = ast::node::Type::getUntypedType(),
      bool isFFI = false):
        name(name),
        parameters(parameters),
        returnType(returnType),
        belongsTo(belongsTo),
        isFFI(isFFI) {}
  };

  struct ASTListWrapper {
    ast::node::ASTList* list_;
    ASTListWrapper(): list_(new ast::node::ASTList()) {}
    void addFront(ast::node::ASTList::value_type elm) {
      list_->insert(list_->begin(), elm);
    }
    auto list() {
      return *list_;
    }
    auto ptr() {
      return list_;
    }
  };

}

%union {
  char* lexeme;

  ast::node::ASTNode* node;
  ast::node::Expression* expr;
  FunctionPrototypeWrapper* funcPrototypeWrapper;
  ast::node::FunctionPrototype* funcPrototype;
  ast::node::Type* type;
  ast::node::Statement* stmt;
  ast::node::OperatorType op;
  ast::node::Parameter* param;
  ast::node::Scope* scope;
  ast::symbol::Symbol* symbol;

  ASTListWrapper* nodeList;
}

%token FUNC VAR CLASS EXTERN FFI IF THEN ELSE FOR WHILE IN RETURN NIL REF OPERATOR NEW
%token INIT DEINIT THIS
%token ARROW COLON COMMA SEMICOLON
%token BOOL BYTE INT UINT REAL STRING ANY
%token LPAREN RPAREN LCURLY RCURLY LSQUARE RSQUARE
%token <lexeme> ID INTEGER_LITERAL HEX_LITERAL BINARY_LITERAL REAL_LITERAL STRING_LITERAL


/* higher precedence goes last */
%right EQUALS TERNARY
%left OR
%left AND
%left BW_OR
%left AMPERSAND
%left EQ NEQ
%left LT LTEQ GT GTEQ
%left LSHIFT RSHIFT
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%right UNARY_MINUS BW_NEGATE BANG
%right QUESTION TAKE_REF
%precedence DOT LSQUARE LPAREN

%token ERROR


%type <nodeList> class_statement_list definition_list statement_list expression_list optional_expression_list parameter_list parameter_list_
%type <node> file class_statement definition function_definition extern_definition operator_definition class_definition init_definition
%type <funcPrototype> function_prototype
%type <funcPrototypeWrapper> function_prototype_base function_prototype_typebase function_prototype_ffi
%type <scope> curly_statement_list
%type <stmt> statement control_flow_statement statement_ if_statement while_statement for_statement return_statement
%type <expr> expression number if_condition def_expression def_expression_with_value
  /* closure_definition */
%type <type> type_specifier type type_ref_ type_nilable_ type_base return_type array_type class_type
  /* tuple_type callable_type callable_return_type */
%type <param> parameter
%type <op> overloadable_op
%type <lexeme> name
%type <symbol> symbol


%glr-parser
%define parse.error verbose

%parse-param { parser::Context* context }


%start file
%%

file:
  definition_list {
    for(auto def: $1->list()) {
      context->ast->addChild(def);
    }
  }
  ;
definition_list:
  definition {
    $$ = new ASTListWrapper();
    $$->addFront($1);
  }
  | definition definition_list {
    $$ = $2;
    $$->addFront($1);
  }
  ;
definition:
  function_definition   { $$ = $1; }
  | extern_definition   { $$ = $1; }
  | class_definition    { $$ = $1; }
  | operator_definition { $$ = $1; }
  ;


function_definition:
  FUNC function_prototype curly_statement_list {
    $$ = new FunctionDefinition(line_num, $2, $3);
  }
  ;

function_prototype:
  function_prototype_ffi {
    bool mangled = !$1->isFFI;
    $$ = new FunctionPrototype(line_num, $1->name, $1->parameters, $1->returnType, mangled, $1->belongsTo);
    delete $1;
  }
  ;
function_prototype_ffi:
  FFI function_prototype_typebase %dprec 1 {
    $$ = $2;
    $$->isFFI = true;
  }
  | function_prototype_typebase %dprec 2 { $$ = $1; }
function_prototype_typebase:
  type_base DOT function_prototype_base %dprec 1 {
    $$ = $3;
    $$->belongsTo = $1;
  }
  | function_prototype_base %dprec 2 { $$ = $1; }
  ;
function_prototype_base:
  name parameter_list return_type {
    $$ = new FunctionPrototypeWrapper($1, $2->list(), $3);
  }
  ;
parameter_list:
  LPAREN parameter_list_ RPAREN { $$ = $2; }
  | LPAREN RPAREN               { $$ = new ASTListWrapper(); }
  ;
parameter_list_:
  parameter {
    $$ = new ASTListWrapper();
    $$->addFront($1);
  }
  | parameter COMMA parameter_list_ {
    $$ = $3;
    $$->addFront($1);
  }
  ;

parameter:
  symbol type_specifier { $$ = new Parameter(line_num, $1, $2); }
  ;
return_type:
  type_specifier { $$ = $1; }
  | COLON NIL    {
    $$ = Type::getNilType();
    $$->setLine(line_num);
  }
  ;
symbol:
  name { $$ = new Symbol($1); }
  ;
type_specifier:
  %empty       {
    $$ = Type::getUnknownType();
    $$->setLine(line_num);
  }
  | COLON type { $$ = $2; }
  ;
  /* type_list:
    type {
      $$ = new TypeList();
      if($1) $$->addFront($1);
    }
    | type COMMA type_list {
      $$ = $3;
      if($1) $$->addFront($1);
    }
    ; */
type:
  type_ref_ {
    $$ = $1;
    $$->setUserSpecified();
  }
  | error {
    context->addError("  malformed type");
    $$ = nullptr;
  }
  ;
type_ref_:
  type_nilable_ { $$ = $1; }
  | REF type_nilable_ {
    $$ = $2;
    $$->setRef(true);
  }
  ;
type_nilable_:
  type_base { $$ = $1; }
  | type_base QUESTION {
    $$ = $1;
    $$->setNilable(true);
  }
type_base:
  INT             { $$ = new PrimitiveType(line_num, PrimitiveTypeEnum::INT); }
  | UINT          { $$ = new PrimitiveType(line_num, PrimitiveTypeEnum::UINT); }
  | REAL          { $$ = new PrimitiveType(line_num, PrimitiveTypeEnum::REAL); }
  | STRING        { $$ = new PrimitiveType(line_num, PrimitiveTypeEnum::STRING); }
  | BOOL          { $$ = new PrimitiveType(line_num, PrimitiveTypeEnum::BOOL); }
  | BYTE          { $$ = new PrimitiveType(line_num, PrimitiveTypeEnum::BYTE); }
  | ANY           { $$ = new PrimitiveType(line_num, PrimitiveTypeEnum::ANY); }
  | array_type    { $$ = $1; }
  /* | tuple_type    { $$ = $1; } */
  /* | callable_type { $$ = $1; } */
  | class_type    { $$ = $1; }
  ;
array_type:
  LSQUARE type RSQUARE    { $$ = new ArrayType(line_num, $2); }
  ;
  /* tuple_type:
    LPAREN type_list RPAREN { $$ = new TupleType(line_num, $2); }
    ; */
  /* callable_type:
    LPAREN LPAREN type_list RPAREN ARROW callable_return_type RPAREN %dprec 1 {
      $$ = new CallableType(line_num, $3, $6);
    }
    | LPAREN LPAREN RPAREN ARROW callable_return_type RPAREN %dprec 2 {
      $$ = new CallableType(line_num, new TypeList(), $5);
    }
    ; */
  /* callable_return_type:
    type  { $$ = $1; }
    | NIL {
      $$ = Type::getNilType();
      $$->setLine(line_num);
    }
    ; */
class_type:
  name { $$ = new ClassType(line_num, $1); }
  ;


statement_list:
  statement {
    $$ = new ASTListWrapper();
    if($1) $$->addFront($1);
  }
  | statement statement_list {
    $$ = $2;
    if($1) $$->addFront($1);
  }
  ;

statement:
  statement_  SEMICOLON    { $$ = $1; }
  | control_flow_statement { $$ = $1; }
  /* | closure_definition        { $$ = $1->toStatement(); } */
  ;

control_flow_statement:
  if_statement   { $$ = $1; }
  | while_statement { $$ = $1; }
  | for_statement   { $$ = $1; }
  ;
statement_:
  def_expression              { $$ = new ExpressionStatement(line_num, $1); }
  | def_expression_with_value { $$ = new ExpressionStatement(line_num, $1); }
  | return_statement          { $$ = $1; }
  | expression                { $$ = new ExpressionStatement(line_num, $1); }
  ;
curly_statement_list:
  LCURLY statement_list RCURLY { $$ = new Scope(line_num, $2->list()); }
  | LCURLY RCURLY              { $$ = new Scope(line_num); }
  ;


if_statement:
  IF if_condition curly_statement_list {
    $$ = new IfStatement(line_num, $2, $3);
  }
  | IF if_condition curly_statement_list ELSE if_statement {
    $$ = new IfStatement(line_num, $2, $3, $5);
  }
  | IF if_condition curly_statement_list ELSE curly_statement_list {
    $$ = new IfStatement(line_num, $2, $3, $5);
  }
  ;
if_condition:
  expression                  { $$ = $1; }
  | def_expression_with_value { $$ = $1; }
  ;


while_statement:
  WHILE expression curly_statement_list {
    $$ = new WhileStatement(line_num, $2, $3);
  }
  ;
for_statement:
  FOR def_expression IN expression curly_statement_list {
    $$ = new ForStatement(line_num, $2->toDefExpression(), $4, $5);
  }
  ;
return_statement:
  RETURN expression { $$ = new ReturnStatement(line_num, $2); }
  ;


def_expression:
  VAR symbol type_specifier {
    $$ = new DefExpression($3, $2);
  }
  | error {
    context->addError("  malformed def");
    $$ = nullptr;
  }
  ;
def_expression_with_value:
  def_expression EQUALS expression {
    $$ = $1;
    auto v = $$->toDefExpression();
    if(v) v->setInitialValue($3);
    else context->addError("  invalid def expression");
  }
  ;

  /* closure_definition:
    FUNC symbol type_specifier EQUALS parameter_list return_type curly_statement_list {
      auto closure_type = Closure::determineClosureType($3, *$5, $6);
      if(closure_type != nullptr) {
          auto closure = new Closure(line_num, closure_type, $5, $7);
          $$ = new DefExpression(line_num, $2, closure);
    }
      else {
          yyerror(context, "invalid closure type");
          $$ = nullptr;
    }
    }
    ; */

expression:
  symbol                     { $$ = new UseExpression(line_num, $1); }
  | THIS                     { $$ = new UseExpression(line_num, new Symbol("this")); }
  | number                   { $$ = $1; }
  | STRING_LITERAL           { $$ = new StringExpression(line_num, $1); }
  | NIL                      { $$ = new Nil(line_num, true); }
  | LPAREN expression RPAREN { $$ = $2; }
  | expression EQUALS expression {
    $$ = new CallExpression(line_num, OperatorType::ASSIGNMENT, $1, $3);
  }
  | expression DOT symbol {
    $$ = new CallExpression(
        line_num,
        OperatorType::FIELD_ACCESS,
        $1,
        new UseExpression(line_num, $3));
  }
  | BW_NEGATE expression {
    $$ = new CallExpression(line_num, OperatorType::BW_NEGATE, $2);
  }
  | BANG expression {
    $$ = new CallExpression(line_num, OperatorType::NOT, $2);
  }
  | MINUS expression %prec UNARY_MINUS {
    $$ = new CallExpression(line_num, OperatorType::NEGATE, $2);
  }
  | AMPERSAND expression %prec TAKE_REF {
    $$ = new CallExpression(line_num, OperatorType::TAKE_REF, $2);
  }
  | expression BANG {
    $$ = new CallExpression(line_num, OperatorType::FORCE_UNWRAP, $1);
  }
  | expression QUESTION {
    $$ = new CallExpression(line_num, OperatorType::OPTIONAL_UNWRAP, $1);
  }
  | IF expression THEN expression ELSE expression %prec TERNARY {
    $$ = new CallExpression(line_num, OperatorType::CONDITIONAL, $2, $4, $6);
  }
  | expression PLUS expression {
    $$ = new CallExpression(line_num, OperatorType::PLUS, $1, $3);
  }
  | expression MINUS expression {
    $$ = new CallExpression(line_num, OperatorType::MINUS, $1, $3);
  }
  | expression MULTIPLY expression {
    $$ = new CallExpression(line_num, OperatorType::MULTIPLY, $1, $3);
  }
  | expression DIVIDE expression {
    $$ = new CallExpression(line_num, OperatorType::DIVIDE, $1, $3);
  }
  | expression LSHIFT expression {
    $$ = new CallExpression(line_num, OperatorType::LSHIFT, $1, $3);
  }
  | expression RSHIFT expression {
    $$ = new CallExpression(line_num, OperatorType::RSHIFT, $1, $3);
  }
  | expression MODULO expression {
    $$ = new CallExpression(line_num, OperatorType::MODULO, $1, $3);
  }
  | expression AND expression {
    $$ = new CallExpression(line_num, OperatorType::AND, $1, $3);
  }
  | expression OR expression {
    $$ = new CallExpression(line_num, OperatorType::OR, $1, $3);
  }
  | expression AMPERSAND expression {
    $$ = new CallExpression(line_num, OperatorType::BW_AND, $1, $3);
  }
  | expression BW_OR expression {
    $$ = new CallExpression(line_num, OperatorType::BW_OR, $1, $3);
  }
  | expression EQ expression {
    $$ = new CallExpression(line_num, OperatorType::EQ, $1, $3);
  }
  | expression NEQ expression {
    $$ = new CallExpression(line_num, OperatorType::NEQ, $1, $3);
  }
  | expression LT expression {
    $$ = new CallExpression(line_num, OperatorType::LT, $1, $3);
  }
  | expression GT expression {
    $$ = new CallExpression(line_num, OperatorType::GT, $1, $3);
  }
  | expression LTEQ expression {
    $$ = new CallExpression(line_num, OperatorType::LTEQ, $1, $3);
  }
  | expression GTEQ expression {
    $$ = new CallExpression(line_num, OperatorType::GTEQ, $1, $3);
  }
  | expression LPAREN optional_expression_list RPAREN {
    auto ce = new CallExpression(line_num, OperatorType::FUNCTION, $1);
    ce->addOperands($3->list());
    $$ = ce;
  }
  | expression LSQUARE expression RSQUARE {
    $$ = new CallExpression(line_num, OperatorType::SUBSCRIPT, $1, $3);
  }
  | NEW class_type LPAREN optional_expression_list RPAREN {
    auto ce = new CallExpression(line_num, OperatorType::NEW_CLASS, $2);
    ce->addOperands($4->list());
    $$ = ce;
  }
  | NEW LSQUARE type_specifier COMMA expression RSQUARE {
    $$ = new CallExpression(line_num, OperatorType::NEW_ARRAY, $3, $5);
  }
  | NEW LCURLY expression_list RCURLY {
    auto ce = new CallExpression(line_num, OperatorType::NEW_ARRAY);
    ce->addOperands($3->list());
    $$ = ce;
  }
  /* | NEW LPAREN expression_list RPAREN {
    $$ = new CallExpression(line_num, OperatorType::NEW_TUPLE, $3);
  } */
  ;
number:
  INTEGER_LITERAL {
    auto v = std::strtoll($1, nullptr, 10);
    $$ = new IntExpression(line_num, v);
  }
  | HEX_LITERAL {
    // skip '0x'
    auto v = std::strtoull($1 + 2, nullptr, 10);
    $$ = new UIntExpression(line_num, v);
  }
  | BINARY_LITERAL {
    // skip '0b'
    auto v = std::strtoull($1 + 2, nullptr, 10);
    $$ = new UIntExpression(line_num, v);
  }
  | REAL_LITERAL {
    auto v = std::strtod($1, nullptr);
    $$ = new RealExpression(line_num, v);
  }
  ;

optional_expression_list:
  expression_list { $$ = $1; }
  | %empty { $$ = new ASTListWrapper(); }
  ;
expression_list:
  expression {
    $$ = new ASTListWrapper();
    $$->addFront($1);
  }
  | expression COMMA expression_list {
    $$ = $3;
    $$->addFront($1);
  }
  ;

extern_definition:
  EXTERN function_prototype SEMICOLON {
    $$ = new ExternDefinition(line_num, $2);
  }
  ;


class_definition:
  CLASS class_type LCURLY class_statement_list RCURLY {
    $$ = ClassDefinition::buildClass($2->toClassType(), $4->list());
    $$->setLine(line_num);
  }
  ;
class_statement_list:
  class_statement {
    $$ = new ASTListWrapper();
    $$->addFront($1);
  }
  | class_statement class_statement_list {
    $$ = $2;
    $$->addFront($1);
  }
  ;
class_statement:
  def_expression SEMICOLON              { $$ = $1; }
  | def_expression_with_value SEMICOLON { $$ = $1; }
  | function_definition                 { $$ = $1; }
  | init_definition                     { $$ = $1; }
  ;
init_definition:
  INIT parameter_list curly_statement_list {
    $$ = new InitDefinition(line_num, $2->list(), $3);
  }
  | DEINIT curly_statement_list {
    $$ = new InitDefinition(line_num, $2);
  }
  | error {
    context->addError("  malformed init");
    $$ = nullptr;
  }
  ;


operator_definition:
  OPERATOR overloadable_op parameter_list curly_statement_list {
    $$ = new OperatorDefinition(line_num, $2, $3->list(), $4);
  }
  ;

overloadable_op:
  PLUS        { $$ = OperatorType::PLUS; }
  | MINUS     { $$ = OperatorType::MINUS; }
  | MULTIPLY  { $$ = OperatorType::MULTIPLY; }
  | DIVIDE    { $$ = OperatorType::DIVIDE; }
  | LSHIFT    { $$ = OperatorType::LSHIFT; }
  | RSHIFT    { $$ = OperatorType::RSHIFT; }
  | MODULO    { $$ = OperatorType::MODULO; }
  | AND       { $$ = OperatorType::AND; }
  | OR        { $$ = OperatorType::OR; }
  | AMPERSAND { $$ = OperatorType::BW_AND; }
  | BW_OR     { $$ = OperatorType::BW_OR; }
  | EQ        { $$ = OperatorType::EQ; }
  | NEQ       { $$ = OperatorType::NEQ; }
  | LT        { $$ = OperatorType::LT; }
  | GT        { $$ = OperatorType::GT; }
  | LTEQ      { $$ = OperatorType::LTEQ; }
  | GTEQ      { $$ = OperatorType::GTEQ; }
  | BANG      { $$ = OperatorType::NEGATE; }
  | BW_NEGATE { $$ = OperatorType::BW_NEGATE; }
  ;


name:
  ID { $$ = $1; }
  ;

%%

void yyerror(parser::Context* context, const char *s) {
  context->addError(std::string(s) + " on line " + std::to_string(line_num));
}

ASTNode* parser::parse(FILE* fp) {
  #if defined(DEBUG_PARSER) && DEBUG_PARSER==1
  yydebug = 1;
  #endif

  yyin = fp;
  Context context;
  yyparse(&context);
  if(context.hasErrors()) {
    std::cerr << "Errors occured while parsing" << std::endl;
    for(auto e: context.errors()) {
      std::cerr << "  " << e << std::endl;
    }
    return nullptr;
  }
  return context.ast;
}
