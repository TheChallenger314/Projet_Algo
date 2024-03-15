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

        // Tokenize the line to read conditions
        char *token = strtok(line, " ,->\n");
        while (token && *token != '>' && current_rule->conditions_count < MAX_CONDITIONS) {
            current_rule->conditions[current_rule->conditions_count++] = token[0];
            token = strtok(NULL, " ,->\n");
        }

        // Read the conclusion
        token = strtok(NULL, " ,->\n");
        if (token) {
            current_rule->conclusion = *token;
        }
    }

    fclose(file);
}
