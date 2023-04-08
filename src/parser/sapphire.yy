
%{
    #include <stdio.h>
    #include <stdlib.h>
    // stuff from flex that bison needs to know about:
    extern int yylex(void);
    extern FILE *yyin;
    extern int line_num;

    #include "ast/ast.h"
    using namespace ast;

    void yyerror([[maybe_unused]] ASTNode*& ast, const char *s);
%}

%code requires {
    #include "parser/parser.h"
}

%union {
  char* lexeme;

  ast::ASTNode* node;
  ast::Expression* expr;
  ast::Definition* def;
  ast::FunctionPrototype* funcPrototype;
  ast::Type* type;
  ast::Statement* stmt;
  ast::Operator* op;
  ast::Parameter* param;
  ast::Scope* scope;
  ast::Symbol* symbol;

  ast::NodeList<ast::ASTNode>* nodeList;
  ast::NodeList<ast::Expression>* exprList;
  ast::NodeList<ast::Definition>* defList;
  ast::NodeList<ast::Statement>* stmtList;
  ast::NodeList<ast::Type>* typeList;
  ast::NodeList<ast::Parameter>* paramList;
}

%token FUNC VAR CLASS EXTERN IF THEN ELSE FOR WHILE IN NIL REF OPERATOR NEW
%token INIT DEINIT
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



%type <nodeList> class_statement_list
%type <node> file class_statement
%type <defList> definition_list
%type <def> definition function_definition extern_definition operator_definition class_definition init_definition
%type <funcPrototype> function_prototype function_prototype_
%type <scope> curly_statement_list
%type <stmtList> statement_list 
%type <stmt> statement statement_semi if_statement while_statement for_statement
%type <exprList> expression_list optional_expression_list
%type <expr> expression number if_condition closure_definition def_expression def_expression_with_value
%type <typeList> type_list
%type <type> type_specifier type type_ref_ type_nilable_ type_base_ return_type array_type tuple_type callable_type callable_return_type class_type
%type <paramList> parameter_list parameter_list_
%type <param> parameter
%type <op> overloadable_op
%type <lexeme> name
%type <symbol> symbol



%glr-parser

%parse-param { ast::ASTNode*& ast }


%start file
%%

file:
    definition_list { ast = $1; }
    ;
definition_list:
    definition { 
        $$ = new NodeList<Definition>();
        $$->addElement($1);
    }
    | definition definition_list {
        $$ = $2;
        $$->addElement($1);
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
    | LPAREN RPAREN               { $$ = new NodeList<Parameter>(); }
    ;
parameter_list_:
    parameter {
        $$ = new NodeList<Parameter>();
        $$->addElement($1);
    }
    | parameter COMMA parameter_list_ {
        $$ = $3;
        $$->addElement($1);
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
    %empty       { $$ = Type::getNilType(); }
    | COLON type { $$ = $2; }
    ;
type_list:
    type {
        $$ = new NodeList<Type>();
        $$->addElement($1);
    }
    | type COMMA type_list {
        $$ = $3;
        $$->addElement($1);
    }
    ;
type:
    type_ref_ { $$ = $1; }
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
    INT      { $$ = new PrimitiveType(PrimitiveTypeEnum::INT); }
    | UINT   { $$ = new PrimitiveType(PrimitiveTypeEnum::UINT); }
    | REAL   { $$ = new PrimitiveType(PrimitiveTypeEnum::REAL); }
    | STRING { $$ = new PrimitiveType(PrimitiveTypeEnum::STRING); }
    | BOOL   { $$ = new PrimitiveType(PrimitiveTypeEnum::BOOL); }
    | BYTE   { $$ = new PrimitiveType(PrimitiveTypeEnum::BYTE); }
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
    LPAREN LPAREN type_list RPAREN ARROW callable_return_type RPAREN {
        $$ = new CallableType($3, $6);
    }
    | LPAREN LPAREN RPAREN ARROW callable_return_type RPAREN { 
        $$ = new CallableType(new NodeList<Type>(), $5);
    }
    ;
callable_return_type:
    type  { $$ = $1; }
    | NIL { $$ = Type::getNilType(); }
    ;
class_type:
    symbol { $$ = new ClassType($1); }
    ;


statement_list:
    statement_semi {
        $$ = new NodeList<Statement>();
        $$->addElement($1);
    }
    | statement_semi statement_list {
        $$ = $2;
        $$->addElement($1);
    }
    ;

statement_semi:
    statement SEMICOLON { $$ = $1; }
    ;
statement:
    def_expression                   { $$ = toStatementNode($1); }
    | def_expression_with_value      { $$ = toStatementNode($1); }
    | closure_definition             { $$ = toStatementNode($1); }
    | if_statement                   { $$ = $1; }
    | while_statement                { $$ = $1; }
    | for_statement                  { $$ = $1; }
    | expression                     { $$ = toStatementNode($1); }
    ;
curly_statement_list:
    LCURLY statement_list RCURLY { $$ = new Scope($2); }
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


def_expression:
    VAR symbol type_specifier { 
        $$ = new DefExpression($3, $2);
    }
    ;
def_expression_with_value:
    def_expression EQUALS expression {
        $$ = $1;
        toDefExpressionNode($$)->setValue($3);
    }
    ;

closure_definition:
    FUNC symbol type_specifier EQUALS parameter_list return_type curly_statement_list {
        auto closure_type = Closure::determineClosureType($3, *$5, $6);
        auto closure = new Closure(closure_type, $5, $7);
        $$ = new DefExpression($2, closure);
    }
    ;
        
expression:
    symbol                     { $$ = new UseExpression($1); }
    | number                   { $$ = $1; }
    | STRING_LITERAL           { $$ = new StringExpression($1); }
    | NIL                      { $$ = new Nil(); }
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
    | NEW symbol LPAREN optional_expression_list RPAREN {
        $$ = new CallExpression(
            OperatorType::NEW_CLASS,
            new UseExpression($2),
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
        $$ = new NumberExpression<long long>(v);
    }
    | HEX_LITERAL {
        // skip '0x'
        auto v = std::strtoull($1 + 2, nullptr, 10);
        $$ = new NumberExpression<unsigned long long>(v);
    }
    | BINARY_LITERAL {
        // skip '0b'
        auto v = std::strtoull($1 + 2, nullptr, 10);
        $$ = new NumberExpression<unsigned long long>(v);
    }
    | REAL_LITERAL {
        auto v = std::strtod($1, nullptr);
        $$ = new NumberExpression<double>(v);
    }
    ;
    
optional_expression_list:
    expression_list { $$ = $1; }
    | %empty { $$ = new NodeList<Expression>(); }
    ;
expression_list:
    expression {
        $$ = new NodeList<Expression>();
        $$->addElement($1);
    }
    | expression COMMA expression_list {
        $$ = $3;
        $$->addElement($1);
    }
    ;

extern_definition:
    EXTERN function_prototype SEMICOLON { $$ = new ExternDefinition($2); }
    ;


class_definition:
    CLASS symbol LCURLY class_statement_list RCURLY {
        $$ = ClassDefinition::buildClass($2, $4);
    }
    ;
class_statement_list:
    class_statement {
        $$ = new NodeList<ASTNode>();
        $$->addElement($1);
    }
    | class_statement class_statement_list {
        $$ = $2;
        $$->addElement($1);
    }
    ;
class_statement:
    def_expression              { $$ = $1; }
    | def_expression_with_value { $$ = $1; }
    | function_definition       { $$ = $1; }
    | init_definition           { $$ = $1; }
    ;
init_definition:
    INIT parameter_list curly_statement_list {
        $$ = new InitDefinition($2, $3);
    }
    | DEINIT curly_statement_list {
        $$ = new InitDefinition($2);
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

void yyerror([[maybe_unused]] ASTNode*& ast, const char *s) {
    fprintf(stderr, "Parse error!  Message: %s on %d\n", s, line_num);
    // might as well halt now:
    exit(-1);
}

ASTNode* parse(FILE* fp) {
    yyin = fp;
    ASTNode* root;
    yyparse(root);
    return root;
}

