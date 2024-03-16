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

void addFact(Fact** head, char* description) {
    Fact* newFact = (Fact*)malloc(sizeof(Fact));
    if (newFact == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les faits.\n");
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

// Chaînage avant
void forwardChaining(Fact** facts, Rule* rules) {
    bool new_fact_added = true;
    int iteration = 1;

    printf("Début du chaînage avant :\n");

    while (new_fact_added) {
        new_fact_added = false;
        printf("Iteration %d :\n", iteration++);

        Rule* current_rule = rules;
        bool facts_added_in_iteration = false; // Indicateur pour vérifier si des faits ont été ajoutés dans cette itération
        while (current_rule != NULL) {
            // Vérifier si toutes les conditions de la règle sont satisfaites par les faits actuels
            bool all_conditions_satisfied = true;
            char* token = strtok(current_rule->condition->condition, " ");
            while (token != NULL) {
                if (!fact_exists_in_facts(token, *facts)) {
                    all_conditions_satisfied = false;
                    break;
                }
                token = strtok(NULL, " ");
            }

            // Si toutes les conditions sont satisfaites, ajouter la conclusion comme nouveau fait
            if (all_conditions_satisfied) {
                // Si la conclusion n'est pas déjà un fait, l'ajouter
                if (!fact_exists_in_facts(current_rule->conclusion, *facts)) {
                    addFact(facts, current_rule->conclusion);
                    printf("Nouveau fait ajouté : %s\n", current_rule->conclusion);
                    new_fact_added = true;
                    facts_added_in_iteration = true;
                }
            }
            current_rule = current_rule->next;
        }

        // Si aucun nouveau fait n'a été ajouté dans cette itération, cela signifie que le chaînage est terminé
        if (!facts_added_in_iteration) {
            break;
        }
    }

    printf("Fin du chaînage avant.\n");
}




// Chaînage arrière
int backwardChaining(char* goal, Fact* facts, Rule* rules, Fact* checkedFacts) {
    // Vérifier si le goal est déjà un fait dans les faits
    Fact* currentFact = facts;
    while (currentFact != NULL) {
        if (strstr(goal, currentFact->description) != NULL) {
            printf("Goal: %s est vrai\n", goal);
            return 1; // Le goal est déjà un fait présent, donc il est vrai
        }
        currentFact = currentFact->next;
    }

    // Vérifier si le goal a déjà été vérifié pour éviter les boucles infinies
    Fact* checkedFact = checkedFacts;
    while (checkedFact != NULL) {
        if (strstr(goal, checkedFact->description) != NULL) {
            printf("Goal: %s a déjà été vérifié, évitant les boucles infinies.\n", goal);
            return 0; // Éviter la vérification répétée pour éviter les boucles infinies
        }
        checkedFact = checkedFact->next;
    }

    // Ajouter le goal à la liste des faits vérifiés
    addFact(&checkedFacts, goal);

    // Parcourir les règles pour voir si le goal peut être prouvé
    Rule* currentRule = rules;
    while (currentRule != NULL) {
        if (strstr(currentRule->conclusion, goal) != NULL) {
            printf("Goal: %s peut être prouvé par la règle: %s -> %s\n", goal, currentRule->condition->condition, currentRule->conclusion);
            
            // Vérifier si les conditions de la règle peuvent être prouvées
            char* token = strtok(currentRule->condition->condition, " ");
            bool conditionsProuvees = true;
            while (token != NULL) {
                if (!backwardChaining(token, facts, rules, checkedFacts)) {
                    conditionsProuvees = false;
                    printf("La condition %s dans la règle %s -> %s ne peut pas être prouvée.\n", token, currentRule->condition->condition, currentRule->conclusion);
                    break; // Si une condition ne peut pas être prouvée, arrêter la vérification des autres conditions
                }
                token = strtok(NULL, " ");
            }

            if (conditionsProuvees) {
                printf("Conditions prouvées pour la règle: %s -> %s\n", currentRule->condition->condition, currentRule->conclusion);
                printf("Goal: %s est prouvé\n", goal); // Afficher que le goal est prouvé
                return 1; // Si toutes les conditions de la règle sont prouvées, la règle peut prouver le goal
            }
        }
        currentRule = currentRule->next;
    }

    printf("Goal: %s ne peut pas être prouvé\n", goal);
    return 0; // Si aucune règle ne peut prouver le goal
}






 
// Fonction pour afficher les règles
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

void freeFacts(Fact* head) {
    Fact* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp); // Libérer le nœud
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




void displayMenu() {
    printf("\nMenu Principal\n");
    printf("1. Afficher les règles\n");
    printf("2. Effectuer un chaînage avant\n");
    printf("3. Effectuer un chaînage arrière\n");
    printf("4. Ajouter de nouveaux faits\n");
    printf("5. Ajouter de nouvelles règles\n");
    printf("6. Quitter\n");
}

void handleUserInput(Fact** facts, Rule** rules) {
    int nbFaits;
    printf("Entrez le nombre de faits : ");
    scanf("%d", &nbFaits);
    while (getchar() != '\n'); // Nettoyer le buffer d'entrée après la lecture du nombre

    for (int i = 0; i < nbFaits; i++) {
        char fait[100];
        printf("Entrez le fait n°%d : ", i + 1);
        fgets(fait, sizeof(fait), stdin);
        fait[strcspn(fait, "\n")] = 0; // Supprimer le caractère de saut de ligne à la fin
        addFact(facts, fait);
    }
    int choice;
    char goal[100], condition[100], conclusion[100];

    while(1) {
        displayMenu();
        printf("Entrez votre choix : ");
        scanf("%d", &choice);
        // Nettoyer le buffer d'entrée
        while(getchar() != '\n');

        switch(choice) {
            case 1:
                print_rules(*rules);
                break;
            case 2:
                forwardChaining(facts, *rules);
                break;
            case 3:
                printf("Entrez le goal pour le chaînage arrière : ");
                fgets(goal, sizeof(goal), stdin);
                goal[strcspn(goal, "\n")] = 0; // Supprimer le saut de ligne
                backwardChaining(goal, *facts, *rules, NULL); // NULL car il n'y a pas encore de faits vérifiés
                break;
            case 4:
                printf("Entrez le fait à ajouter : ");
                if (fgets(goal, sizeof(goal), stdin) != NULL) { // Utilisation sécurisée de fgets
                    goal[strcspn(goal, "\n")] = 0; // Supprimer le saut de ligne
                    addFact(facts, goal); // Utilisation correcte des variables
                }
                break;
            case 5:
                printf("Entrez la condition de la nouvelle règle : ");
                fgets(condition, sizeof(condition), stdin);
                condition[strcspn(condition, "\n")] = 0; // Supprimer le saut de ligne
                printf("Entrez la conclusion de la nouvelle règle : ");
                fgets(conclusion, sizeof(conclusion), stdin);
                conclusion[strcspn(conclusion, "\n")] = 0; // Supprimer le saut de ligne
                addRule(rules, condition, conclusion);
                break;
            case 6:
                printf("Quitter.\n");
                return; // Sortir de la fonction et donc terminer le programme
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    }
}


int main() {
    Fact* facts = NULL;
    Rule* rules = NULL;

    loadRulesFromFile(&rules);
    // Initialiser les faits et les règles ici si nécessaire
    handleUserInput(&facts, &rules);

    // Libérer les ressources allouées
    freeFacts(facts);
    freeRules(rules);

    return 0;
}
