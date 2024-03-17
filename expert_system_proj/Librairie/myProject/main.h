#ifndef LOGIQUE_KNOWLEDGE_BASE_H_
#define LOGIQUE_KNOWLEDGE_BASE_H_

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
void addFact(Fact** head, char* description);
void addRule(Rule** head, char* condition, char* conclusion);
bool fact_exists_in_facts(const char* fact_description, Fact* facts);
void loadRulesFromFile(Rule** rules);
void forwardChaining(Fact** facts, Rule* rules);
int backwardChaining(char* goal, Fact* facts, Rule* rules);
void print_rules(Rule* rules);
void freeFacts(Fact* head);
void freeRules(Rule* head);
void displayMenu();
void handleUserInput(Fact** facts, Rule** rules);
#endif /* LOGIQUE_KNOWLEDGE_BASE_H_ */
