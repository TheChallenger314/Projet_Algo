#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "fonction.c"


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


 
