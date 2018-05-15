%{
/* This space is for C code, such as includes, definitions, functions, and
 * variables.
 * CSE443 Project 1 : Lexical Analyser
 * authors: Rupesh Bordiya @rupeshbo
 *          Amrit Pal Singh @asingh42
 *          Alex Rosa @arosa2
 *          Liam Carroll @lscarrol
 */
 #include <stdbool.h>
 #include <stdio.h>
 #include <stdlib.h>
#include "parser.tab.h"
#include "scopelist.c"


#define YY_DECL extern int yylex()
int hold = 0;
int linecounter = 1;
int currLine = 1;
int columncounter = 1;
int scope = 0;
L_NODE* head = NULL;
%}
/* This is for your definitions */
/* Any options you want for the lexer will be declared here also */

DIGIT		[0-9]
INTEGER	("+"|"-")?{DIGIT}+
REAL 		("+"|"-")?[0-9]+"."[0-9]+(("e"|"E")("+"|"-")?{DIGIT}+)?
STRING 		\"(\\.|[^\n^"\\])*\"
CHARACTER	'(\\.|[a-zA-Z0-9])*'
BOOLEAN		("true"|"false")

%option yylineno

%%
<<EOF>>                return END;
{INTEGER}              yylval.str = strdup(yytext); return C_INTEGER;
{REAL}			           yylval.str = strdup(yytext); return C_REAL;
{CHARACTER}		         yylval.str = strdup(yytext); return C_CHARACTER;
{STRING}	             yylval.str = strdup(yytext); return C_STRING;
"true"				         yylval.str = strdup(yytext); return C_TRUE;
"false"			           yylval.str = strdup(yytext); return C_FALSE;

"integer"	             yylval.keyword = strdup(yytext); return T_INTEGER;
"real"	               yylval.keyword = strdup(yytext); return T_REAL;
"Boolean"		           yylval.keyword = strdup(yytext); return T_BOOLEAN;
"character"            yylval.keyword = strdup(yytext); return T_CHARACTER;
"string"		           yylval.keyword = strdup(yytext); return T_STRING;
 /* comment v2.0 */
\(\*[^()]*\*\)         hold = linecounter;return COMMENT;

  /* Operators */
"+"                     yylval.str = strdup(yytext); return ADD;
"-"                     yylval.str = strdup(yytext); return SUB_OR_NEG;
"/"                     yylval.str = strdup(yytext); return DIV;
"*"                     yylval.str = strdup(yytext); return MUL;
"%"                     yylval.str = strdup(yytext); return REM;
"."                     yylval.str = strdup(yytext); return DOT;
"<"                     yylval.str = strdup(yytext); return LESS_THAN;
">"                     yylval.str = strdup(yytext); return GREATER_THAN;
"="                     yylval.str = strdup(yytext); return EQUAL_TO;
":="                    yylval.str = strdup(yytext); return ASSIGN;
"i2r"                   yylval.str = strdup(yytext); return INT2REAL;
"r2i"                   yylval.str = strdup(yytext); return REAL2INT;
"isNull"                yylval.str = strdup(yytext); return IS_NULL;
"!"                     yylval.str = strdup(yytext); return NOT;
"&"                     yylval.str = strdup(yytext);  return AND;
"|"                     yylval.str = strdup(yytext); return OR;
[ ]+			              columncounter = columncounter +yyleng;
[ \t]+                	columncounter = columncounter +yyleng;
[\n]                    insertList(&head, linecounter, columncounter, scope); linecounter = linecounter + 1; currLine = linecounter; columncounter = 1;

  /*KEYWORDS*/
"null"                  return NULL_PTR;
"reserve"               yylval.str = strdup(yytext); return RESERVE;
"release"               yylval.str = strdup(yytext); return RELEASE;
"for"                   return FOR;
"while"                 return WHILE;
"if"                    return IF;
"then"                  return THEN;
"else"                  return ELSE;
"switch"                return SWITCH;
"case"                  return CASE;
"otherwise"             return OTHERWISE;
"function"              return FUNCTION;
"closure"               return CLOSURE;
"type"                  yylval.keyword = strdup(yytext); return TYPE;

 /*Punctuations-GROUPING*/
"("                     yylval.str = strdup(yytext);return L_PARENTHESIS;
")"                     yylval.str = strdup(yytext);return R_PARENTHESIS;
"["                     return L_BRACKET;
"]"                     return R_BRACKET;
"{"                     scope++; return L_BRACE;
"}"                     scope--; return R_BRACE;
"'"                     return S_QUOTE;
"\""                    return D_QUOTE;

	/*Punctuations - other*/
";"                     return SEMI_COLON;
":"                     return COLON;
","                     yylval.str = strdup(yytext); return COMMA;
"->"                    return ARROW;
"\\"                    return BACKSLASH;


[A-Za-z0-9]*         yylval.str = strdup(yytext); return ID;

%%
int yywrap(void){
  return 1;
}
