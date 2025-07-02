// generateurs/symtab.c (Version complète avec tableaux et matrices)

#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static SymbolTable symbol_table;

void symtab_init() {
    symbol_table.count = 0;
}

void symtab_free() {
    for (int i = 0; i < symbol_table.count; i++) {
        if (symbol_table.symbols[i].name) {
            free(symbol_table.symbols[i].name);
        }
    }
    symbol_table.count = 0;
}

Symbol* symtab_lookup(const char* name) {
    for (int i = 0; i < symbol_table.count; i++) {
        if (strcmp(symbol_table.symbols[i].name, name) == 0) {
            return &symbol_table.symbols[i];
        }
    }
    return NULL;
}

int symtab_add_symbol(const char* name, DataType type, DimensionNode* dims) {
    if (symtab_lookup(name) != NULL) {
        return 0; // Symbole déjà déclaré
    }
    if (symbol_table.count >= MAX_SYMBOLS) {
        fprintf(stderr, "Erreur: Table des symboles pleine.\n");
        return 0;
    }
    
    Symbol* symbol = &symbol_table.symbols[symbol_table.count];
    symbol->name = strdup(name);
    symbol->type = type;
    symbol->is_initialized = 0;
    
    int i = 0;
    DimensionNode* current = dims;
    while (current != NULL && i < MAX_DIMS) {
        current = current->next;
        i++;
    }
    symbol->num_dims = i;
    
    symbol_table.count++;
    return 1;
}

void symtab_set_initialized(const char* name) {
    Symbol* symbol = symtab_lookup(name);
    if (symbol) {
        symbol->is_initialized = 1;
    }
}