#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure de fait
typedef struct Fact {
    char description[100]; // Assumons une longueur maximale de 100 caractères pour la description du fait
    struct Fact* next;
} Fact;
typedef struct Condition{
    char cond[100]; //taille 100
    struct Condition *suiv;
}Condition;
// Structure de règle
typedef struct Rule {
    Condition *condition; // Assumons une longueur maximale de 100 caractères pour la condition de la règle
    char conclusion[100]; // Assumons une longueur maximale de 100 caractères pour la conclusion de la règle
    struct Rule* next;
} Rule;

FILE *rulesFile= NULL;
Fact* facts = NULL;
FILE* factsFile = NULL;
Rule* rules = NULL;
// Fonction pour ajouter un fait à la liste chaînée

Fact * init_fact(){
    Fact *newfact = (Fact*)malloc(sizeof(Fact));
    int i=0;
    strcpy(newfact->description," ");
    newfact->next =NULL;
    return newfact;
}
Fact *addFact(Fact* head, char* description) {
    Fact* newFact = head;
    
    if (newFact == NULL) {
        newFact =init_fact();
        strcpy(newFact->description, description);
        return newFact;
    }
    while(newFact != NULL)
    {
        newFact=newFact->next;
    }
    newFact=init_fact();
    strcpy(newFact->description, description);
    return newFact;
}

// Fonction pour ajouter une règle à la liste chaînée

void addRule(Rule** head, Condition* condition, char* conclusion) {
    Rule* newRule = (Rule*)malloc(sizeof(Rule));
    newRule->condition = (Condition*)malloc(sizeof(Condition));
    strcpy(newRule->condition->cond,"");
    if (newRule == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    // Créer la liste chaînée des conditions
    Condition* currentCondition = condition;
    char conditionStr[100] = ""; // Initialiser la chaîne de caractères pour stocker la condition
    while (currentCondition != NULL) {
        strcat(conditionStr, currentCondition->cond);
        strcat(conditionStr, " ");
        currentCondition = currentCondition->suiv;
    }

    strcpy(newRule->condition->cond, conditionStr);
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
                printf("Condition: %s est vraie, conclusion: %s\n", currentRule->condition->cond, currentRule->conclusion);
            if (strstr(currentFact->description, currentRule->condition->cond) != NULL) {
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
                    addFact(facts, currentRule->conclusion);
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
            // Vérifier si les conditions de la règle peuvent être prouvées
            printf("Goal: %s peut être prouvé par la règle: %s -> %s\n", goal, currentRule->condition->cond, currentRule->conclusion);
            int conditionsProuvees = 1;
            char* token = strtok(currentRule->condition->cond, " ");
            while (token != NULL) {
                if (!backwardChaining(token, facts, rules)) {
                    conditionsProuvees = 0;
                    break; // Si une condition ne peut pas être prouvée, arrêter la vérification des autres conditions
                }
                token = strtok(NULL, " ");
            }
            if (conditionsProuvees) {
                printf("Goal: %s est prouvé\n", goal); // Afficher que le goal est prouvé
                printf("Conditions prouvées pour la règle: %s -> %s\n", currentRule->condition->cond, currentRule->conclusion);
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
        printf("Rule %d:\n", count);
        printf("Conditions:\n");
        // Affichage des conditions
        char* token = strtok(current->condition->cond, " ");
        while (token != NULL) {
            printf("%s\n", token);
            token = strtok(NULL, " ");
        }
        printf("Conclusion: %s\n", current->conclusion);
        current = current->next;
    }
}




FILE *ouvrir_fichier(char *filename) {
    FILE* factsFile = fopen(filename, "r");
    if (factsFile == NULL) {
        perror("Impossible d'ouvrir le fichier de faits");
        return NULL;
    }
    return factsFile;
}

Fact *lireFait(FILE *factsFile) {
    Fact* facts = NULL;
    Fact* newfacts = NULL;
    char line[100];
    while (fgets(line, sizeof(line), factsFile) != NULL) {
        // Supprimer le caractère de saut de ligne à la fin
        line[strcspn(line, "\n")] = 0;

        // Ignorer les lignes vides
        if (strcmp(line, "") == 0)
            continue;
        newfacts=addFact(facts, line);
        newfacts=newfacts->next;
    }
    facts= newfacts;
    while(facts != NULL)
    {
        printf("%s",facts->description);
        facts=facts->next;
    }
    fclose(factsFile);
    return facts;
}


Rule *lireRegle(FILE *rulesFile) {
    Rule *rules = NULL;
    char ruleLine[200];
    
    while (fgets(ruleLine, sizeof(ruleLine), rulesFile) != NULL) {
        Condition *condition=(Condition*)malloc(sizeof(Condition));
        char conclusion[100]="";
        if (sscanf(ruleLine, "%99s %99[^;];", condition->cond, conclusion) == 2) {
            addRule(&rules, condition, conclusion);
        }
    }
    fclose(rulesFile);
    return rules;
}



Rule *libereRegle(Rule *rules) {
    Rule* currentRule = rules;
    while (currentRule != NULL) {
        Rule* temp = currentRule;
        currentRule = currentRule->next;
        free(temp);
    }
    return NULL; // Ajout de retour de la tête de liste
}

Fact *libereFait(Fact *facts) {
    Fact* currentFact = facts;
    while (currentFact != NULL) {
        Fact* temp = currentFact;
        currentFact = currentFact->next;
        free(temp);
    }
    return NULL; // Ajout de retour de la tête de liste
}

int menu() {
    
    int choix = 0;
    printf("============================================================================================\n");
    printf("\t\t1. Chainage Avant \n\t\t2. Chainage Arriere \n\t\t3. Ajouter un Fait \n\t\t4. Ajouter un Regle\n\t\t5.Afficher les regles\n\t\t0.Quitter\n");
    printf("============================================================================================\n");
    scanf("%d",&choix);
    switch (choix) {
        case 1:
            printf("Chaînage avant :\n");
            forwardChaining(facts, rules);
            break;
        case 2:
            printf("\nEntrez le goal : ");
            char goal[100];
            scanf("%s", goal);
            printf("\nChaînage arrière :\n");
            backwardChaining(goal, facts, rules);
            break;
        case 3: {
            char description[100]; // Modification pour éviter les dépassements de tampon
            printf("Quel est le nouveau fait ?\n");
            scanf("%s", description);
            addFact(facts, description);
            break;
        }
        case 4: {
            Condition *condition=(Condition*)malloc(sizeof(Condition));
            char conclusion[100];
            printf("Quelle est la nouvelle regle ?\n");
            scanf("%s %s -> %s", condition->cond, condition->suiv->cond, conclusion);
            addRule(&rules, condition, conclusion);
            break;
        }
        case 5:
            print_rules(rules);
            break;
        case 0:
            printf("EXIT\n");
            facts = libereFait(facts);
            rules = libereRegle(rules);
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }
    return choix;
}

// Fonction principale
int main() {
    int choix;
    factsFile = ouvrir_fichier("faits.kbs");
    facts = lireFait(factsFile);
    rulesFile = ouvrir_fichier("regles.kbs");
    rules = lireRegle(rulesFile);
    do {
        choix = menu();
    } while (choix != 0); // Continuer tant que l'utilisateur ne choisit pas de quitter
    return 0;
}
