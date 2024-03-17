#ifndef EXPERT_SYSTEM_H
#define EXPERT_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "GfxLib.h"
#include "ESLib.h"
#define LargeurFenetre 800
#define HauteurFenetre 600

// Définition d'une structure pour les faits
typedef struct Fait {
    char description[100];
    struct Fait* suivant;
} Fait;

// Définition d'une structure pour les conditions des règles
typedef struct Condition {
    char condition[100];
    struct Condition* suivant;
} Condition;


// Définition d'une structure pour les règles
typedef struct Regle {
    Condition* condition;
    char conclusion[100];
    struct Regle* suivant;
} Regle;

// Prototypes de fonctions
void ajouterFait(Fait** tete, char* description);
void ajouterRegle(Regle** tete, char* condition, char* conclusion);
void chainageAvant(Fait** faits, Regle* regles);
int chainageArriere(char* objectif, Fait* faits, Regle* regles, Fait* faitsVerifies);
void afficherRegles(Regle* regles);
bool faitExisteDansFaits(const char* descriptionFait, Fait* faits);
void chargerReglesDepuisFichier(Regle** regles);
void libererFaits(Fait* tete);
void libererRegles(Regle* tete);
void sauvegarderReglesDansFichier(Regle* regles);
void afficherMenu();
void gererSaisieUtilisateur(Fait** faits, Regle** regles);

#endif // EXPERT_SYSTEM_H
