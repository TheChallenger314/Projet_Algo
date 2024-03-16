#include "expert_system.h"

int main() {
    Fact* facts = NULL;
    Rule* rules = NULL;

    loadRulesFromFile(&rules);
    // Initialiser les faits et les règles ici si nécessaire
    handleUserInput(&facts, &rules);

    // Libérer les ressources allouées
    freeFacts(facts);
    freeRules(rules);

    return 0;
}
