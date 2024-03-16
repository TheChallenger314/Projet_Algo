#include "expert_system.h"

void forwardChaining(Fact** facts, Rule* rules) {
    bool new_fact_added = true;
    int iteration = 1;

    printf("Début du chaînage avant :\n");

    while (new_fact_added) {
        new_fact_added = false;
        printf("Iteration %d :\n", iteration++);

        Rule* current_rule = rules;
        bool facts_added_in_iteration = false; // Indicateur pour vérifier si des faits ont été ajoutés dans cette itération
        while (current_rule != NULL) {
            // Vérifier si la condition de la règle est satisfaite par les faits actuels
            if (fact_exists_in_facts(current_rule->condition->condition, *facts)) {
                // Si la conclusion n'est pas déjà un fait, ajouter la conclusion comme nouveau fait
                if (!fact_exists_in_facts(current_rule->conclusion, *facts)) {
                    addFact(facts, current_rule->conclusion);
                    printf("Nouveau fait ajouté : %s\n", current_rule->conclusion);
                    new_fact_added = true;
                    facts_added_in_iteration = true;
                }
            }
            current_rule = current_rule->next;
        }

        // Si aucun nouveau fait n'a été ajouté dans cette itération, cela signifie que le chaînage est terminé
        if (!facts_added_in_iteration) {
            // Si aucune nouvelle information n'est ajoutée lors de la première itération,
            // évaluons à nouveau toutes les règles pour déduire d'autres faits initiaux
            if (iteration == 2) {
                current_rule = rules;
                while (current_rule != NULL) {
                    // Vérifiez si les conditions de cette règle sont satisfaites par les faits initiaux fournis
                    bool all_conditions_satisfied = true;
                    char* token = strtok(current_rule->condition->condition, " ");
                    while (token != NULL) {
                        if (!fact_exists_in_facts(token, *facts)) {
                            all_conditions_satisfied = false;
                            break;
                        }
                        token = strtok(NULL, " ");
                    }
                    // Si toutes les conditions sont satisfaites, ajoutez la conclusion de la règle comme nouveau fait
                    if (all_conditions_satisfied) {
                        addFact(facts, current_rule->conclusion);
                        printf("Nouveau fait ajouté : %s\n", current_rule->conclusion);
                        new_fact_added = true;
                    }
                    current_rule = current_rule->next;
                }
            } else {
                break;
            }
        }
    }

    printf("Fin du chaînage avant.\n");
}
