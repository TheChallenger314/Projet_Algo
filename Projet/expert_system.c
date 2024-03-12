#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FACTS 100
#define MAX_RULES 100
#define MAX_CONDITIONS 10

typedef struct {
    char fact;
    int state; // 0 for false, 1 for true
} Fact;

typedef struct {
    char conditions[MAX_CONDITIONS]; // Conditions to meet (facts)
    char result; // Resulting fact if conditions are met
    int conditions_count;
} Rule;

Fact facts[MAX_FACTS];
Rule rules[MAX_RULES];
int facts_count = 0;
int rules_count = 0;

void initializeFacts() {
    // Example: Initialize with two facts
    facts[0].fact = 'b';
    facts[0].state = 1;
    facts[1].fact = 'c';
    facts[1].state = 1;
    facts_count = 2;
}

void loadFactsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    while (fscanf(file, "%c %d\n", &facts[facts_count].fact, &facts[facts_count].state) != EOF) {
        facts_count++;
    }

    fclose(file);
}

void initializeRules() {
    // Example: Initialize with one rule
    // Rule: if b and c then d (b c -> d)
    rules[0].conditions[0] = 'b';
    rules[0].conditions[1] = 'c';
    rules[0].result = 'd';
    rules[0].conditions_count = 2;
    rules_count = 1;
}

int isFactTrue(char fact) {
    for (int i = 0; i < facts_count; i++) {
        if (facts[i].fact == fact && facts[i].state == 1) {
            return 1;
        }
    }
    return 0;
}

void loadRulesFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    while (fscanf(file, "%c %c %c %d\n", &rules[rules_count].conditions[0], &rules[rules_count].conditions[1], &rules[rules_count].result, &rules[rules_count].conditions_count) != EOF) {
        rules_count++;
    }

    fclose(file);
}

void applyRule(Rule rule) {
    int conditions_met = 1;
    for (int i = 0; i < rule.conditions_count; i++) {
        if (!isFactTrue(rule.conditions[i])) {
            conditions_met = 0;
            break;
        }
    }

    if (conditions_met) {
        for (int i = 0; i < facts_count; i++) {
            if (facts[i].fact == rule.result) {
                facts[i].state = 1;
                return;
            }
        }

        // Add new fact if it doesn't exist
        facts[facts_count].fact = rule.result;
        facts[facts_count].state = 1;
        facts_count++;
    }
}

void forwardChaining() {
    int applied = 1;
    while (applied) {
        applied = 0;
        for (int i = 0; i < rules_count; i++) {
            if (!isFactTrue(rules[i].result)) {
                applyRule(rules[i]);
                applied = 1;
            }
        }
    }
}

int main() {
    initializeFacts();
    initializeRules();

    loadFactsFromFile("facts.txt");
    loadRulesFromFile("regle.txt");
    forwardChaining();

    printf("Facts after forward chaining:\n");
    for (int i = 0; i < facts_count; i++) {
        if (facts[i].state == 1) {
            printf("Fact %c is true\n", facts[i].fact);
        }
        else 
        {
            printf("Fact %c is false\n", facts[i].fact);
        }
    }

    return 0;
}
