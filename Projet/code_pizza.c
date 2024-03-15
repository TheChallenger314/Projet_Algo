#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
// Structure de fait
typedef struct Fact {
    char description[100]; // Assumons une longueur maximale de 100 caractères pour la description du fait
    struct Fact* next;
} Fact;
 
// Structure de règle
typedef struct Rule {
    char condition[100]; // Assumons une longueur maximale de 100 caractères pour la condition de la règle
    char conclusion[100]; // Assumons une longueur maximale de 100 caractères pour la conclusion de la règle
    int num_hypotheses;
    struct Rule* next;
} Rule;
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
void addRule(Rule** head, char* condition, char* conclusion) {
    Rule* newRule = (Rule*)malloc(sizeof(Rule));
    if (newRule == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newRule->condition, condition);
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
            if (strstr(currentFact->description, currentRule->condition) != NULL) {
                printf("Condition: %s est vraie, conclusion: %s\n", currentRule->condition, currentRule->conclusion);
                // Ajouter la conclusion comme un nouveau fait si elle n'existe pas déjà
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
    // Vérifier si le goal est déjà un fait dans les faits
    Fact* currentFact = facts;
    while (currentFact != NULL) {
        if (strstr(currentFact->description, goal) != NULL) {
            printf("Goal: %s est vrai\n", goal);
            return 1; // Le goal est déjà un fait présent, donc il est vrai
        }
        currentFact = currentFact->next;
    }
 
    // Parcourir les règles pour voir si le goal peut être prouvé
    Rule* currentRule = rules;
    while (currentRule != NULL) {
        if (strstr(currentRule->conclusion, goal) != NULL) {
            printf("Goal: %s peut être prouvé par la règle: %s -> %s\n", goal, currentRule->condition, currentRule->conclusion);
            // Vérifier si les conditions de la règle peuvent être prouvées
            int conditionsProuvees = 1;
            char* token = strtok(currentRule->condition, " ");
            while (token != NULL) {
                if (!backwardChaining(token, facts, rules)) {
                    conditionsProuvees = 0;
                    break; // Si une condition ne peut pas être prouvée, arrêter la vérification des autres conditions
                }
                token = strtok(NULL, " ");
            }
            if (conditionsProuvees) {
                printf("Conditions prouvées pour la règle: %s -> %s\n", currentRule->condition, currentRule->conclusion);
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
        printf("%s -> %s\n", current->condition, current->conclusion);
        current = current->next;
    }
}
 FILE *ouvrir_fichier(char *filename)
 {
    FILE* factsFile = fopen(filename, "r");
    if (factsFile == NULL) {
        perror("Impossible d'ouvrir le fichier de faits");
        return NULL;
    }
    return factsFile;
 }
 Fact *lireFait(FILE *factsFile)
 {
    Fact* facts = NULL;
    char line[100];
    while (fgets(line, sizeof(line), factsFile) != NULL) {
        // Supprimer le caractère de saut de ligne à la fin
        line[strcspn(line, "\n")] = 0;
 
        // Ignorer les lignes vides
        if (strcmp(line, "") == 0)
            continue;
 
        // Ajouter le fait à la liste chaînée
        addFact(&facts, line);
    }
    fclose(factsFile);
    return facts;
 }
 Rule *lireRegle(FILE *rulesFile)
 {
    Rule *rules=NULL;
    char ruleLine[200];
    while (fgets(ruleLine, sizeof(ruleLine), rulesFile) != NULL) {
        char condition[100], conclusion[100];
        if (sscanf(ruleLine, "%99s %*s %99[^;];", condition, conclusion) == 2) {
            addRule(&rules, condition, conclusion);
        }
    }
    fclose(rulesFile);
    return rules;
 }
 Rule *libereRegle(Rule *rules)
 {
    Rule* currentRule = rules;
    while (currentRule != NULL) {
        Rule* temp = currentRule;
        currentRule = currentRule->next;
        free(temp);
    }
 }
 Fact *libereFait(Fact *facts)
 {
    Fact* currentFact = facts;
    while (currentFact != NULL) {
        Fact* temp = currentFact;
        currentFact = currentFact->next;
        free(temp);
    }
 }
void menu()
{
    FILE* factsFile = ouvrir_fichier("faits.kbs");
    Fact* facts = lireFait(factsFile);
    FILE* rulesFile = ouvrir_fichier("regles.kbs");
    Rule* rules = lireRegle(rulesFile);
    int choix = 0;
    printf("============================================================================================\n");
    printf("\t\t1. Chainage Avant \n\t\t2. Chainage Arriere \n\t\t3. Ajouter un Fait \n\t\t4. Ajouter un Regle\n\t\t0.Quitter");
    printf("============================================================================================\n");
    scanf("%s",&choix);
    switch (choix)
    {
        case 1:
            printf("Chaînage avant :\n");
            print_rules(rules);
            forwardChaining(facts, rules);
            break;
        case 2:
            printf("\nEntrez le goal : ");
            char goal[100];
            scanf("%s", goal);
            printf("\nChaînage arrière :\n");
            backwardChaining(goal, facts, rules);
            break;
        case 3:
    
    }
    libereFait(facts);
    libereRegle(rules);
}
// Fonction principale
int main() {
    menu();
    return 0;
}