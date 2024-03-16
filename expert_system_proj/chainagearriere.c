#include "expert_system.h"

int backwardChaining(char* goal, Fact* facts, Rule* rules) {
    Fact* currentFact = facts;
    while (currentFact != NULL) {
        if (strstr(currentFact->description, goal) != NULL) {
            printf("Goal: %s est vrai\n", goal);
            return 1; // Le goal est déjà un fait présent, donc il est vrai
        }
        currentFact = currentFact->next;
    }

    // Parcourir les règles pour voir si le goal peut être prouvé
    Rule* currentRule = rules;
    while (currentRule != NULL) {
        if (strstr(currentRule->conclusion, goal) != NULL) {
            printf("Goal: %s peut être prouvé par la règle: %s -> %s\n", goal, currentRule->condition, currentRule->conclusion);
            // Vérifier si les conditions de la règle peuvent être prouvées
            int conditionsProuvees = 1;
            char* token = strtok(currentRule->condition, " ");
            while (token != NULL) {
                if (!backwardChaining(token, facts, rules)) {
                    conditionsProuvees = 0;
                    break; // Si une condition ne peut pas être prouvée, arrêter la vérification des autres conditions
                }
                token = strtok(NULL, " ");
            }
            if (conditionsProuvees) {
                printf("Conditions prouvées pour la règle: %s -> %s\n", currentRule->condition, currentRule->conclusion);
                printf("Goal: %s est prouvé\n", goal); // Afficher que le goal est prouvé
                return 1; // Si toutes les conditions de la règle sont prouvées, la règle peut prouver le goal
            }
        }
        currentRule = currentRule->next;
    }

    printf("Goal: %s ne peut pas être prouvé\n", goal);
    return 0; // Si aucune règle ne peut prouver le goal
}
