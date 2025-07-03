# Compilateur Bamoun-C 🇨🇲

Un compilateur innovant qui traduit des programmes écrits en langue Bamoun (langue culturelle camerounaise) vers le langage C.

## 📋 Table des Matières

- [Description](#description)
- [Fonctionnalités](#fonctionnalités)
- [Installation](#installation)
- [Utilisation](#utilisation)
- [Syntaxe Bamoun](#syntaxe-bamoun)
- [Exemples](#exemples)
- [Architecture](#architecture)
- [Tests](#tests)
- [Contribution](#contribution)
- [Auteurs](#auteurs)

## 📖 Description

Ce projet implémente un compilateur permettant d'écrire des programmes informatiques en langue Bamoun, une langue parlée dans la région de l'Ouest du Cameroun. Le compilateur traduit automatiquement le code Bamoun vers du code C standard, démocratisant ainsi la programmation dans les langues locales africaines.

### Objectifs du Projet

- ✅ Développer un compilateur Bamoun vers C
- ✅ Démontrer la faisabilité de la programmation en langues naturelles
- ✅ Préserver et valoriser la culture linguistique camerounaise
- ✅ Faciliter l'apprentissage de la programmation dans les langues locales

## 🚀 Fonctionnalités

- **Types de données** : Entiers (tn), Réels (pn), Chaînes (kmn)
- **Structures de contrôle** : Conditions, boucles tant que, boucles pour
- **Entrées/Sorties** : Lecture (f) et écriture (nzi/nzu)
- **Opérations arithmétiques** : +, -, *, /, %
- **Opérations logiques** : ET (&&), OU (||), NON (!)
- **Comparaisons** : ==, !=, <, >, <=, >=

## 🛠️ Installation

### Prérequis

```bash
# Ubuntu/Debian
sudo apt-get install flex bison gcc

# CentOS/RHEL
sudo yum install flex bison gcc

# macOS (avec Homebrew)
brew install flex bison gcc
```

### Compilation du Compilateur

```bash
# Cloner le repository
git clone https://github.com/votre-username/compilateur-bamoun-c.git
cd compilateur-bamoun-c

# Générer l'analyseur lexical
flex lexer_bamoun.l

# Générer l'analyseur syntaxique
bison -d parser.y

# Compiler le compilateur
gcc -o bamoun-compiler lex.yy.c parser.tab.c -lfl
```

## 📚 Utilisation

### Compilation d'un Programme Bamoun

```bash
# Compiler un fichier .bamoun vers C
./bamoun-compiler programme.bamoun

# Le fichier output.c est généré automatiquement
gcc -o programme output.c

# Exécuter le programme
./programme
```

### Exemple d'Utilisation

```bash
# Créer un fichier hello.bamoun
echo 'zn n dz
nzu "Mbakɔ Bamoun!"
m zn' > hello.bamoun

# Compiler
./bamoun-compiler hello.bamoun
gcc -o hello output.c
./hello
```

## 🗣️ Syntaxe Bamoun

### Mots-clés de Base

| Bamoun | Français | Description |
|--------|----------|-------------|
| `zn n dz` | programme | Début de programme |
| `m zn` | finprogramme | Fin de programme |
| `yn` | si | Condition si |
| `yn m` | sinon | Alternative sinon |
| `m yn` | finsi | Fin de condition |
| `lf m y` | tant que | Boucle tant que |
| `d` | faire | Action faire |
| `m lf` | fintantque | Fin tant que |

### Types de Données

| Bamoun | Type C | Description |
|--------|--------|-------------|
| `tn` | `int` | Nombre entier |
| `pn` | `float` | Nombre réel |
| `kmn` | `char*` | Chaîne de caractères |

### Entrées/Sorties

| Bamoun | Description |
|--------|-------------|
| `f` | Lire une valeur |
| `nzi` | Écrire une valeur |
| `nzu` | Afficher à l'écran |

### Valeurs Booléennes

| Bamoun | Valeur |
|--------|--------|
| `nkùm` | vrai (1) |
| `nket` | faux (0) |

## 💡 Exemples

### Programme Simple

**Bamoun :**
```bamoun
zn n dz
    tn x
    x = 10
    nzu "La valeur de x est : "
    nzu x
m zn
```

**Code C généré :**
```c
int main() {
    int x;
    x = 10;
    printf("La valeur de x est : ");
    printf("%d", x);
    return 0;
}
```

### Structure Conditionnelle

**Bamoun :**
```bamoun
zn n dz
    tn age
    f age
    yn age >= 18 yn
        nzu "Vous êtes majeur"
    yn m
        nzu "Vous êtes mineur"
    m yn
m zn
```

### Boucle Tant Que

**Bamoun :**
```bamoun
zn n dz
    tn i
    i = 1
    lf m y i <= 5 d
        nzu i
        i = i + 1
    m lf
m zn
```

## 🏗️ Architecture

Le compilateur suit une architecture classique en trois phases :

```
Code Bamoun → Analyseur Lexical → Analyseur Syntaxique → Générateur de Code → Code C→ Compilateur c→ executable
```


## 🧪 Tests

### Exécuter les Tests

verifier le make file dans le projet

### Cas de Test Couverts

- ✅ Déclarations de variables
- ✅ Structures de contrôle
- ✅ Opérations arithmétiques
- ✅ Opérations logiques
- ✅ Entrées/sorties
- ✅ Gestion d'erreurs
- ✅ tableau et matrice

## 🐛 Gestion des Erreurs

Le compilateur gère plusieurs types d'erreurs :

- **Erreurs lexicales** : Caractères non reconnus
- **Erreurs syntaxiques** : Structures grammaticales incorrectes
- **Erreurs sémantiques** : Variables non déclarées, types incompatibles
- **Erreurs de fichiers** : Problèmes d'ouverture/écriture

## 🤝 Contribution

Les contributions sont les bienvenues ! Voici comment contribuer :

1. Fork le projet
2. Créez une branche feature (`git checkout -b feature/AmazingFeature`)
3. Committez vos changements (`git commit -m 'Add AmazingFeature'`)
4. Push vers la branche (`git push origin feature/AmazingFeature`)
5. Ouvrez une Pull Request

### Domaines de Contribution

- 🌟 Extension du vocabulaire Bamoun
- 🔧 Amélioration des performances
- 📚 Documentation et exemples
- 🧪 Tests supplémentaires
- 🎨 Interface utilisateur graphique

## 📝 Licence

Ce projet est sous licence MIT. Voir le fichier `LICENSE` pour plus de détails.

## 👥 Auteurs

- **NDONKOU FRANK** - 
- **TCHIAZE ROMERO** - 
- **FOTSING ENGOULOU SIMON GAETAN** -   
- **NANA ORNELLA** - 
- **MOREL** -

## 🙏 Remerciements

- La communauté Bamoun pour la préservation de leur langue
- Les linguistes ayant documenté la langue Bamoun
- L'Université de Yaoundé pour le support académique
- Tous les contributeurs et testeurs


---

**"Ngkùm mbam Bamoun ndàp computer!"** 
*(Faire que la langue Bamoun rencontre l'informatique !)*
