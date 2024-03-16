#include "expert_system.h"

void forwardChaining(Fact* facts, Rule* rules) {
    Fact* currentFact = facts;
    while (currentFact != NULL) {
        Rule* currentRule = rules;
        while (currentRule != NULL) {
            if (strstr(currentFact->description, currentRule->condition) != NULL) {
                printf("Condition: %s est vraie, conclusion: %s\n", currentRule->condition, currentRule->conclusion);
                // Ajouter la conclusion comme un nouveau fait si elle n'existe pas déjà
                Fact* newFact = facts;
                int found = 0;
                while (newFact != NULL) {
                    if (strcmp(newFact->description, currentRule->conclusion) == 0) {
                        found = 1;
                        break;
                    }
                    newFact = newFact->next;
                }
                if (!found) {
                    addFact(&facts, currentRule->conclusion);
                }
            }
            currentRule = currentRule->next;
        }
        currentFact = currentFact->next;
    }
}
