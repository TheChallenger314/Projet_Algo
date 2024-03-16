#include "expert_system.h"

int main() {
    FILE* factsFile = fopen("faits.kbs", "r");
    if (factsFile == NULL) {
        perror("Impossible d'ouvrir le fichier de faits");
        return 1;
    }

    // Lire les faits du fichier et les stocker dans une liste chaînée
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

    // Ouvrir le fichier de règles
    FILE* rulesFile = fopen("regles.kbs", "r");
    if (rulesFile == NULL) {
        perror("Impossible d'ouvrir le fichier de règles");
        return 1;
    }

    // Lire les règles du fichier et les stocker dans une liste chaînée
    Rule* rules = NULL;
    char ruleLine[200];
    while (fgets(ruleLine, sizeof(ruleLine), rulesFile) != NULL) {
        char condition[100], conclusion[100];
        if (sscanf(ruleLine, "%99s %*s %99[^;];", condition, conclusion) == 2) {
            addRule(&rules, condition, conclusion);
        }
    }
    fclose(rulesFile);

    // Appliquer le chaînage avant
    printf("Chaînage avant :\n");
    print_rules(rules);
    forwardChaining(facts, rules);

    // Demander à l'utilisateur d'entrer le goal
    printf("\nEntrez le goal : ");
    char goal[100];
    scanf("%s", goal);

    // Appliquer le chaînage arrière
    printf("\nChaînage arrière :\n");
    backwardChaining(goal, facts, rules);

    // Libérer la mémoire
    Fact* currentFact = facts;
    while (currentFact != NULL) {
        Fact* temp = currentFact;
        currentFact = currentFact->next;
        free(temp);
    }

    Rule* currentRule = rules;
    while (currentRule != NULL) {
        Rule* temp = currentRule;
        currentRule = currentRule->next;
        free(temp);
    }

    return 0;
}
