<<<<<<< HEAD
# Système Expert

Ce système expert est une application conçue pour démontrer les principes du chaînage avant et arrière dans un environnement de règles définies.

## Fonctionnalités

- **Chargement des règles**: Les règles sont chargées à partir d'un fichier pour initialiser le système.
- **Ajout de faits**: L'utilisateur peut ajouter des faits dynamiquement.
- **Chaînage avant**: Implémente le chaînage avant pour déduire de nouveaux faits à partir des règles existantes pour la première itération. Les autres itérations possible peuvent déduire de nouveaux faits sans avoir forcément tous les prémices d'une règles qui sont vrai.
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


## Architecture du programme 

// Affiche le menu principal du programme:  
void afficherMenu();  
.  
.  
// Charge les règles depuis un fichier:  
void chargerReglesDepuisFichier(Regle** regles);  
.  
// Affiche la liste des règles:  
void afficherRegles(Regle* regles);  
.  
.  
// Gère la saisie de l'utilisateur et les actions correspondantes:  
void gererSaisieUtilisateur(Fait** faits, Regle** regles);  
.  
// Fonction pour ajouter un fait à la liste de faits:  
void ajouterFait(Fait** tete, char* description);  
.  
// Fonction pour ajouter une règle à la liste de règles:  
void ajouterRegle(Regle** tete, char* condition, char* conclusion);  
.  
.  
// Vérifie si un fait existe déjà dans la liste de faits:  
bool faitExisteDansFaits(const char* descriptionFait, Fait* faits);  
.  
// Implémente le chaînage avant pour inférer de nouveaux faits à partir des règles et faits existants:  
void chainageAvant(Fait** faits, Regle* regles);  
.  
// Fonction de chaînage arrière qui vérifie si un objectif peut être atteint à partir des faits et règles existants:  
int chainageArriere(char* objectif, Fait* faits, Regle* regles, Fait* faitsVerifies);  
.  
.  
// Sauvegarde les règles actuelles dans un fichier:  
void sauvegarderReglesDansFichier(Regle* regles);  
.  
// Libère la mémoire allouée pour les faits:  
void libererFaits(Fait* tete);  
.  
// Libère la mémoire allouée pour les règles:  
void libererRegles(Regle* tete);  


## Compilation

Utilisez le fichier `Makefile` fourni pour compiler le projet : make


## Exécution

Après la compilation, exécutez le système expert avec : ./expert_system
=======
# Projet_Algo

>>>>>>> 078aa8043870b4bca1d09f516928932cf61869ff
## Protocol

<pre>Pour rentrer une nouvelle règle suivre cet exemple : fait1 fait2 -> conclusion1; [Entrée]</pre>  
<pre>                                                     fait3 -> conclusion2; [Entrée]</pre>  

<pre>Pour rentrer un nouveau fait suivre cet exemple : fait1; [Entrée]</pre> 
<pre>                                                  fait2; [Entrée]</pre>
<pre>                                                  fait3; [Entrée]</pre>

