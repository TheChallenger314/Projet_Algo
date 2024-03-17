#ifndef EXPERT_SYSTEM_H
#define EXPERT_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Fact {
    char description[100];
    struct Fact* next;
} Fact;

typedef struct Condition {
    char condition[100];
    struct Condition* suiv;
} Condition;

typedef struct Rule {
    Condition* condition;
    char conclusion[100];
    struct Rule* next;
} Rule;

// Prototypes de fonctions
void addFact(Fact** head, char* description);
void addRule(Rule** head, char* condition, char* conclusion);
void forwardChaining(Fact** facts, Rule* rules);
int backwardChaining(char* goal, Fact* facts, Rule* rules);
void print_rules(Rule* rules);
bool fact_exists_in_facts(const char* fact_description, Fact* facts);
void loadRulesFromFile(Rule** rules);
void freeFacts(Fact* head);
void freeRules(Rule* head);
void displayMenu();
void handleUserInput(Fact** facts, Rule** rules);

#endif // EXPERT_SYSTEM_H
