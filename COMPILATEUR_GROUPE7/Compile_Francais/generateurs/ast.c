// generateurs/ast.c (Version finale avec tableaux, matrices et ternaire)

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* create_int_node(int value) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = INT_NODE;
    n->data.integer.i_val = value;
    return n;
}

ASTNode* create_float_node(double value) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = FLOAT_NODE;
    n->data.floating.f_val = value;
    return n;
}

ASTNode* create_string_node(char* value) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = STRING_NODE;
    n->data.name = strdup(value);
    return n;
}

ASTNode* create_identifier_node(char* name) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = IDENTIFIER_NODE;
    n->data.name = strdup(name);
    return n;
}

DimensionNode* create_dimension_node(ASTNode* size_expr, DimensionNode* next) {
    DimensionNode* n = malloc(sizeof(DimensionNode));
    if (!n) { perror("malloc"); exit(1); }
    n->size_expr = size_expr;
    n->next = next;
    return n;
}

void free_dimension_list(DimensionNode* list) {
    DimensionNode* current = list;
    while (current != NULL) {
        DimensionNode* next = current->next;
        free_ast(current->size_expr);
        free(current);
        current = next;
    }
}

ASTNode* create_declaration_node(DataType type, char* name, DimensionNode* dims, ASTNode* value) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = DECLARATION_NODE;
    n->data.declaration.data_type = type;
    n->data.declaration.name = strdup(name);
    n->data.declaration.dimensions = dims;
    n->data.declaration.initial_value = value;
    return n;
}

ASTNode* create_assignment_node(ASTNode* lvalue, ASTNode* value) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = ASSIGNMENT_NODE;
    n->data.assignment.lvalue = lvalue;
    n->data.assignment.value = value;
    return n;
}

ASTNode* create_array_access_node(ASTNode* array_node, DimensionNode* indices) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = ARRAY_ACCESS_NODE;
    n->data.array_access.array_node = array_node;
    n->data.array_access.indices = indices;
    return n;
}

ASTNode* create_binary_op_node(BinaryOperator op, ASTNode* left, ASTNode* right) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = BINARY_OP_NODE;
    n->data.binary.op = op;
    n->data.binary.left = left;
    n->data.binary.right = right;
    return n;
}

ASTNode* create_unary_op_node(UnaryOperator op, ASTNode* operand) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = UNARY_OP_NODE;
    n->data.unary.op = op;
    n->data.unary.operand = operand;
    return n;
}

ASTNode* create_ternary_op_node(ASTNode* cond, ASTNode* true_expr, ASTNode* false_expr) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = TERNARY_OP_NODE;
    n->data.ternary.condition = cond;
    n->data.ternary.true_expr = true_expr;
    n->data.ternary.false_expr = false_expr;
    return n;
}

ASTNode* create_block_node(ASTNode** statements, int count) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = BLOCK_NODE;
    n->data.block.statements = statements;
    n->data.block.statement_count = count;
    return n;
}

ASTNode* create_if_node(ASTNode* cond, ASTNode* if_b, ASTNode* else_b) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = IF_NODE;
    n->data.if_stmt.condition = cond;
    n->data.if_stmt.if_block = if_b;
    n->data.if_stmt.else_block = else_b;
    return n;
}

ASTNode* create_while_node(ASTNode* cond, ASTNode* loop_b) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = WHILE_NODE;
    n->data.while_loop.condition = cond;
    n->data.while_loop.loop_block = loop_b;
    return n;
}

ASTNode* create_for_node(ASTNode* init, ASTNode* condition, ASTNode* increment, ASTNode* body) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = FOR_NODE;
    n->data.for_loop.init = init;
    n->data.for_loop.condition = condition;
    n->data.for_loop.increment = increment;
    n->data.for_loop.body = body;
    return n;
}

ASTNode* create_repeat_node(ASTNode* body, ASTNode* condition) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = REPEAT_NODE;
    n->data.repeat_loop.body = body;
    n->data.repeat_loop.condition = condition;
    return n;
}

ASTNode* create_print_node(ASTNode* expr) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = PRINT_NODE;
    n->data.unary.operand = expr;
    return n;
}

ASTNode* create_return_node(ASTNode* expr) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = RETURN_NODE;
    n->data.unary.operand = expr;
    return n;
}

ASTNode* create_function_call_node(char* name, ASTNode* arg) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = FUNCTION_CALL_NODE;
    n->data.function_call.function_name = strdup(name);
    n->data.function_call.argument = arg;
    return n;
}

ASTNode* create_switch_node(ASTNode* expression, ASTNode* cases_block, ASTNode* default_case) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = SWITCH_NODE;
    n->data.switch_stmt.expression = expression;
    n->data.switch_stmt.cases_block = cases_block;
    n->data.switch_stmt.default_case = default_case;
    return n;
}

ASTNode* create_case_node(int value, ASTNode* block) {
    ASTNode* n = malloc(sizeof(ASTNode));
    if (!n) { perror("malloc"); exit(1); }
    n->type = CASE_NODE;
    n->data.case_branch.case_value = value;
    n->data.case_branch.block = block;
    return n;
}

void free_ast(ASTNode* node) {
    if (!node) return;
    switch (node->type) {
        case INT_NODE:
        case FLOAT_NODE:
            break;
        case STRING_NODE:
        case IDENTIFIER_NODE:
            free(node->data.name);
            break;
        case DECLARATION_NODE:
            free(node->data.declaration.name);
            if (node->data.declaration.dimensions) free_dimension_list(node->data.declaration.dimensions);
            if (node->data.declaration.initial_value) free_ast(node->data.declaration.initial_value);
            break;
        case ASSIGNMENT_NODE:
            free_ast(node->data.assignment.lvalue);
            free_ast(node->data.assignment.value);
            break;
        case ARRAY_ACCESS_NODE:
            free_ast(node->data.array_access.array_node);
            free_dimension_list(node->data.array_access.indices);
            break;
        case BINARY_OP_NODE:
            free_ast(node->data.binary.left);
            free_ast(node->data.binary.right);
            break;
        case UNARY_OP_NODE:
        case PRINT_NODE:
        case RETURN_NODE:
            free_ast(node->data.unary.operand);
            break;
        case TERNARY_OP_NODE:
            free_ast(node->data.ternary.condition);
            free_ast(node->data.ternary.true_expr);
            free_ast(node->data.ternary.false_expr);
            break;
        case BLOCK_NODE:
            for (int i = 0; i < node->data.block.statement_count; i++) {
                free_ast(node->data.block.statements[i]);
            }
            if(node->data.block.statements) free(node->data.block.statements);
            break;
        case IF_NODE:
            free_ast(node->data.if_stmt.condition);
            free_ast(node->data.if_stmt.if_block);
            if (node->data.if_stmt.else_block) free_ast(node->data.if_stmt.else_block);
            break;
        case WHILE_NODE:
            free_ast(node->data.while_loop.condition);
            free_ast(node->data.while_loop.loop_block);
            break;
        case FOR_NODE:
            free_ast(node->data.for_loop.init);
            free_ast(node->data.for_loop.condition);
            free_ast(node->data.for_loop.increment);
            free_ast(node->data.for_loop.body);
            break;
        case REPEAT_NODE:
            free_ast(node->data.repeat_loop.body);
            free_ast(node->data.repeat_loop.condition);
            break;
        case SWITCH_NODE:
            free_ast(node->data.switch_stmt.expression);
            free_ast(node->data.switch_stmt.cases_block);
            if (node->data.switch_stmt.default_case) free_ast(node->data.switch_stmt.default_case);
            break;
        case CASE_NODE:
            free_ast(node->data.case_branch.block);
            break;
        case FUNCTION_CALL_NODE:
            free(node->data.function_call.function_name);
            if (node->data.function_call.argument) free_ast(node->data.function_call.argument);
            break;
    }
    free(node);
}