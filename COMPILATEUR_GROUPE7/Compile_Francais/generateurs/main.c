// main.c (version corrigée avec tous les includes nécessaires)

//Tchiaze Fouoss Romero 21T2474
//Ndonkou Franck 21T2254
//Engoulou
//Nana
//Morel


#include "ast.h"
#include "codegen.h"
#include "symtab.h"  // <<<--- L'INCLUDE ESSENTIEL EST ICI
#include <stdio.h>
#include <stdlib.h>

extern FILE* yyin;
extern int yyparse();
extern ASTNode* root;

void yyerror(const char* s);

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s input.franco\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Erreur d'ouverture du fichier");
        return 1;
    }

    symtab_init(); // Maintenant la déclaration est vue via symtab.h

    if (yyparse() != 0) {
        fprintf(stderr, "Echec de l'analyse syntaxique.\n");
        fclose(yyin);
        return 1;
    }
    fclose(yyin);

    FILE* output = fopen("output.c", "w");
    if (!output) {
        perror("Erreur d'ouverture du fichier de sortie");
        free_ast(root);
        return 1;
    }

    generate_c_code(root, output);
    fclose(output);
    free_ast(root);
    symtab_free(); // Maintenant la déclaration est vue via symtab.h

    printf("Code C généré dans output.c. Compilation en cours...\n");
    int compile_status = system("gcc -Wall -Wextra output.c -o output");
    if (compile_status != 0) {
        fprintf(stderr, "Erreur lors de la compilation de output.c\n");
        return 1;
    }
    printf("Compilation réussie. Exécutable 'output' créé.\n");

    return 0;
}
