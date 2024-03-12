#include "expert_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Rule rules[MAX_RULES];
int rules_count = 0;

void loadRules(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening rules file");
        exit(1);
    }

    // Implémentez la logique de lecture des règles ici
    while (fscanf(file, "%s -> %c;", rules[rules_count].conditions, &rules[rules_count].conclusion) != EOF) {
        rules[rules_count].conditions_count = strlen(rules[rules_count].conditions);
        rules_count++;
    }
    // Mettez à jour rules et rules_count
    fclose(file);
}
