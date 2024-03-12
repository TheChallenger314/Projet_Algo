#ifndef EXPERT_SYSTEM_H
#define EXPERT_SYSTEM_H

#define MAX_RULES 100
#define MAX_FACTS 100
#define MAX_CONDITIONS 10

typedef struct {
    char conditions[MAX_CONDITIONS];
    char conclusion;
    int conditions_count;
} Rule;

typedef struct {
    char fact;
    int state; // 0 pour faux, 1 pour vrai
} Fact;

void loadRules(const char *filename);
void inputFacts();
void forwardChaining();
void backwardChaining(char goal);

#endif
