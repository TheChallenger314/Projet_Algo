# Système Expert

Ce système expert est une application conçue pour démontrer les principes du chaînage avant et arrière dans un environnement de règles définies.

## Fonctionnalités

- **Chargement des règles**: Les règles sont chargées à partir d'un fichier pour initialiser le système.
- **Ajout de faits**: L'utilisateur peut ajouter des faits dynamiquement.
- **Chaînage avant**: Implémente le chaînage avant pour déduire de nouveaux faits à partir des règles existantes.
- **Chaînage arrière**: Réalise le chaînage arrière pour vérifier la validité d'un fait donné.
- **Gestion de l'interface utilisateur**: Propose un menu interactif pour naviguer entre les différentes fonctionnalités.

## Utilisation

1. **Menu Principal**: À l'exécution, le système affiche un menu principal offrant plusieurs options.
2. **Ajouter de nouveaux faits**: L'utilisateur peut ajouter des faits qui seront pris en compte dans les chaînages.
3. **Effectuer un chaînage avant**: Démarre le processus de chaînage avant, affichant les résultats en temps réel.
4. **Effectuer un chaînage arrière**: Permet à l'utilisateur de tester la validité d'un fait spécifique.
5. **Ajouter de nouvelles règles**: Offre la possibilité d'ajouter de nouvelles règles à la base de connaissances.

## Description des fichiers

- **main.c**: Point d'entrée du programme. Initialise les structures et démarre l'interaction avec l'utilisateur.
- **expert_system.h**: Définit les structures et prototypes des fonctions utilisées dans l'application.
- **chainageavant.c**: Contient l'algorithme du chaînage avant.
- **chainagearriere.c**: Contient l'algorithme du chaînage arrière.
- **affichage.c**: Gère l'affichage du menu et l'interaction avec l'utilisateur.
- **menu.c**: Contient les fonctions pour l'ajout de faits et de règles, et le chargement des règles depuis un fichier.

## Compilation

Utilisez le fichier `Makefile` fourni pour compiler le projet : make


## Exécution

Après la compilation, exécutez le système expert avec : ./expert_system
