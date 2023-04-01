# Grammar

```
file -> definition_list
definition_list -> definition | definition definition_list
definition -> function_definition | extern_definition | class_definition | operator_definition

function_definition -> FUNC func_namespace func_name optional_parameter_list return_type bracket_statement_list
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
type_specifier -> COLON ref? type QUESTION?
type_list -> type | type COMMA type_list
type -> INT
type -> REAL
type -> STRING
type -> BOOL
type -> BYTE
type -> array_type
array_type -> LBRACK type RBRACK
type -> tuple_type
tuple_type -> LPAREN type_list RPAREN
type -> callable_type
callable_type -> optional_paren_type_list ARROW optional_paren_type_list
optional_paren_type_list -> LPAREN RPAREN
optional_paren_type_list -> LPAREN type_list RPAREN
optional_paren_type_list -> type_list

statement_list -> statement_semi | statement_semi statement_list
statement_semi -> statement SEMICOLON
statement -> variable_definition | variable_definition_with_value | if_statement | while_statement | for_statement | assign_statement
bracket_statement_list -> LBRACK statement_list RBRACK

if_statement -> IF if_condition bracket_statement_list elseif_chain else_statement
elseif_chain -> EPSILON | elseif_statement | elseif_statement elseif_chain
elseif_statement -> ELSE IF if_condition bracket_statement_list
else_statement -> EPSILON | ELSE bracket_statement_list
if_condition -> expression
# unwrap
if_condition -> variable_definition_with_value

while_statement -> WHILE expression bracket_statement_list
for_statement -> FOR variable_definition IN expression bracket_statement_list

assign_statement -> lvalue_expression EQUALS assign_rhs
variable_definition -> VAR var_name type_specifier
variable_definition_with_value -> variable_definition EQUALS assign_rhs
assign_rhs -> expression

closure_definition -> optional_parameter_list return_type bracket_statement_list

lvalue_expression -> expression
expression -> closure_definition
expression -> primary
expression -> LPAREN primary RPAREN
expression -> expression op expression
expression -> NEGATE expression
expression -> TAKE_REF lvalue_expression
expression -> lvalue_expression FORCE_UNWRAP
expression -> if expression then expression else expression
op -> PLUS | MINUS | MULT | DIV | LSHIFT | RSHIFT | MOD | AND | OR | NOT | BITWISE_AND | BITWISE_OR | BITWISE_NOT | EQUAL_TO | NOT_EQUAL_TO | LESS_THAN | GREATER_THAN | LESS_THAN_EQUAL_TO | GREATER_THAN_EQUAL_TO
primary -> function_call | number | field_access | NIL | init_statement | collection_access
number -> INTEGER_CONSTANT | HEX_CONSTANT | BINARY_CONSTANT | REAL_CONSTANT
function_call -> access_chain LPAREN argument_list RPAREN
optional_argument_list -> EPSILON | LPAREN argument_list RPAREN
argument_list -> EPSILON | expression | expression COMMA argument_list
# can be a function call if the field_access resolves to a function
field_access -> access_chain
access_chain -> access_chain_atom | access_chain_atom DOT access_chain
access_chain_atom -> var_name 
access_chain_atom -> var_name FORCE_UNWRAP
init_statement -> NEW class_name optional_argument_list
collection_access -> lvalue_expression LBRACK expression RBRACK

extern_definition -> EXTERN func_namespace func_name optional_parameter_list return_type SEMICOLON

class_definition -> CLASS class_name LBRACK class_statement_list RBRACK
class_name -> name
class_statement_list -> class_statement | class_statement class_statement_list
class_statement -> variable_definition | variable_definition_with_value | function_definition | init_definition
init_definition -> INIT optional_parameter_list bracket_statement_list
init_definition -> DEINIT bracket_statement_list

name -> ID

```
