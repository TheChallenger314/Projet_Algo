#include "expert_system.h"
#include <stdio.h>

Fact facts[MAX_FACTS];
int facts_count = 0;

void inputFacts() {
    char fact;
    int state;
    printf("Enter your facts (e.g., a 1 for fact 'a' is true, a 0 for fact 'a' is false):\n");
    // Implémentez la logique de saisie des faits ici
    while (1) {
        scanf(" %c", &fact);
        if (fact == 'q') break;
        scanf("%d", &state);

        facts[facts_count].fact = fact;
        facts[facts_count].state = state;
        facts_count++;
    }
    // Mettez à jour facts et facts_count
}
