// generateurs/ast.h (Version complète avec opérateur ternaire)

#ifndef AST_H
#define AST_H

typedef enum {
    TYPE_INCONNU, TYPE_ENTIER, TYPE_REEL, TYPE_CHAINE,
    TYPE_TABLEAU_ENTIER, TYPE_TABLEAU_REEL, TYPE_TABLEAU_CHAINE
} DataType;

typedef enum {
    INT_NODE, FLOAT_NODE, STRING_NODE, IDENTIFIER_NODE,
    BINARY_OP_NODE, UNARY_OP_NODE,
    TERNARY_OP_NODE, // NOUVEAU
    BLOCK_NODE, DECLARATION_NODE, ASSIGNMENT_NODE, PRINT_NODE,
    IF_NODE, WHILE_NODE, FOR_NODE, REPEAT_NODE, SWITCH_NODE, CASE_NODE,
    RETURN_NODE, FUNCTION_CALL_NODE, ARRAY_ACCESS_NODE
} NodeType;

typedef enum { PLUS_OP, MINUS_OP, MULT_OP, DIV_OP, MOD_OP, LT_OP, LE_OP, GT_OP, GE_OP, EQ_OP, NE_OP, AND_OP, OR_OP } BinaryOperator;
typedef enum { UNARY_MINUS_OP, UNARY_NOT_OP } UnaryOperator;

typedef struct DimensionNode {
    struct ASTNode* size_expr;
    struct DimensionNode* next;
} DimensionNode;

typedef struct ASTNode {
    NodeType type;
    union {
        struct { int i_val; } integer;
        struct { double f_val; } floating;
        char* name;
        struct { BinaryOperator op; struct ASTNode* left; struct ASTNode* right; } binary;
        struct { UnaryOperator op; struct ASTNode* operand; } unary;
        struct { struct ASTNode* condition; struct ASTNode* true_expr; struct ASTNode* false_expr; } ternary;
        struct { struct ASTNode** statements; int statement_count; } block;
        struct { DataType data_type; char* name; DimensionNode* dimensions; struct ASTNode* initial_value; } declaration;
        struct { struct ASTNode* lvalue; struct ASTNode* value; } assignment;
        struct { struct ASTNode* array_node; DimensionNode* indices; } array_access;
        struct { struct ASTNode* condition; struct ASTNode* if_block; struct ASTNode* else_block; } if_stmt;
        struct { struct ASTNode* condition; struct ASTNode* loop_block; } while_loop;
        struct { struct ASTNode* init; struct ASTNode* condition; struct ASTNode* increment; struct ASTNode* body; } for_loop;
        struct { struct ASTNode* body; struct ASTNode* condition; } repeat_loop;
        struct { struct ASTNode* expression; struct ASTNode* cases_block; struct ASTNode* default_case; } switch_stmt;
        struct { int case_value; struct ASTNode* block; } case_branch;
        struct { char* function_name; struct ASTNode* argument; } function_call;
    } data;
} ASTNode;

// Prototypes complets
ASTNode* create_int_node(int value);
ASTNode* create_float_node(double value);
ASTNode* create_string_node(char* value);
ASTNode* create_identifier_node(char* name);
ASTNode* create_declaration_node(DataType type, char* name, DimensionNode* dims, ASTNode* value);
ASTNode* create_assignment_node(ASTNode* lvalue, ASTNode* value);
ASTNode* create_array_access_node(ASTNode* array_node, DimensionNode* indices);
DimensionNode* create_dimension_node(ASTNode* size_expr, DimensionNode* next);
void free_dimension_list(DimensionNode* list);
ASTNode* create_binary_op_node(BinaryOperator op, ASTNode* left, ASTNode* right);
ASTNode* create_unary_op_node(UnaryOperator op, ASTNode* operand);
ASTNode* create_ternary_op_node(ASTNode* cond, ASTNode* true_expr, ASTNode* false_expr);
ASTNode* create_block_node(ASTNode** statements, int count);
ASTNode* create_if_node(ASTNode* condition, ASTNode* if_block, ASTNode* else_block);
ASTNode* create_while_node(ASTNode* condition, ASTNode* loop_block);
ASTNode* create_for_node(ASTNode* init, ASTNode* condition, ASTNode* increment, ASTNode* body);
ASTNode* create_repeat_node(ASTNode* body, ASTNode* condition);
ASTNode* create_print_node(ASTNode* expr);
ASTNode* create_return_node(ASTNode* expr);
ASTNode* create_function_call_node(char* name, ASTNode* argument);
ASTNode* create_switch_node(ASTNode* expression, ASTNode* cases_block, ASTNode* default_case);
ASTNode* create_case_node(int value, ASTNode* block);

void free_ast(ASTNode* node);

#endif