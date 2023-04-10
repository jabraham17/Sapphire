%{
    #include <stdio.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <string.h>
    #include "sapphire.yy.hpp"
    size_t line_num = 1;
    extern int yylex();
%}

DIGIT [0-9]
HEX_DIGIT [0-9a-fA-F]
BIN_DIGIT [01]
CHAR [a-zA-Z]
CHAR_UND ({CHAR}|"_")

ID {CHAR_UND}({CHAR_UND}|{DIGIT})*

INTEGER_LITERAL {DIGIT}+
HEX_LITERAL "0x"{HEX_DIGIT}+
BINARY_LITERAL "0b"{BIN_DIGIT}+
REAL_LITERAL {DIGIT}*"."{DIGIT}+

%option noyywrap
%option nomain

%x COMMENTS
%x STRING_PARSE

%%

    /* keywords */
"func"     { return FUNC; }
"var"      { return VAR; }
"class"    { return CLASS; }
"extern"   { return EXTERN; }
"if"       { return IF; }
"then"     { return THEN; }
"else"     { return ELSE; }
"for"      { return FOR; }
"while"    { return WHILE; }
"in"       { return IN; }
"return"   { return RETURN; }
"new"      { return NEW; }
"nil"      { return NIL; }
"ref"      { return REF; }
"operator" { return OPERATOR; }

    /* reserved words */
"init"   { return INIT; }
"deinit" { return DEINIT; }
"this"   { return THIS; }

    /* operators */
"+"  { return PLUS; }
"-"  { return MINUS; }
"*"  { return MULTIPLY; }
"/"  { return DIVIDE; }
"%"  { return MODULO; }
"<<" { return LSHIFT; }
">>" { return RSHIFT; }
"==" { return EQ; }
"="  { return EQUALS; }
"!=" { return NEQ; }
    /* gets used for NEGATE and FORCE_UNWRAP */
"!"  { return BANG; }
"<"  { return LT; }
"<=" { return LTEQ; }
">"  { return GT; }
">=" { return GTEQ; }
    /* gets used for both BW_AND and TAKE_REF */
"&"  { return AMPERSAND; }
"|"  { return BW_OR; }
"~"  { return BW_NEGATE; }
"&&" { return AND; }
"||" { return OR; }
"?"  { return QUESTION; }

    /* misc symbols */
"->" { return ARROW; }
":"  { return COLON; }
","  { return COMMA; }
"."  { return DOT; }
";"  { return SEMICOLON; }

    /* types */
"bool"   { return BOOL; }
"byte"   { return BYTE; }
"int"    { return INT; }
"uint"   { return UINT; }
"real"   { return REAL; }
"string" {return STRING; }


    /* grouping */
"("  { return LPAREN; }
")"  { return RPAREN; }
[\{] { return LCURLY; }
[\}] { return RCURLY; }
[\[] { return LSQUARE; }
[\]] { return RSQUARE; }


    /* primaries */
{ID} {   
    yylval.lexeme = strdup(yytext);
    return ID; 
}
{INTEGER_LITERAL} { 
    yylval.lexeme = strdup(yytext);
    return INTEGER_LITERAL;
}
{HEX_LITERAL} { 
    yylval.lexeme = strdup(yytext);
    return HEX_LITERAL;
}
{BINARY_LITERAL} { 
    yylval.lexeme = strdup(yytext);
    return BINARY_LITERAL;
}
{REAL_LITERAL} { 
    yylval.lexeme = strdup(yytext);
    return REAL_LITERAL;
}
    /* string needs custom state to handle escapes */
["] { BEGIN(STRING_PARSE); }
<STRING_PARSE>[\\]["] {}
<STRING_PARSE>["] {
    BEGIN(INITIAL);
    yylval.lexeme = strdup(yytext);
    return STRING_LITERAL;
}
<STRING_PARSE>. {}


    /* comments */
"//".* {}
"#".* {}
[\/][*] { BEGIN(COMMENTS); }
<COMMENTS>[*][\/] { BEGIN(INITIAL); }
<COMMENTS>. {}

    /* whitespace */
    /* no matter what count line numbers*/
<INITIAL,COMMENTS>\n { line_num++; }
[ \t] {}
. { return ERROR; }

%%