#include "expert_system.h"

// Fonction de chaînage arrière qui vérifie si un objectif peut être atteint à partir des faits et règles existants
int chainageArriere(char* objectif, Fait* faits, Regle* regles, Fait* faitsVerifies) {
    // Vérifie si l'objectif est déjà un fait
    Fait* faitCourant = faits;
    while (faitCourant != NULL) {
        if (strstr(objectif, faitCourant->description) != NULL) {
            printf("Objectif: %s est vrai\n", objectif);
            return 1;
        }
        faitCourant = faitCourant->suivant;
    }

 // Évite les vérifications répétées pour prévenir les boucles infinies
    Fait* faitVerifie = faitsVerifies;
    while (faitVerifie != NULL) {
        if (strstr(objectif, faitVerifie->description) != NULL) {
            printf("Objectif: %s a déjà été vérifié, évitant les boucles infinies.\n", objectif);
            return 0; // Objectif déjà vérifié
        }
        faitVerifie = faitVerifie->suivant;
    }

    ajouterFait(&faitsVerifies, objectif);

    Regle* regleCourante = regles;
    while (regleCourante != NULL) {
        if (strstr(regleCourante->conclusion, objectif) != NULL) {
            printf("Objectif: %s peut être prouvé par la règle: %s -> %s\n", objectif, regleCourante->condition->condition, regleCourante->conclusion);

            char* jeton = strtok(regleCourante->condition->condition, " ");
            bool conditionsProuvees = true;
            while (jeton != NULL) {
                if (!chainageArriere(jeton, faits, regles, faitsVerifies)) {
                    conditionsProuvees = false;
                    printf("La condition %s dans la règle %s -> %s ne peut pas être prouvée.\n", jeton, regleCourante->condition->condition, regleCourante->conclusion);
                    break;
                }
                jeton = strtok(NULL, " ");
            }

            if (conditionsProuvees) {
                printf("Conditions prouvées pour la règle: %s -> %s\n", regleCourante->condition->condition, regleCourante->conclusion);
                printf("Objectif: %s est prouvé\n", objectif);
                return 1; // Toutes les conditions sont prouvées
            }
        }
        regleCourante = regleCourante->suivant;
    }

    printf("Objectif: %s ne peut pas être prouvé\n", objectif);
    return 0; // Objectif ne peut pas être prouvé
}
