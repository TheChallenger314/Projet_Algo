#include "expert_system.h"
#include <stdio.h>

int main() {
    char filename[] = "rules.txt";
    loadRules(filename);

    inputFacts();

    int choice;
    printf("Choose inference method:\n1. Forward chaining\n2. Backward chaining\n");
    scanf("%d", &choice);

    if (choice == 1) {
        forwardChaining();
    } else if (choice == 2) {
        char goal;
        printf("Enter your goal fact: ");
        scanf(" %c", &goal);
        backwardChaining(goal);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}
