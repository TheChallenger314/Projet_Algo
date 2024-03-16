#ifndef EXPERT_SYSTEM_H
#define EXPERT_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure de fait
typedef struct Fact {
    char description[100];
    struct Fact* next;
} Fact;

// Structure de règle
typedef struct Rule {
    char condition[100];
    char conclusion[100];
    struct Rule* next;
} Rule;

// Prototypes de fonctions
void addFact(Fact** head, char* description);
void addRule(Rule** head, char* condition, char* conclusion);
void forwardChaining(Fact* facts, Rule* rules);
int backwardChaining(char* goal, Fact* facts, Rule* rules);
void print_rules(Rule* rules);

#endif // EXPERT_SYSTEM_H
