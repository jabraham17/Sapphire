
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

    void yyerror(parser::Context* context, const char *s);
%}

%code requires {
    #include "parser/parser.h"
}

%union {
  char* lexeme;

  ast::ASTNode* node;
  ast::Expression* expr;
  ast::FunctionPrototype* funcPrototype;
  ast::Type* type;
  ast::Statement* stmt;
  ast::Operator* op;
  ast::Parameter* param;
  ast::Scope* scope;
  ast::Symbol* symbol;

  ast::NodeList* nodeList;
}

%token FUNC VAR CLASS EXTERN IF THEN ELSE FOR WHILE IN RETURN NIL REF OPERATOR NEW
%token INIT DEINIT THIS
%token ARROW COLON COMMA SEMICOLON
%token BOOL BYTE INT UINT REAL STRING
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


%type <nodeList> class_statement_list definition_list statement_list expression_list optional_expression_list type_list parameter_list parameter_list_
%type <node> file class_statement definition function_definition extern_definition operator_definition class_definition init_definition
%type <funcPrototype> function_prototype function_prototype_
%type <scope> curly_statement_list
%type <stmt> statement control_flow_statement statement_ if_statement while_statement for_statement return_statement
%type <expr> expression number if_condition closure_definition def_expression def_expression_with_value
%type <type> type_specifier type type_ref_ type_nilable_ type_base_ return_type array_type tuple_type callable_type callable_return_type class_type
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
    definition_list { context->ast = $1; }
    ;
definition_list:
    definition { 
        $$ = new NodeList();
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
        $2->setMangled(true);
        $$ = new FunctionDefinition($2, $3);
    }
    ;
function_prototype:
    name DOT function_prototype_ %dprec 1 {
        $$ = $3;
        $$->setNamespace($1);
    }
    | function_prototype_ %dprec 2 { $$ = $1; }
    ;
function_prototype_:
    name parameter_list return_type { $$ = new FunctionPrototype($1, $2, $3); }
    ;
parameter_list:
    LPAREN parameter_list_ RPAREN { $$ = $2; }
    | LPAREN RPAREN               { $$ = new NodeList(); }
    ;
parameter_list_:
    parameter {
        $$ = new NodeList();
        $$->addFront($1);
    }
    | parameter COMMA parameter_list_ {
        $$ = $3;
        $$->addFront($1);
    }
    ;

parameter:
    symbol type_specifier { $$ = new Parameter($1, $2); }
    ;
return_type:
    type_specifier { $$ = $1; }
    | COLON NIL    { $$ = Type::getNilType(); }
    ;
symbol:
    name { $$ = new Symbol($1); }
    ;
type_specifier:
    %empty       { $$ = Type::getUnknownType(); }
    | COLON type { $$ = $2; }
    ;
type_list:
    type {
        $$ = new NodeList();
        if($1) $$->addFront($1);
    }
    | type COMMA type_list {
        $$ = $3;
        if($1) $$->addFront($1);
    }
    ;
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
    type_base_ { $$ = $1; }
    | type_base_ QUESTION {
        $$ = $1;
        $$->setNilable(true);
    }
type_base_:
    INT             { $$ = new PrimitiveType(PrimitiveTypeEnum::INT); }
    | UINT          { $$ = new PrimitiveType(PrimitiveTypeEnum::UINT); }
    | REAL          { $$ = new PrimitiveType(PrimitiveTypeEnum::REAL); }
    | STRING        { $$ = new PrimitiveType(PrimitiveTypeEnum::STRING); }
    | BOOL          { $$ = new PrimitiveType(PrimitiveTypeEnum::BOOL); }
    | BYTE          { $$ = new PrimitiveType(PrimitiveTypeEnum::BYTE); }
    | array_type    { $$ = $1; }
    | tuple_type    { $$ = $1; }
    | callable_type { $$ = $1; }
    | class_type    { $$ = $1; }
    ;
array_type:
    LSQUARE type RSQUARE    { $$ = new ArrayType($2); }
    ;
tuple_type:
    LPAREN type_list RPAREN { $$ = new TupleType($2); }
    ;
callable_type:
    LPAREN LPAREN type_list RPAREN ARROW callable_return_type RPAREN %dprec 1 {
        $$ = new CallableType($3, $6);
    }
    | LPAREN LPAREN RPAREN ARROW callable_return_type RPAREN %dprec 2 { 
        $$ = new CallableType(new NodeList(), $5);
    }
    ;
callable_return_type:
    type  { $$ = $1; }
    | NIL { $$ = Type::getNilType(); }
    ;
class_type:
    name { $$ = new ClassType($1); }
    ;


statement_list:
    statement {
        $$ = new NodeList();
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
    | closure_definition        { $$ = toStatementNode($1); }
    ;

control_flow_statement: 
    if_statement   { $$ = $1; }
    | while_statement { $$ = $1; }
    | for_statement   { $$ = $1; }
    ;
statement_:
    def_expression              { $$ = new ExpressionStatement($1); }
    | def_expression_with_value { $$ = new ExpressionStatement($1); }
    | return_statement          { $$ = $1; }
    | expression                { $$ = new ExpressionStatement($1); }
    ;
curly_statement_list:
    LCURLY statement_list RCURLY { $$ = new Scope($2); }
    | LCURLY RCURLY              { $$ = new Scope(new NodeList()); }
    ;


if_statement:
    IF if_condition curly_statement_list { 
        $$ = new IfStatement($2, $3);
    }
    | IF if_condition curly_statement_list ELSE if_statement { 
        $$ = new IfStatement($2, $3, $5);
    }
    | IF if_condition curly_statement_list ELSE curly_statement_list { 
        $$ = new IfStatement($2, $3, $5);
    }
    ;
if_condition:
    expression                  { $$ = $1; }
    | def_expression_with_value { $$ = $1; }
    ;


while_statement:
    WHILE expression curly_statement_list {
        $$ = new WhileStatement($2, $3);
    }
    ;
for_statement:
    FOR def_expression IN expression curly_statement_list {
        $$ = new ForStatement(toDefExpressionNode($2), $4, $5);
    }
    ;
return_statement:
    RETURN expression { $$ = new ReturnStatement($2); }
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
        auto v = toDefExpressionNode($$);
        if(v) v->setValue($3);
        else context->addError("  invalid def expression");
    }
    ;

closure_definition:
    FUNC symbol type_specifier EQUALS parameter_list return_type curly_statement_list {
        auto closure_type = Closure::determineClosureType($3, *$5, $6);
        if(closure_type != nullptr) {
            auto closure = new Closure(closure_type, $5, $7);
            $$ = new DefExpression($2, closure);
        }
        else {
            yyerror(context, "invalid closure type");
            $$ = nullptr;
        }
    }
    ;
        
expression:
    symbol                     { $$ = new UseExpression($1); }
    | THIS                     { $$ = new UseExpression(new Symbol("this")); }
    | number                   { $$ = $1; }
    | STRING_LITERAL           { $$ = new StringExpression($1); }
    | NIL                      { $$ = new Nil(true); }
    | LPAREN expression RPAREN { $$ = $2; }
    | expression EQUALS expression { 
        $$ = new CallExpression(OperatorType::ASSIGNMENT, $1, $3);
    }
    | expression DOT symbol {
        $$ = new CallExpression(
            OperatorType::FIELD_ACCESS,
            $1,
            new UseExpression($3));
    }
    | BW_NEGATE expression {
        $$ = new CallExpression(OperatorType::BW_NEGATE, $2);
    }
    | BANG expression {
        $$ = new CallExpression(OperatorType::NOT, $2);
    }
    | MINUS expression %prec UNARY_MINUS {
        $$ = new CallExpression(OperatorType::NEGATE, $2);
    }
    | AMPERSAND expression %prec TAKE_REF {
        $$ = new CallExpression(OperatorType::TAKE_REF, $2);
    }
    | expression BANG {
        $$ = new CallExpression(OperatorType::FORCE_UNWRAP, $1);
    }
    | expression QUESTION {
        $$ = new CallExpression(OperatorType::OPTIONAL_UNWRAP, $1);
    }
    | IF expression THEN expression ELSE expression %prec TERNARY {
        $$ = new CallExpression(OperatorType::CONDITIONAL, $2, $4, $6);
    }
    | expression PLUS expression {
        $$ = new CallExpression(OperatorType::PLUS, $1, $3);
    }
    | expression MINUS expression {
        $$ = new CallExpression(OperatorType::MINUS, $1, $3);
    }
    | expression MULTIPLY expression {
        $$ = new CallExpression(OperatorType::MULTIPLY, $1, $3);
    }
    | expression DIVIDE expression {
        $$ = new CallExpression(OperatorType::DIVIDE, $1, $3);
    }
    | expression LSHIFT expression {
        $$ = new CallExpression(OperatorType::LSHIFT, $1, $3);
    }
    | expression RSHIFT expression {
        $$ = new CallExpression(OperatorType::RSHIFT, $1, $3);
    }
    | expression MODULO expression {
        $$ = new CallExpression(OperatorType::MODULO, $1, $3);
    }
    | expression AND expression {
        $$ = new CallExpression(OperatorType::AND, $1, $3);
    }
    | expression OR expression {
        $$ = new CallExpression(OperatorType::OR, $1, $3);
    }
    | expression AMPERSAND expression {
        $$ = new CallExpression(OperatorType::BW_AND, $1, $3);
    }
    | expression BW_OR expression {
        $$ = new CallExpression(OperatorType::BW_OR, $1, $3);
    }
    | expression EQ expression {
        $$ = new CallExpression(OperatorType::EQ, $1, $3);
    }
    | expression NEQ expression {
        $$ = new CallExpression(OperatorType::NEQ, $1, $3);
    }
    | expression LT expression {
        $$ = new CallExpression(OperatorType::LT, $1, $3);
    }
    | expression GT expression {
        $$ = new CallExpression(OperatorType::GT, $1, $3);
    }
    | expression LTEQ expression {
        $$ = new CallExpression(OperatorType::LTEQ, $1, $3);
    }
    | expression GTEQ expression {
        $$ = new CallExpression(OperatorType::GTEQ, $1, $3);
    }
    | expression LPAREN optional_expression_list RPAREN {
        $$ = new CallExpression(OperatorType::FUNCTION, $1, $3);
    }
    | expression LSQUARE expression RSQUARE {
        $$ = new CallExpression(OperatorType::SUBSCRIPT, $1, $3);
    }
    | NEW class_type LPAREN optional_expression_list RPAREN {
        $$ = new CallExpression(
            OperatorType::NEW_CLASS,
            $2,
            $4);
    }
    | NEW LSQUARE type_specifier COMMA expression RSQUARE {
        $$ = new CallExpression(OperatorType::NEW_ARRAY, $3, $5);
    }
    | NEW LCURLY expression_list RCURLY {
        $$ = new CallExpression(OperatorType::NEW_ARRAY, $3);
    }
    | NEW LPAREN expression_list RPAREN {
        $$ = new CallExpression(OperatorType::NEW_TUPLE, $3);
    }
    ;
number:
    INTEGER_LITERAL {
        auto v = std::strtoll($1, nullptr, 10);
        $$ = new IntExpression(v);
    }
    | HEX_LITERAL {
        // skip '0x'
        auto v = std::strtoull($1 + 2, nullptr, 10);
        $$ = new UIntExpression(v);
    }
    | BINARY_LITERAL {
        // skip '0b'
        auto v = std::strtoull($1 + 2, nullptr, 10);
        $$ = new UIntExpression(v);
    }
    | REAL_LITERAL {
        auto v = std::strtod($1, nullptr);
        $$ = new RealExpression(v);
    }
    ;
    
optional_expression_list:
    expression_list { $$ = $1; }
    | %empty { $$ = new NodeList(); }
    ;
expression_list:
    expression {
        $$ = new NodeList();
        $$->addFront($1);
    }
    | expression COMMA expression_list {
        $$ = $3;
        $$->addFront($1);
    }
    ;

extern_definition:
    EXTERN function_prototype SEMICOLON {
        $2->setMangled(false);
        $$ = new ExternDefinition($2);
    }
    ;


class_definition:
    CLASS class_type LCURLY class_statement_list RCURLY {
        $$ = ClassDefinition::buildClass(toClassTypeNode($2), $4);
    }
    ;
class_statement_list:
    class_statement {
        $$ = new NodeList();
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
        $$ = new InitDefinition($2, $3);
    }
    | DEINIT curly_statement_list {
        $$ = new InitDefinition($2);
    }
    | error {
        context->addError("  malformed init");
        $$ = nullptr;
    }
    ;


operator_definition:
    OPERATOR overloadable_op parameter_list curly_statement_list {
        $$ = new OperatorDefinition($2, $3, $4);
    }
    ;

overloadable_op:
    PLUS        { $$ = new Operator(OperatorType::PLUS); }
    | MINUS     { $$ = new Operator(OperatorType::MINUS); }
    | MULTIPLY  { $$ = new Operator(OperatorType::MULTIPLY); }
    | DIVIDE    { $$ = new Operator(OperatorType::DIVIDE); }
    | LSHIFT    { $$ = new Operator(OperatorType::LSHIFT); }
    | RSHIFT    { $$ = new Operator(OperatorType::RSHIFT); }
    | MODULO    { $$ = new Operator(OperatorType::MODULO); }
    | AND       { $$ = new Operator(OperatorType::AND); }
    | OR        { $$ = new Operator(OperatorType::OR); }
    | AMPERSAND { $$ = new Operator(OperatorType::BW_AND); }
    | BW_OR     { $$ = new Operator(OperatorType::BW_OR); }
    | EQ        { $$ = new Operator(OperatorType::EQ); }
    | NEQ       { $$ = new Operator(OperatorType::NEQ); }
    | LT        { $$ = new Operator(OperatorType::LT); }
    | GT        { $$ = new Operator(OperatorType::GT); }
    | LTEQ      { $$ = new Operator(OperatorType::LTEQ); }
    | GTEQ      { $$ = new Operator(OperatorType::GTEQ); }
    | BANG      { $$ = new Operator(OperatorType::NEGATE); }
    | BW_NEGATE { $$ = new Operator(OperatorType::BW_NEGATE); }
    ;


name:
    ID { $$ = $1; }
    ;

%%

void yyerror(parser::Context* context, const char *s) {
    context->addError(std::string(s) + " on line " + std::to_string(line_num));
}

namespace parser {
ASTNode* parse(FILE* fp) {
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

}
