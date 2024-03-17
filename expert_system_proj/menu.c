#include "expert_system.h"

void addFact(Fact** head, char* description) {
    Fact* newFact = (Fact*)malloc(sizeof(Fact));
    if (newFact == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    strcpy(newFact->description, description);
    newFact->next = *head;
    *head = newFact;
}

void addRule(Rule** head, char* condition, char* conclusion) {
    Rule* newRule = (Rule*)malloc(sizeof(Rule));
    if (newRule == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    newRule->condition = (Condition*)malloc(sizeof(Condition));
    if (newRule->condition == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    strcpy(newRule->condition->condition, condition);
    newRule->condition->suiv = NULL;
    strcpy(newRule->conclusion, conclusion);
    newRule->next = NULL;

    // Si la liste est vide, ajouter la nouvelle règle en tête.
    if (*head == NULL) {
        *head = newRule;
    } else {
        // Parcourir la liste jusqu'au dernier élément.
        Rule* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        // Ajouter la nouvelle règle à la fin.
        current->next = newRule;
    }
}

void print_rules(Rule* rules) {
    printf("Liste des règles :\n");
    Rule* current = rules;
    int count = 0;
    while (current != NULL) {
        count++;
        printf("n°%d) ", count);
        printf("%s -> %s\n", current->condition->condition, current->conclusion);
        current = current->next;
    }
}

bool fact_exists_in_facts(const char* fact_description, Fact* facts) {
    while (facts != NULL) {
        if (strcmp(facts->description, fact_description) == 0) {
            return true;
        }
        facts = facts->next;
    }
    return false;
}

void loadRulesFromFile(Rule** rules) {
    FILE* file = fopen("regles.kbs", "r");
    if (!file) {
        perror("Impossible d'ouvrir le fichier de règles");
        exit(EXIT_FAILURE);
    }

    char line[200];
    while (fgets(line, sizeof(line), file) != NULL) {
        char condition[100], conclusion[100];
        if (sscanf(line, "%99[^-]-> %99[^\n];", condition, conclusion) == 2) {
            addRule(rules, condition, conclusion);
        }
    }

    fclose(file);
}

void freeFacts(Fact* head) {
    Fact* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void freeRules(Rule* head) {
    Rule* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}
