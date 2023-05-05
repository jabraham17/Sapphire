
#ifndef ast_node_def
  #define ast_node_def(name)
#endif

/*items always get include
in 'ast_node_def_exclude' mode, user can specify the handful to exclude
in 'ast_node_def_include' mode, user can specify the handful to include
*/

// TODO: there is a bug in clang-format that causes 'ast_node_def' to get
// indented infinitely. Possibly a quick fix?
// clang-format off


#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_ASTNode)) ||                              \
    (defined(ast_node_def_include) && defined(ast_node_def_include_ASTNode))
ast_node_def(ASTNode)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_Parameter)) ||                              \
    (defined(ast_node_def_include) && defined(ast_node_def_include_Parameter))
ast_node_def(Parameter)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_FunctionPrototype)) ||                      \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_FunctionPrototype))
ast_node_def(FunctionPrototype)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_FunctionDefinition)) ||                     \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_FunctionDefinition))
ast_node_def(FunctionDefinition)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_ExternDefinition)) ||                       \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_ExternDefinition))
ast_node_def(ExternDefinition)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_InitDefinition)) ||                         \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_InitDefinition))
ast_node_def(InitDefinition)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_Statement)) ||                              \
    (defined(ast_node_def_include) && defined(ast_node_def_include_Statement))
ast_node_def(Statement)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) && !defined(ast_node_def_exclude_Scope)) || \
    (defined(ast_node_def_include) && defined(ast_node_def_include_Scope))
ast_node_def(Scope)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_Expression)) ||                             \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_Expression))
ast_node_def(Expression)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_ExpressionStatement)) ||                    \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_ExpressionStatement))
ast_node_def(ExpressionStatement)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_DefExpression)) ||                          \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_DefExpression))
ast_node_def(DefExpression)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_ClassDefinition)) ||                        \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_ClassDefinition))
ast_node_def(ClassDefinition)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_OperatorDefinition)) ||                     \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_OperatorDefinition))
ast_node_def(OperatorDefinition)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) && !defined(ast_node_def_exclude_Type)) ||  \
    (defined(ast_node_def_include) && defined(ast_node_def_include_Type))
ast_node_def(Type)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_PrimitiveType)) ||                          \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_PrimitiveType))
ast_node_def(PrimitiveType)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_ArrayType)) ||                              \
    (defined(ast_node_def_include) && defined(ast_node_def_include_ArrayType))
ast_node_def(ArrayType)
#endif
// #if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
//     (defined(ast_node_def_exclude) &&                                          \
//      !defined(ast_node_def_exclude_TupleType)) ||                              \
//     (defined(ast_node_def_include) && defined(ast_node_def_include_TupleType))
// ast_node_def(TupleType)
// #endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_CallableType)) ||                           \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_CallableType))
ast_node_def(CallableType)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_ClassType)) ||                              \
    (defined(ast_node_def_include) && defined(ast_node_def_include_ClassType))
ast_node_def(ClassType)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_IfStatement)) ||                            \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_IfStatement))
ast_node_def(IfStatement)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_WhileStatement)) ||                         \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_WhileStatement))
ast_node_def(WhileStatement)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_ForStatement)) ||                           \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_ForStatement))
ast_node_def(ForStatement)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_ReturnStatement)) ||                        \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_ReturnStatement))
ast_node_def(ReturnStatement)
#endif
// #if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
//     (defined(ast_node_def_exclude) &&                                          \
//      !defined(ast_node_def_exclude_Closure)) ||                                \
//     (defined(ast_node_def_include) && defined(ast_node_def_include_Closure))
// ast_node_def(Closure)
// #endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_CallExpression)) ||                         \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_CallExpression))
ast_node_def(CallExpression)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_UseExpression)) ||                          \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_UseExpression))
ast_node_def(UseExpression)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_IntExpression)) ||                          \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_IntExpression))
ast_node_def(IntExpression)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_UIntExpression)) ||                         \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_UIntExpression))
ast_node_def(UIntExpression)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_RealExpression)) ||                         \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_RealExpression))
ast_node_def(RealExpression)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) &&                                          \
     !defined(ast_node_def_exclude_StringExpression)) ||                       \
    (defined(ast_node_def_include) &&                                          \
     defined(ast_node_def_include_StringExpression))
ast_node_def(StringExpression)
#endif
#if(!defined(ast_node_def_exclude) && !defined(ast_node_def_include)) ||       \
    (defined(ast_node_def_exclude) && !defined(ast_node_def_exclude_Nil)) ||   \
    (defined(ast_node_def_include) && defined(ast_node_def_include_Nil))
ast_node_def(Nil)
#endif
//clang-format on

#undef ast_node_def_exclude
#undef ast_node_def_include

#undef ast_node_def_exclude_ASTNode
#undef ast_node_def_include_ASTNode
#undef ast_node_def_exclude_Parameter
#undef ast_node_def_include_Parameter
#undef ast_node_def_exclude_FunctionPrototype
#undef ast_node_def_include_FunctionPrototype
#undef ast_node_def_exclude_FunctionDefinition
#undef ast_node_def_include_FunctionDefinition
#undef ast_node_def_exclude_ExternDefinition
#undef ast_node_def_include_ExternDefinition
#undef ast_node_def_exclude_InitDefinition
#undef ast_node_def_include_InitDefinition
#undef ast_node_def_exclude_Statement
#undef ast_node_def_include_Statement
#undef ast_node_def_exclude_Scope
#undef ast_node_def_include_Scope
#undef ast_node_def_exclude_Expression
#undef ast_node_def_include_Expression
#undef ast_node_def_exclude_ExpressionStatement
#undef ast_node_def_include_ExpressionStatement
#undef ast_node_def_exclude_DefExpression
#undef ast_node_def_include_DefExpression
#undef ast_node_def_exclude_ClassDefinition
#undef ast_node_def_include_ClassDefinition
#undef ast_node_def_exclude_OperatorDefinition
#undef ast_node_def_include_OperatorDefinition
#undef ast_node_def_exclude_Type
#undef ast_node_def_include_Type
#undef ast_node_def_exclude_PrimitiveType
#undef ast_node_def_include_PrimitiveType
#undef ast_node_def_exclude_ArrayType
#undef ast_node_def_include_ArrayType
// #undef ast_node_def_exclude_TupleType
// #undef ast_node_def_include_TupleType
#undef ast_node_def_exclude_CallableType
#undef ast_node_def_include_CallableType
#undef ast_node_def_exclude_ClassType
#undef ast_node_def_include_ClassType
#undef ast_node_def_exclude_IfStatement
#undef ast_node_def_include_IfStatement
#undef ast_node_def_exclude_WhileStatement
#undef ast_node_def_include_WhileStatement
#undef ast_node_def_exclude_ForStatement
#undef ast_node_def_include_ForStatement
#undef ast_node_def_exclude_ReturnStatement
#undef ast_node_def_include_ReturnStatement
// #undef ast_node_def_exclude_Closure
// #undef ast_node_def_include_Closure
#undef ast_node_def_exclude_CallExpression
#undef ast_node_def_include_CallExpression
#undef ast_node_def_exclude_UseExpression
#undef ast_node_def_include_UseExpression
#undef ast_node_def_exclude_IntExpression
#undef ast_node_def_include_IntExpression
#undef ast_node_def_exclude_UIntExpression
#undef ast_node_def_include_UIntExpression
#undef ast_node_def_exclude_RealExpression
#undef ast_node_def_include_RealExpression
#undef ast_node_def_exclude_StringExpression
#undef ast_node_def_include_StringExpression
#undef ast_node_def_exclude_Nil
#undef ast_node_def_include_Nil

#undef ast_node_def
