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

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        Rule *current_rule = &rules[rules_count++];
        current_rule->conditions_count = 0;

        // Tokeniser la ligne pour lire les conditions
        // Chaque condition est séparée par un espace et fait partie de l'opération logique ET
        char *token = strtok(line, " ,->\n");
        while (token && *token != '>' && current_rule->conditions_count < MAX_CONDITIONS) {
            // Chaque condition est stockée individuellement
            current_rule->conditions[current_rule->conditions_count++] = token[0];
            token = strtok(NULL, " ,->\n");
        }

        // Lire la conclusion de la règle
        token = strtok(NULL, " ,->\n");
        if (token) {
            current_rule->conclusion = *token;
        }
    }

    fclose(file);
}
