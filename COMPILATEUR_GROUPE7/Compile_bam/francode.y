//---> PATH: /home/romeo/Bureau/Mes_cours_M1/Semestre2/compilation/Compile4 (1)/Compile_final (3)/Compile/francode.y

/* francode.y (Version finale avec ternaire corrigé) */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generateurs/ast.h"
#include "generateurs/symtab.h"
#include "generateurs/codegen.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern int yylineno;
void yyerror(const char* s);
ASTNode* root = NULL;
%}

%union {
    int num;
    double d_val;
    char* str;
    ASTNode* node;
    DataType type_val;
    DimensionNode* dim_list;
    UnaryOperator un_op;
}

%token TOKEN_TYPE_ENTIER TOKEN_TYPE_REEL TOKEN_TYPE_CHAINE
%token PROGRAMME AFFICHER SI SINON TANTQUE POUR REPETER JUSQUA SELON CAS DEFAUT RETOURNER LIRE
%token <str> STRING_LITERAL
%token <num> INT_LITERAL
%token <d_val> FLOAT_LITERAL
%token <str> IDENTIFIER
%token EQ NE LE GE LT GT AND OR NOT ASSIGN PLUS MINUS MULT DIV MOD
%token COLON /* <<<--- COLON est maintenant le seul token pour ":" */
%token PV LBRACE RBRACE PO PF LBRACKET RBRACKET QUESTION_MARK

%type <node> programme statement_list case_list
%type <node> statement expression block declaration
%type <type_val> type_specifier
%type <node> if_statement while_statement for_statement repeat_statement switch_statement
%type <node> print_statement assignment return_statement
%type <node> case_branch default_branch
%type <node> assignment_expr logical_or_expr logical_and_expr equality_expr relational_expr additive_expr multiplicative_expr unary_expr primary_expression
%type <node> function_call
%type <node> lvalue ternary_expr
%type <dim_list> dimension_specifier_list index_list

%right ASSIGN
%right QUESTION_MARK COLON /* <<<--- CORRECTION ICI : la précédence utilise aussi COLON */
%left OR
%left AND
%nonassoc EQ NE LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT
%precedence UMINUS
%precedence THEN

%%

programme: PROGRAMME LBRACE statement_list RBRACE { root = $3; };

statement_list: %empty { $$ = create_block_node(NULL, 0); }
    | statement_list statement { if ($2) { $1->data.block.statements = realloc($1->data.block.statements, ($1->data.block.statement_count + 1) * sizeof(ASTNode*)); $1->data.block.statements[$1->data.block.statement_count] = $2; $1->data.block.statement_count++; } $$ = $1; };

statement: declaration | assignment | print_statement | if_statement | while_statement | for_statement | repeat_statement | switch_statement | return_statement | block | PV { $$ = NULL; };

block: LBRACE statement_list RBRACE { $$ = $2; };

dimension_specifier_list: %empty { $$ = NULL; }
    | dimension_specifier_list LBRACKET INT_LITERAL RBRACKET { $$ = create_dimension_node(create_int_node($3), $1); };

declaration: type_specifier IDENTIFIER dimension_specifier_list PV { if (!symtab_add_symbol($2, $1, $3)) { fprintf(stderr, "Ligne %d: Var '%s' déjà déclarée.\n", yylineno, $2); YYABORT; } $$ = create_declaration_node($1, $2, $3, NULL); free($2); }
    | type_specifier IDENTIFIER ASSIGN expression PV { if (!symtab_add_symbol($2, $1, NULL)) { fprintf(stderr, "Ligne %d: Var '%s' déjà déclarée.\n", yylineno, $2); YYABORT; } DataType vt = $1; DataType et = get_expression_type($4); if (vt != et && !(vt == TYPE_REEL && et == TYPE_ENTIER)) { fprintf(stderr, "Ligne %d: Incompatibilité de type init '%s'.\n", yylineno, $2); YYABORT; } symtab_set_initialized($2); $$ = create_declaration_node(vt, $2, NULL, $4); free($2); };

type_specifier: TOKEN_TYPE_ENTIER { $$ = TYPE_ENTIER; } | TOKEN_TYPE_REEL { $$ = TYPE_REEL; } | TOKEN_TYPE_CHAINE { $$ = TYPE_CHAINE; };

index_list: LBRACKET expression RBRACKET { $$ = create_dimension_node($2, NULL); }
          | index_list LBRACKET expression RBRACKET { $$ = create_dimension_node($3, $1); };

lvalue: IDENTIFIER { $$ = create_identifier_node($1); free($1); }
      | IDENTIFIER index_list { $$ = create_array_access_node(create_identifier_node($1), $2); free($1); };

assignment: lvalue ASSIGN expression PV { Symbol* s = symtab_lookup(((ASTNode*)$1)->data.name); if(s) symtab_set_initialized(s->name); $$ = create_assignment_node($1, $3); };

print_statement: AFFICHER expression PV { $$ = create_print_node($2); };

if_statement: SI PO expression PF block { $$ = create_if_node($3, $5, NULL); }
            | SI PO expression PF block SINON block { $$ = create_if_node($3, $5, $7); };

while_statement: TANTQUE PO expression PF block { $$ = create_while_node($3, $5); };

for_statement: POUR PO assignment_expr PV expression PV assignment_expr PF block { $$ = create_for_node($3, $5, $7, $9); };

repeat_statement: REPETER block JUSQUA PO expression PF PV { $$ = create_repeat_node($2, $5); };

return_statement: RETOURNER expression PV { $$ = create_return_node($2); };

switch_statement: SELON PO expression PF LBRACE case_list default_branch RBRACE { $$ = create_switch_node($3, $6, $7); };
case_list: %empty { $$ = create_block_node(NULL, 0); }
         | case_list case_branch { $1->data.block.statements = realloc($1->data.block.statements, ($1->data.block.statement_count + 1) * sizeof(ASTNode*)); $1->data.block.statements[$1->data.block.statement_count] = $2; $1->data.block.statement_count++; $$ = $1; };
case_branch: CAS INT_LITERAL COLON block { $$ = create_case_node($2, $4); };
default_branch: %empty { $$ = NULL; } | DEFAUT COLON block { $$ = $3; };

expression: assignment_expr;
assignment_expr: ternary_expr | lvalue ASSIGN assignment_expr { symtab_set_initialized(((ASTNode*)$1)->data.name); $$ = create_assignment_node($1, $3); };
ternary_expr: logical_or_expr | logical_or_expr QUESTION_MARK expression COLON ternary_expr { $$ = create_ternary_op_node($1, $3, $5); }; /* <<<--- CORRECTION ICI */
logical_or_expr: logical_and_expr | logical_or_expr OR logical_and_expr { $$ = create_binary_op_node(OR_OP, $1, $3); };
logical_and_expr: equality_expr | logical_and_expr AND equality_expr { $$ = create_binary_op_node(AND_OP, $1, $3); };
equality_expr: relational_expr | equality_expr EQ relational_expr { $$ = create_binary_op_node(EQ_OP, $1, $3); } | equality_expr NE relational_expr { $$ = create_binary_op_node(NE_OP, $1, $3); };
relational_expr: additive_expr | relational_expr LT additive_expr { $$ = create_binary_op_node(LT_OP, $1, $3); } | relational_expr GT additive_expr { $$ = create_binary_op_node(GT_OP, $1, $3); } | relational_expr LE additive_expr { $$ = create_binary_op_node(LE_OP, $1, $3); } | relational_expr GE additive_expr { $$ = create_binary_op_node(GE_OP, $1, $3); };
additive_expr: multiplicative_expr | additive_expr PLUS multiplicative_expr { $$ = create_binary_op_node(PLUS_OP, $1, $3); } | additive_expr MINUS multiplicative_expr { $$ = create_binary_op_node(MINUS_OP, $1, $3); };
multiplicative_expr: unary_expr | multiplicative_expr MULT unary_expr { $$ = create_binary_op_node(MULT_OP, $1, $3); } | multiplicative_expr DIV unary_expr { $$ = create_binary_op_node(DIV_OP, $1, $3); } | multiplicative_expr MOD unary_expr { $$ = create_binary_op_node(MOD_OP, $1, $3); };
unary_expr: primary_expression | NOT unary_expr { $$ = create_unary_op_node(UNARY_NOT_OP, $2); } | MINUS unary_expr %prec UMINUS { $$ = create_unary_op_node(UNARY_MINUS_OP, $2); };
primary_expression: INT_LITERAL { $$ = create_int_node($1); } | FLOAT_LITERAL { $$ = create_float_node($1); } | STRING_LITERAL { $$ = create_string_node($1); free($1); } | lvalue { $$ = $1; } | PO expression PF { $$ = $2; } | function_call;
function_call: LIRE PO PF { $$ = create_function_call_node("lire", NULL); };

%%
void yyerror(const char* s) { fprintf(stderr, "[Erreur] Ligne %d: %s\n", yylineno, s); }

