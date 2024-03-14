#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FACT_LENGTH 10

// Structure pour représenter une règle
typedef struct Rule {
    char *condition;
    char *result;
    struct Rule *next;
} Rule;

// Structure pour représenter une liste chaînée de règles
typedef struct RuleList {
    Rule *head;
    struct RuleList *next;
} RuleList;

// Fonction pour libérer la mémoire allouée pour les règles
void freeRules(RuleList *ruleList) {
    while (ruleList != NULL) {
        RuleList *temp = ruleList;
        ruleList = ruleList->next;
        while (temp->head != NULL) {
            Rule *tempRule = temp->head;
            temp->head = temp->head->next;
            free(tempRule->condition);
            free(tempRule->result);
            free(tempRule);
        }
        free(temp);
    }
}

// Fonction pour ajouter une règle à une liste de règles
void addRule(RuleList **ruleList, char *condition, char *result) {
    if (*ruleList == NULL) {
        *ruleList = (RuleList *)malloc(sizeof(RuleList));
        (*ruleList)->head = NULL;
        (*ruleList)->next = NULL;
    }

    Rule *newRule = (Rule *)malloc(sizeof(Rule));
    newRule->condition = strdup(condition);
    newRule->result = strdup(result);
    newRule->next = (*ruleList)->head;
    (*ruleList)->head = newRule;
}

// Fonction pour vérifier si une condition est satisfaite
int checkCondition(char *condition, char *facts) {
    char *token = strtok(condition, " ");
    printf("%s %s\n",token,facts);
    while (token != NULL) {
        if (strstr(facts, token) == NULL)
            return 0;
        token = strtok(NULL, " ");
    }
    return 1;
}

// Fonction pour appliquer les règles et retourner le résultat correspondant aux faits
char *applyRules(RuleList *ruleList, char *facts) {
    char *finalResult = (char *)malloc(1); // Chaîne de caractères finale
    finalResult[0] = '\0'; // Chaîne vide
    while (ruleList != NULL) {
        Rule *rule = ruleList->head;
        while (rule != NULL) {
            if (checkCondition(rule->condition, facts)) {
                // Réallouer la mémoire pour la nouvelle taille de la chaîne de caractères finale
                finalResult = (char *)realloc(finalResult, strlen(finalResult) + strlen(rule->result) + 1);
                strcat(finalResult, rule->result); // Concaténer le résultat avec la chaîne de caractères finale
            }
            rule = rule->next;
        }
        ruleList = ruleList->next;
    }
    return finalResult;
}

int main() {
    // Ajout des règles
    RuleList *ruleList = NULL;
    addRule(&ruleList, "a", "beta");
    addRule(&ruleList, "a b c", "e");
    addRule(&ruleList, "b", "d");

    // Saisie des faits
    char facts[MAX_FACT_LENGTH];
    printf("Entrez les faits (séparés par des espaces) : ");
    scanf("%s", facts);

    // Application des règles
    char *resultFact = applyRules(ruleList, facts);
    
    // Affichage du résultat
    if (strlen(resultFact) > 0) {
        printf("Le résultat correspondant aux faits est : %s\n", resultFact);
    } else {
        printf("Aucune règle ne correspond aux faits donnés.\n");
    }

    // Libération de la mémoire allouée pour les règles
    freeRules(ruleList);
    free(resultFact); // Libérer la mémoire allouée pour le résultat

    return 0;
}
