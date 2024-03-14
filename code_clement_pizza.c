#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Fact {
    char name[100];
    struct Fact *next;
} Fact;

typedef struct Rule {
    char hypothesis[10][100];
    int hypothesisCount;
    char conclusion[100];
    struct Rule *next;
} Rule;

typedef struct KnowledgeBase {
    Fact *facts;
    Rule *rules;
} KnowledgeBase;

// Prototype de la fonction pour éviter l'avertissement d'appel implicite
int factExists(KnowledgeBase *kb, const char *name);

void addFact(KnowledgeBase *kb, const char *name) {
    if (!factExists(kb, name)) {
        Fact *newFact = (Fact *)malloc(sizeof(Fact));
        strcpy(newFact->name, name);
        newFact->next = kb->facts;
        kb->facts = newFact;
    }
}

int factExists(KnowledgeBase *kb, const char *name) {
    Fact *current = kb->facts;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void addRule(KnowledgeBase *kb, char hypothesis[][100], int hypothesisCount, const char *conclusion) {
    Rule *newRule = (Rule *)malloc(sizeof(Rule));
    for (int i = 0; i < hypothesisCount; ++i) {
        strcpy(newRule->hypothesis[i], hypothesis[i]);
    }
    newRule->hypothesisCount = hypothesisCount;
    strcpy(newRule->conclusion, conclusion);
    newRule->next = kb->rules;
    kb->rules = newRule;
}

void loadKnowledgeBase(const char *filename, KnowledgeBase *kb) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *token, *context;
        char tempHypothesis[10][100];
        int count = 0;
        char *conclusion = NULL;

        if (strstr(line, "->")) { // It's a rule
            token = strtok_r(line, "->\n", &context);
            while (token != NULL && count < 10) {
                if (conclusion == NULL) {
                    char *innerToken = strtok(token, " ");
                    while (innerToken != NULL) {
                        strcpy(tempHypothesis[count++], innerToken);
                        innerToken = strtok(NULL, " ");
                    }
                    conclusion = strtok_r(NULL, "->\n", &context);
                }
                token = strtok_r(NULL, "->\n", &context);
            }
            addRule(kb, tempHypothesis, count, conclusion);
        } else { // It's a fact
            token = strtok(line, "\n");
            addFact(kb, token);
        }
    }
    fclose(file);
}


void forwardChaining(KnowledgeBase *kb) {
    bool newFactAdded;
    do {
        newFactAdded = false;
        Rule *currentRule = kb->rules;
        while (currentRule != NULL) {
            bool ruleApplies = true;
            for (int i = 0; i < currentRule->hypothesisCount; i++) {
                if (!factExists(kb, currentRule->hypothesis[i])) {
                    ruleApplies = false;
                    break;
                }
            }
            if (ruleApplies && !factExists(kb, currentRule->conclusion)) {
                addFact(kb, currentRule->conclusion);
                newFactAdded = true;
                printf("Nouvelle couleur déduite : %s\n", currentRule->conclusion);
            }
            currentRule = currentRule->next;
        }
    } while (newFactAdded);
}


int backwardChaining(KnowledgeBase *kb, const char *goal) {
    if (factExists(kb, goal)) {
        printf("%s est déjà une couleur connue.\n", goal);
        return 1;
    }

    Rule *rule = kb->rules;
    while (rule) {
        if (strcmp(rule->conclusion, goal) == 0) {
            int hypothesesVerified = 1;
            for (int i = 0; i < rule->hypothesisCount; i++) {
                if (!backwardChaining(kb, rule->hypothesis[i])) {
                    hypothesesVerified = 0;
                    break;
                }
            }
            if (hypothesesVerified) {
                printf("%s peut être dérivé des règles.\n", goal);
                return 1;
            }
        }
        rule = rule->next;
    }

    return 0;
}

int main() {
    KnowledgeBase kb = {NULL, NULL};
    loadKnowledgeBase("couleurs.kbs", &kb);

    int choix;
    char couleur[100];
    printf("\nMenu:\n");
    printf("1. Chaînage avant\n");
    printf("2. Chaînage arrière\n");
    printf("3. Quitter\n");
    printf("Choisissez une option: ");
    scanf("%d", &choix);

    switch (choix) {
        case 1:
              printf("Entrez le nombre de couleurs de base que vous voulez ajouter: ");
            int nombre;
            scanf("%d", &nombre);
            for (int i = 0; i < nombre; i++) {
                printf("Entrez la couleur de base %d: ", i + 1);
                scanf("%s", couleur);
                addFact(&kb, couleur);
            }
            //printf("Exécution du chaînage avant...\n");
            forwardChaining(&kb);
            break;
        case 2:
            printf("Entrez la couleur cible pour le chaînage arrière: ");
            scanf("%s", couleur);
            if (!backwardChaining(&kb, couleur)) {
                printf("La couleur %s ne peut pas être déduite avec la base de connaissances actuelle.\n", couleur);
            }
            break;
        case 3:
            printf("Quitter.\n");
            break;
        default:
            printf("Option invalide, veuillez réessayer.\n");
    }

    // Libération de la mémoire allouée

    return 0;
}
