#include "expert_system.h"

void addFact(Fact** head, char* description) {
    Fact* newFact = (Fact*)malloc(sizeof(Fact));
    if (newFact == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newFact->description, description);
    newFact->next = *head;
    *head = newFact;
}

void addRule(Rule** head, char* condition, char* conclusion) {
    Rule* newRule = (Rule*)malloc(sizeof(Rule));
    if (newRule == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newRule->condition, condition);
    strcpy(newRule->conclusion, conclusion);
    newRule->next = *head;
    *head = newRule;
}

void print_rules(Rule* rules) {
    printf("Liste des règles :\n");
    Rule* current = rules;
    int count = 0;
    while (current != NULL) {
        count++;
        printf("n°%d) ", count);
        printf("%s -> %s\n", current->condition, current->conclusion);
        current = current->next;
    }
}
