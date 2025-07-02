# Nom du compilateur C et options
CC = gcc
CFLAGS = -Wall -Wextra -g

# Outils de génération
LEX = flex
BISON = bison -d

# --- DÉFINITION DES CHEMINS ---
# Dossier où se trouvent vos sources .c et .h
SRC_DIR = generateurs
# Dossier où seront placés les fichiers objets .o
OBJ_DIR = obj

# --- DÉFINITION DES FICHIERS ---
# Nom de l'exécutable final de votre compilateur
COMPILATEUR = francode

# Liste des fichiers sources .c dans le sous-dossier
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Transformation de la liste des .c en liste de .o dans le dossier obj/
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Fichiers objets générés par Flex et Bison
COMPILER_OBJS = $(OBJ_DIR)/francode.tab.o $(OBJ_DIR)/francode.lex.o

# Cible par défaut : construit le compilateur
all: $(COMPILATEUR)

# Règle pour lier tous les objets et créer l'exécutable final
$(COMPILATEUR): $(OBJS) $(COMPILER_OBJS)
	@echo "--- Liaison de l'exécutable du compilateur '$(COMPILATEUR)' ---"
	$(CC) $(CFLAGS) -o $@ $^

# --- RÈGLES DE COMPILATION ---

# Règle générique pour compiler n'importe quel .c de SRC_DIR en .o dans OBJ_DIR
# L'option -I$(SRC_DIR) dit à gcc où trouver les fichiers .h
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR) # Crée le dossier obj/ s'il n'existe pas
	@echo "Compilation de $< -> $@"
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# Règles spécifiques pour les fichiers générés par Bison/Flex
$(OBJ_DIR)/francode.tab.o: francode.tab.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compilation de $< -> $@"
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(OBJ_DIR)/francode.lex.o: francode.lex.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compilation de $< -> $@"
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# --- RÈGLES DE GÉNÉRATION ---

# Règle pour générer le parser avec Bison
francode.tab.c francode.tab.h: francode.y
	@echo "Génération du parser avec Bison..."
	$(BISON) $< -o francode.tab.c

# Règle pour générer le lexer avec Flex
francode.lex.c: francode.l francode.tab.h
	@echo "Génération du lexer avec Flex..."
	$(LEX) -o $@ $<

# --- RÈGLES UTILITAIRES ---

# Règle pour nettoyer le répertoire
clean:
	@echo "Nettoyage du projet..."
	rm -rf $(OBJ_DIR) $(COMPILATEUR) francode.tab.* francode.lex.c output* programme_final

# Cible de test
test: $(COMPILATEUR)
	./$(COMPILATEUR) test.franco
	gcc output.c -o programme_final
	@echo "--- Exécution de 'programme_final' ---"
	./programme_final

# Déclare les cibles qui ne sont pas des fichiers
.PHONY: all clean test