#include "expert_system.h"

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
                scanf("%99s", goal);
                backwardChaining(goal, *facts, *rules);
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
