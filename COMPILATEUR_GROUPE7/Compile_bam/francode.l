%{
#include <stdio.h>
#include <string.h>
#include "generateurs/ast.h"
#include "francode.tab.h"
%}

%option noyywrap yylineno nounput noinput

%%

"//".*          ;
[ \t\r\n]+      ;

"zə̀n nə̀ dzə́ə̀"     { return PROGRAMME; }
"tə̀n"               { return TOKEN_TYPE_ENTIER; }
"pə̀n"               { return TOKEN_TYPE_REEL; }
"kə̀mə̀n"            { return TOKEN_TYPE_CHAINE; }
"nzuə̀"              { return AFFICHER; }
"yə̀n"               { return SI; }
"yə̀n mə̀"           { return SINON; }
"ləfɛ mə̀ yɛǹ"      { return TANTQUE; }
"pə̀"                { return POUR; }
"tə̀pə̀"             { return REPETER; }
"kə̀tə̀"             { return JUSQUA; }
"sə̀lə̀n"            { return SELON; }
"kàs"                { return CAS; }
"də̀fà"              { return DEFAUT; }
"də̀ tɔ̀"            { return RETOURNER; }
"fɔ́ɔ̀"              { return LIRE; }
"nə̀"                { return AND; }
"wə̀"                { return OR; }
"nà"                 { return NOT; }

[0-9]+\.[0-9]+      { yylval.d_val = atof(yytext); return FLOAT_LITERAL; }
[0-9]+              { yylval.num = atoi(yytext); return INT_LITERAL; }
"\""[^"]*"\""       { yytext[strlen(yytext) - 1] = '\0'; yylval.str = strdup(yytext + 1); return STRING_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return IDENTIFIER; }

"=="                { return EQ; }
"!="                { return NE; }
"<="                { return LE; }
">="                { return GE; }
"<"                 { return LT; }
">"                 { return GT; }
"="                 { return ASSIGN; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return MULT; }
"/"                 { return DIV; }
"%"                 { return MOD; }
"?"                 { return QUESTION_MARK; }
":"                 { return COLON; }
";"                 { return PV; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
"("                 { return PO; }
")"                 { return PF; }
"["                 { return LBRACKET; }
"]"                 { return RBRACKET; }

.                   { fprintf(stderr, "[Erreur Lexicale] Ligne %d: Caractère non reconnu: '%s'\n", yylineno, yytext); }

%%