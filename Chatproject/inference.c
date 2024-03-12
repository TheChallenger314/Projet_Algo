#include "expert_system.h"
#include <stdio.h>
#include <string.h>

extern Rule rules[MAX_RULES];
extern int rules_count;
extern Fact facts[MAX_FACTS];
extern int facts_count;

int isFactTrue(char fact) {
    for (int i = 0; i < facts_count; i++) {
        if (facts[i].fact == fact) {
            return facts[i].state;
        }
    }
    return 0; // Fact not found or false
}

void forwardChaining() {
    // Implémentez la logique de chaînage avant ici
  int changes;
    do {
        changes = 0;
        for (int i = 0; i < rules_count; i++) {
            int valid = 1;
            for (int j = 0; j < rules[i].conditions_count; j++) {
                if (!isFactTrue(rules[i].conditions[j])) {
                    valid = 0;
                    break;
                }
            }
            if (valid) {
                for (int k = 0; k < facts_count; k++) {
                    if (facts[k].fact == rules[i].conclusion && facts[k].state == 0) {
                        facts[k].state = 1;
                        changes = 1;
                        break;
                    }
                }
            }
        }
    } while (changes);
}

void backwardChaining(char goal) {
    // Implémentez la logique de chaînage arrière ici
  if (isFactTrue(goal)) {
        printf("%c is already known to be true.\n", goal);
        return;
    }

    for (int i = 0; i < rules_count; i++) {
        if (rules[i].conclusion == goal) {
            int valid = 1;
            for (int j = 0; j < rules[i].conditions_count; j++) {
                if (!isFactTrue(rules[i].conditions[j])) {
                    backwardChaining(rules[i].conditions[j]);
                    if (!isFactTrue(rules[i].conditions[j])) {
                        valid = 0;
                        break;
                    }
                }
            }
            if (valid) {
                printf("%c is concluded to be true based on rule %s -> %c.\n", goal, rules[i].conditions, goal);
                return;
            }
        }
    }

    printf("%c cannot be concluded to be true based on the given rules and facts.\n", goal);
}
