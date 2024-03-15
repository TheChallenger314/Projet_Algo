#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure de fait
typedef struct Fact {
    char description[100];
    struct Fact* next;
} Fact;

// Structure de condition
typedef struct Condition {
    char condition[100];
    struct Condition *suiv;
} Condition;

// Structure de règle
typedef struct Rule {
    Condition* condition;
    char conclusion[100];
    struct Rule* next;
} Rule;
int backwardChaining(char* goal, Fact* facts, Rule* rules);
void addFact(Fact** head, char* description);
void addRule(Rule** head, Condition* condition, char* conclusion);
void forwardChaining(Fact* facts, Rule* rules);
void print_rules(Rule* rules);
FILE *ouvrir_fichier(char *filename);
Rule *libereRegle(Rule *rules);
Fact *libereFait(Fact *facts);
FILE *rulesFile = NULL;
Fact* facts = NULL;
FILE* factsFile = NULL;
Rule* rules = NULL;

// Fonction pour ajouter un fait à la liste chaînée
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

// Fonction pour ajouter une règle à la liste chaînée
void addRule(Rule** head, Condition* condition, char* conclusion) {
    Rule* newRule = (Rule*)malloc(sizeof(Rule));
    if (newRule == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    // Créer la liste chaînée des conditions
    Condition* currentCondition = condition;
    char conditionStr[100] = "";
    while (currentCondition != NULL) {
        strcat(conditionStr, currentCondition->condition);
        strcat(conditionStr, " ");
        currentCondition = currentCondition->suiv;
    }

    newRule->condition = condition;
    strcpy(newRule->conclusion, conclusion);
    newRule->next = *head;
    *head = newRule;
}

// Chaînage avant
void forwardChaining(Fact* facts, Rule* rules) {
    Fact* currentFact = facts;
    while (currentFact != NULL) {
        Rule* currentRule = rules;
        while (currentRule != NULL) {
            printf("Condition: %s est vraie, conclusion: %s\n", currentRule->condition->condition, currentRule->conclusion);
            if (strstr(currentFact->description, currentRule->condition->condition) != NULL) {
                Fact* newFact = facts;
                int found = 0;
                while (newFact != NULL) {
                    if (strcmp(newFact->description, currentRule->conclusion) == 0) {
                        found = 1;
                        break;
                    }
                    newFact = newFact->next;
                }
                if (!found) {
                    addFact(&facts, currentRule->conclusion);
                }
            }
            currentRule = currentRule->next;
        }
        currentFact = currentFact->next;
    }
}

// Chaînage arrière
int backwardChaining(char* goal, Fact* facts, Rule* rules) {
    Fact* currentFact = facts;
    while (currentFact != NULL) {
        if (strstr(currentFact->description, goal) != NULL) {
            printf("Goal: %s est vrai\n", goal);
            return 1;
        }
        currentFact = currentFact->next;
    }

    Rule* currentRule = rules;
    while (currentRule != NULL) {
        if (strstr(currentRule->conclusion, goal) != NULL) {
            printf("Goal: %s peut être prouvé par la règle: %s -> %s\n", goal, currentRule->condition->condition, currentRule->conclusion);
            int conditionsProuvees = 1;
            char* token = strtok(currentRule->condition->condition, " ");
            while (token != NULL) {
                if (!backwardChaining(token, facts, rules)) {
                    conditionsProuvees = 0;
                    break;
                }
                token = strtok(NULL, " ");
            }
            if (conditionsProuvees) {
                printf("Goal: %s est prouvé\n", goal);
                printf("Conditions prouvées pour la règle: %s -> %s\n", currentRule->condition->condition, currentRule->conclusion);
                return 1;
            }
        }
        currentRule = currentRule->next;
    }

    printf("Goal: %s ne peut pas être prouvé\n", goal);
    return 0;
}

// Fonction pour afficher les règles
void print_rules(Rule* rules) {
    printf("Liste des règles :\n");
    Rule* current = rules;
    int count = 0;
    while (current != NULL) {
        count++;
        printf("Rule %d:\n", count);
        printf("Conditions:\n");
        Condition* currentCondition = current->condition;
        while (currentCondition != NULL) {
            printf("%s\n", currentCondition->condition);
            currentCondition = currentCondition->suiv;
        }
        printf("Conclusion: %s\n", current->conclusion);
        current = current->next;
    }
}

// Fonction pour ouvrir un fichier
FILE *ouvrir_fichier(char *filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Impossible d'ouvrir le fichier");
        exit(EXIT_FAILURE);
    }
    return file;
}

// Fonction pour libérer la mémoire allouée pour les règles
Rule *libereRegle(Rule *rules) {
    Rule* currentRule = rules;
    while (currentRule != NULL) {
        Rule* temp = currentRule;
        currentRule = currentRule->next;
        free(temp);
    }
    return NULL;
}

// Fonction pour libérer la mémoire allouée pour les faits
Fact *libereFait(Fact *facts) {
    Fact* currentFact = facts;
    while (currentFact != NULL) {
        Fact* temp = currentFact;
        currentFact = currentFact->next;
        free(temp);
    }
    return NULL;
}

// Fonction principale
int main() {
    // Ouverture des fichiers
    factsFile = ouvrir_fichier("faits.kbs");
    rulesFile = ouvrir_fichier("regles.kbs");

    // Lecture des faits et des règles depuis les fichiers
    facts = lireFait(factsFile);
    rules = lireRegle(rulesFile);

    // Menu
    int choix;
    do {
        choix = menu();
    } while (choix != 0);

    // Libération de la mémoire allouée pour les faits et les règles
    facts = libereFait(facts);
    rules = libereRegle(rules);

    return 0;
}
