// generateurs/codegen.c (Version finale avec ternaire, tableaux et toutes les structures)

#include "codegen.h"
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prototypes internes */
void generate_expression(ASTNode* node, FILE* output);
void generate_statement(ASTNode* node, FILE* output, int indent);

/* --- La fonction clé pour l'analyse sémantique --- */
DataType get_expression_type(ASTNode* node) {
    if (!node) return TYPE_INCONNU;
    switch(node->type) {
        case INT_NODE: return TYPE_ENTIER;
        case FLOAT_NODE: return TYPE_REEL;
        case STRING_NODE: return TYPE_CHAINE;
        case ARRAY_ACCESS_NODE: {
            Symbol* s = symtab_lookup(node->data.array_access.array_node->data.name);
            return s ? s->type : TYPE_INCONNU;
        }
        case IDENTIFIER_NODE: {
            Symbol* s = symtab_lookup(node->data.name);
            return s ? s->type : TYPE_INCONNU;
        }
        case BINARY_OP_NODE: {
            DataType left_type = get_expression_type(node->data.binary.left);
            DataType right_type = get_expression_type(node->data.binary.right);
            if (left_type >= TYPE_TABLEAU_ENTIER || right_type >= TYPE_TABLEAU_ENTIER) {
                fprintf(stderr, "Erreur Sémantique: Opérations sur tableaux non supportées.\n");
                exit(1);
            }
            if (left_type == TYPE_CHAINE || right_type == TYPE_CHAINE) return TYPE_CHAINE;
            if (left_type == TYPE_REEL || right_type == TYPE_REEL) return TYPE_REEL;
            return TYPE_ENTIER;
        }
        case UNARY_OP_NODE:
            if (node->data.unary.op == UNARY_NOT_OP) return TYPE_ENTIER;
            return get_expression_type(node->data.unary.operand);
        case TERNARY_OP_NODE: {
            DataType true_type = get_expression_type(node->data.ternary.true_expr);
            DataType false_type = get_expression_type(node->data.ternary.false_expr);
            if (true_type == TYPE_REEL || false_type == TYPE_REEL) return TYPE_REEL;
            if (true_type == TYPE_ENTIER || false_type == TYPE_ENTIER) return TYPE_ENTIER;
            return TYPE_CHAINE; // Par défaut, ou si les deux sont des chaînes
        }
        case FUNCTION_CALL_NODE:
            return TYPE_ENTIER;
        default: return TYPE_INCONNU;
    }
}

void generate_statement(ASTNode* node, FILE* output, int indent) {
    if (!node) return;
    if (node->type != DECLARATION_NODE && node->type != BLOCK_NODE) {
        fprintf(output, "%*s", indent * 4, "");
    }

    switch (node->type) {
        case DECLARATION_NODE:
            if (node->data.declaration.initial_value) {
                fprintf(output, "%*s%s = ", indent * 4, "", node->data.declaration.name);
                generate_expression(node->data.declaration.initial_value, output);
                fprintf(output, ";\n");
            }
            break;
        case ASSIGNMENT_NODE:
            generate_expression(node->data.assignment.lvalue, output);
            fprintf(output, " = ");
            generate_expression(node->data.assignment.value, output);
            fprintf(output, ";\n");
            break;
        case PRINT_NODE: {
            ASTNode* expr_node = node->data.unary.operand;
            DataType type = get_expression_type(expr_node);
            fprintf(output, "printf(");
            if (type == TYPE_ENTIER) fprintf(output, "\"%%d\\n\", ");
            else if (type == TYPE_REEL) fprintf(output, "\"%%f\\n\", ");
            else if (type == TYPE_CHAINE) fprintf(output, "\"%%s\\n\", ");
            else { 
                fprintf(output, "\"Erreur: Type d'affichage inconnu\\n\");\n");
                break;
            }
            generate_expression(expr_node, output);
            fprintf(output, ");\n");
            break;
        }
        case IF_NODE:
            fprintf(output, "if (");
            generate_expression(node->data.if_stmt.condition, output);
            fprintf(output, ") ");
            generate_statement(node->data.if_stmt.if_block, output, indent);
            if (node->data.if_stmt.else_block) {
                fprintf(output, "%*selse ", indent * 4, "");
                generate_statement(node->data.if_stmt.else_block, output, indent);
            }
            break;
        case WHILE_NODE:
             fprintf(output, "while (");
             generate_expression(node->data.while_loop.condition, output);
             fprintf(output, ") ");
             generate_statement(node->data.while_loop.loop_block, output, indent);
             break;
        case FOR_NODE:
             fprintf(output, "for (");
             generate_expression(node->data.for_loop.init, output);
             fprintf(output, "; ");
             generate_expression(node->data.for_loop.condition, output);
             fprintf(output, "; ");
             generate_expression(node->data.for_loop.increment, output);
             fprintf(output, ") ");
             generate_statement(node->data.for_loop.body, output, indent);
             break;
        case REPEAT_NODE:
             fprintf(output, "do ");
             generate_statement(node->data.repeat_loop.body, output, indent);
             fprintf(output, "%*swhile (!(", indent * 4, "");
             generate_expression(node->data.repeat_loop.condition, output);
             fprintf(output, "));\n");
             break;
        case RETURN_NODE:
             fprintf(output, "return ");
             if(node->data.unary.operand) {
                generate_expression(node->data.unary.operand, output);
             }
             fprintf(output, ";\n");
             break;
        case BLOCK_NODE:
            if (indent > 0) fprintf(output, "%*s", (indent-1) * 4, "");
            fprintf(output, "{\n");
            for (int i = 0; i < node->data.block.statement_count; i++) {
                generate_statement(node->data.block.statements[i], output, indent + 1);
            }
            fprintf(output, "%*s}\n", indent * 4, "");
            break;
        case SWITCH_NODE:
            fprintf(output, "switch (");
            generate_expression(node->data.switch_stmt.expression, output);
            fprintf(output, ") {\n");
            ASTNode* cases_block = node->data.switch_stmt.cases_block;
            if (cases_block) {
                for (int i = 0; i < cases_block->data.block.statement_count; i++) {
                    ASTNode* case_node = cases_block->data.block.statements[i];
                    if (case_node->type == CASE_NODE) {
                        fprintf(output, "%*scase %d:\n", (indent + 1) * 4, "", case_node->data.case_branch.case_value);
                        generate_statement(case_node->data.case_branch.block, output, indent + 2);
                        fprintf(output, "%*sbreak;\n", (indent + 2) * 4, "");
                    }
                }
            }
            if (node->data.switch_stmt.default_case) {
                fprintf(output, "%*sdefault:\n", (indent + 1) * 4, "");
                generate_statement(node->data.switch_stmt.default_case, output, indent + 2);
                fprintf(output, "%*sbreak;\n", (indent + 2) * 4, "");
            }
            fprintf(output, "%*s}\n", indent * 4, "");
            break;
        default: break;
    }
}

void generate_expression(ASTNode* node, FILE* output) {
    if (!node) return;
    switch (node->type) {
        case INT_NODE: fprintf(output, "%d", node->data.integer.i_val); break;
        case FLOAT_NODE: fprintf(output, "%f", node->data.floating.f_val); break;
        case STRING_NODE: fprintf(output, "\"%s\"", node->data.name); break;
        case IDENTIFIER_NODE: fprintf(output, "%s", node->data.name); break;
        case ARRAY_ACCESS_NODE:
            generate_expression(node->data.array_access.array_node, output);
            DimensionNode* current = node->data.array_access.indices;
            while(current) {
                fprintf(output, "[");
                generate_expression(current->size_expr, output);
                fprintf(output, "]");
                current = current->next;
            }
            break;
        case BINARY_OP_NODE: {
            const char* op_str[] = {"+", "-", "*", "/", "%", "<", "<=", ">", ">=", "==", "!=", "&&", "||"};
            fprintf(output, "(");
            generate_expression(node->data.binary.left, output);
            fprintf(output, " %s ", op_str[node->data.binary.op]);
            generate_expression(node->data.binary.right, output);
            fprintf(output, ")");
            break;
        }
        case UNARY_OP_NODE:
            if (node->data.unary.op == UNARY_MINUS_OP) fprintf(output, "(-");
            else if (node->data.unary.op == UNARY_NOT_OP) fprintf(output, "(!");
            generate_expression(node->data.unary.operand, output);
            fprintf(output, ")");
            break;
        case TERNARY_OP_NODE:
            fprintf(output, "(");
            generate_expression(node->data.ternary.condition, output);
            fprintf(output, " ? ");
            generate_expression(node->data.ternary.true_expr, output);
            fprintf(output, " : ");
            generate_expression(node->data.ternary.false_expr, output);
            fprintf(output, ")");
            break;
        case FUNCTION_CALL_NODE:
            if (strcmp(node->data.function_call.function_name, "lire") == 0) {
                fprintf(output, "lire_entier()"); 
            }
            break;
        case ASSIGNMENT_NODE:
            generate_expression(node->data.assignment.lvalue, output);
            fprintf(output, " = ");
            generate_expression(node->data.assignment.value, output);
            break;
        default:
            fprintf(stderr, "Erreur de génération : type d'expression inconnu %d\n", node->type);
            break;
    }
}

void generate_c_code(ASTNode* root, FILE* output) {
    if (!root || root->type != BLOCK_NODE) {
        fprintf(stderr, "Racine de l'AST invalide.\n");
        return;
    }
    
    fprintf(output, "#include <stdio.h>\n#include <string.h>\n\n");
    fprintf(output, "int lire_entier() { int val; printf(\"> \"); fflush(stdout); if(scanf(\"%%d\", &val)!=1) return 0; return val; }\n\n");
    fprintf(output, "int main(void) {\n");
    
    // Pass 1: Déclarer toutes les variables et tableaux
    for (int i = 0; i < root->data.block.statement_count; i++) {
        ASTNode* stmt = root->data.block.statements[i];
        if (stmt->type == DECLARATION_NODE) {
            char* type_str = "void";
            if (stmt->data.declaration.data_type == TYPE_ENTIER) type_str = "int";
            else if (stmt->data.declaration.data_type == TYPE_REEL) type_str = "double";
            else if (stmt->data.declaration.data_type == TYPE_CHAINE) type_str = "char*";
            
            fprintf(output, "    %s %s", type_str, stmt->data.declaration.name);
            
            DimensionNode* current = stmt->data.declaration.dimensions;
            while(current) {
                fprintf(output, "[%d]", current->size_expr->data.integer.i_val);
                current = current->next;
            }
            fprintf(output, ";\n");
        }
    }
    
    fprintf(output, "\n");
    
    // Pass 2: Générer le code pour les instructions (y compris les initialisations)
    for (int i = 0; i < root->data.block.statement_count; i++) {
        ASTNode* stmt = root->data.block.statements[i];
        if (stmt->type != DECLARATION_NODE || stmt->data.declaration.initial_value != NULL) {
            generate_statement(stmt, output, 1);
        }
    }
    
    fprintf(output, "    return 0;\n}\n");
}