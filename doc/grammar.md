# Grammar

```
file -> definition_list
definition_list -> definition | definition definition_list
definition -> function_definition | extern_definition | class_definition | operator_definition

function_definition -> FUNC func_namespace func_name optional_parameter_list return_type curly_statement_list
func_namespace -> name DOT
func_namespace -> EPSILON
func_name -> name
optional_parameter_list -> parameter_list | EPSILON
parameter_list -> LPAREN parameter_list_ RPAREN
parameter_list_ -> parameter | parameter COMMA parameter_list_
parameter_list_ -> EPSILON
parameter -> var_name type_specifier
return_type -> type_specifier
return_type -> COLON NIL
var_name -> name
type_specifier -> EPSILON
type_specifier -> COLON REF? type QUESTION?
type_list -> type | type COMMA type_list
type -> INT
type -> REAL
type -> STRING
type -> BOOL
type -> BYTE
type -> array_type
array_type -> LSQUARE type RSQUARE
type -> tuple_type
tuple_type -> LPAREN type_list RPAREN
type -> callable_type
callable_type -> optional_paren_type_list ARROW optional_paren_type_list
optional_paren_type_list -> LPAREN RPAREN
optional_paren_type_list -> LPAREN type_list RPAREN
optional_paren_type_list -> type_list

statement_list -> statement_semi | statement_semi statement_list
statement_semi -> statement SEMICOLON
statement -> variable_definition | variable_definition_with_value | if_statement | while_statement | for_statement | expression_statement
curly_statement_list -> LCURLY statement_list RCURLY

if_statement -> IF if_condition curly_statement_list elseif_chain else_statement
elseif_chain -> EPSILON | elseif_statement | elseif_statement elseif_chain
elseif_statement -> ELSE IF if_condition curly_statement_list
else_statement -> EPSILON | ELSE curly_statement_list
if_condition -> expression
# unwrap
if_condition -> variable_definition_with_value

while_statement -> WHILE expression curly_statement_list
for_statement -> FOR variable_definition IN expression curly_statement_list

expression_statement -> expression
variable_definition -> VAR var_name type_specifier
variable_definition_with_value -> variable_definition EQUALS expression

closure_definition -> optional_parameter_list return_type curly_statement_list

expression -> closure_definition
expression -> primary
expression -> LPAREN expression RPAREN
expression -> expression op expression
expression -> NEGATE expression
expression -> NOT expression
expression -> BW_NOT expression
expression -> TAKE_REF expression
expression -> expression FORCE_UNWRAP
expression -> expression OPTIONAL_UNWRAP
expression -> if expression then expression else expression
op -> PLUS | MINUS | MULT | DIV | LSHIFT | RSHIFT | MOD | AND | OR | BW_AND | BW_OR | EQ | NEQ | LT | GT | LTEQ | GTEQ
primary -> function_call | number | field_access | NIL | init_statement | collection_access | array_init_statement | tuple_init_statement | STRING_LITERAL
number -> INTEGER_LITERAL | HEX_LITERAL | BINARY_LITERAL | REAL_LITERAL
function_call -> access_chain LPAREN argument_list RPAREN
optional_argument_list -> EPSILON | LPAREN argument_list RPAREN
argument_list -> EPSILON | expression | expression COMMA argument_list
# can be a function call if the field_access resolves to a function
field_access -> access_chain
access_chain -> access_chain_atom | access_chain_atom DOT access_chain
access_chain_atom -> expression 
init_statement -> NEW class_name optional_argument_list
collection_access -> expression LSQUARE expression RSQUARE
array_init_statement -> NEW LSQUARE type_specifier COMMA expression RSQUARE
array_init_statement -> NEW LCURLY expression_list RCURLY
tuple_init_statement -> NEW LPAREN expression_list RPAREN
expression_list -> expression | expression expression_list

extern_definition -> EXTERN func_namespace func_name optional_parameter_list return_type SEMICOLON

class_definition -> CLASS class_name LCURLY class_statement_list RCURLY
class_name -> name
class_statement_list -> class_statement | class_statement class_statement_list
class_statement -> variable_definition | variable_definition_with_value | function_definition | init_definition
init_definition -> INIT optional_parameter_list curly_statement_list
init_definition -> DEINIT curly_statement_list

operator_definition -> OPERATOR op parameter_list curly_statement_list
op -> infix_op | postfix_op | prefix_op

name -> ID

```
