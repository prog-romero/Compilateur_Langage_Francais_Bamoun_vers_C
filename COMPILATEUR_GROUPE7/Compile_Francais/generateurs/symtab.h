// generateurs/symtab.h (Version complète avec tableaux et matrices)

#ifndef SYMTAB_H
#define SYMTAB_H

#include "ast.h"

#define MAX_SYMBOLS 100
#define MAX_DIMS 5 // On supporte jusqu'à 5 dimensions

typedef struct {
    char* name;
    DataType type;
    int is_initialized;
    int num_dims; // Nombre de dimensions (0 pour une variable simple)
} Symbol;

typedef struct { Symbol symbols[MAX_SYMBOLS]; int count; } SymbolTable;

void symtab_init();
void symtab_free();
Symbol* symtab_lookup(const char* name);
int symtab_add_symbol(const char* name, DataType type, DimensionNode* dims);
void symtab_set_initialized(const char* name);

#endif