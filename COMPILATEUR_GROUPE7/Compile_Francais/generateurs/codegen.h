// codegen.h (version corrigée et synchronisée)

#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <stdio.h> // Indispensable pour le type FILE

/* --- Fonctions principales de génération de code --- */

// La fonction principale qui orchestre tout
void generate_c_code(ASTNode* root, FILE* output);

/* --- Fonctions auxiliaires (déclarations correctes) --- */
void generate_expression(ASTNode* node, FILE* output);
void generate_statement(ASTNode* node, FILE* output, int indent);
DataType get_expression_type(ASTNode* node);

#endif