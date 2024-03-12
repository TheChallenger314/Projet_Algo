#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure représentant une règle
typedef struct {
    char premisses[10][50];  // Maximum de 10 prémices
    char conclusion[50];
} Rule;

// Fonction pour lire les règles depuis le fichier
Rule* lire_regles(FILE* fichier, int* nombre_regles) {
    // Assume un maximum de 100 règles, ajuster si nécessaire
    Rule* regles = malloc(100 * sizeof(Rule));
    if (regles == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (fscanf(fichier, "%s", regles[i].premisses[0]) == 1) {
        int j = 1;
        while (fgetc(fichier) == ' '&& fgetc(fichier)!='-') {
            fscanf(fichier, "%s", regles[i].premisses[j]);
            j++;
        }

        fscanf(fichier, "> %s;", regles[i].conclusion);
        printf("%s",regles[i].conclusion);
        i++;
    }

    *nombre_regles = i;
    return regles;
}

// Fonction pour afficher les faits
void afficher_faits(char faits[10][50], int nombre_faits) {
    printf("Faits entrés par l'utilisateur :\n");
    for (int i = 0; i < nombre_faits; i++) {
        printf("Fait %d : %s\n", i + 1, faits[i]);
    }
}

// Fonction pour évaluer les faits avec les règles
void evaluer_faits(char faits[10][50], int nombre_faits, Rule* regles, int nombre_regles) {
    printf("\nRésultat en fonction des règles :\n");

    if (nombre_faits < 1 || nombre_faits > 10) {
        printf("Le nombre de faits doit être compris entre 1 et 10.\n");
        return;
    }

    for (int i = 0; i < nombre_regles; i++) {
        int match = 1;
        for (int j = 0; j < nombre_faits; j++) {
            if (strcmp(faits[j], regles[i].premisses[j]) != 0) {
                match = 0;
                break;
            }
        }

        if (match) {
            printf("Règle correspondante trouvée : ");
            for (int k = 0; k < nombre_faits; k++) {
                printf("%s ", regles[i].premisses[k]);
            }
            printf("-> %s\n", regles[i].conclusion);
            printf("Résultat : %s\n", regles[i].conclusion);
            return;
        }
    }

    printf("Aucune règle correspondante trouvée.\n");
}

int main() {
    FILE* fichier = fopen("regle.kbs", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    int nombre_regles;
    Rule* regles = lire_regles(fichier, &nombre_regles);

    int nombre_faits;
    printf("Entrez le nombre de faits : ");
    scanf("%d", &nombre_faits);

    char faits[nombre_faits][50];

    // Laisser l'utilisateur saisir les faits
    for (int i = 0; i < nombre_faits; i++) {
        printf("Entrez le fait %d : ", i + 1);
        scanf("%s", faits[i]);
    }

    // Afficher les faits
    afficher_faits(faits, nombre_faits);

    // Évaluer les faits avec les règles
    evaluer_faits(faits, nombre_faits, regles, nombre_regles);

    fclose(fichier);
    free(regles);

    return EXIT_SUCCESS;
}
