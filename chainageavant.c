#include "expert_system.h"

// Implémente le chaînage avant pour inférer de nouveaux faits à partir des règles et faits existants
void chainageAvant(Fait** faits, Regle* regles) {
    bool nouveauFaitAjoute = true;
    int iteration = 1;

    printf("Début du chaînage avant :\n");

// Continue tant que de nouveaux faits sont ajoutés à chaque itération
    while (nouveauFaitAjoute) {
        nouveauFaitAjoute = false;
        printf("Itération %d :\n", iteration++);

        Regle* regleCourante = regles;
        bool faitsAjoutesDansIteration = false;
        while (regleCourante != NULL) {
            bool toutesConditionsSatisfaites = true;
            char* jeton = strtok(regleCourante->condition->condition, " ");
            while (jeton != NULL) {
                if (!faitExisteDansFaits(jeton, *faits)) {
                    toutesConditionsSatisfaites = false;
                    break;
                }
                jeton = strtok(NULL, " ");
            }

            if (toutesConditionsSatisfaites) {
                if (!faitExisteDansFaits(regleCourante->conclusion, *faits)) {
                    ajouterFait(faits, regleCourante->conclusion);
                    printf("Nouveau fait ajouté : %s\n", regleCourante->conclusion);
                    nouveauFaitAjoute = true;
                    faitsAjoutesDansIteration = true;
                }
            }
            regleCourante = regleCourante->suivant;
        }

        if (!faitsAjoutesDansIteration) {
            break;
        }
    }

    printf("Fin du chaînage avant.\n");
}
