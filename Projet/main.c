#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure représentant un fait
typedef struct Fait {
    char* fait;
    struct Fait* suivant;
} Fait;

// Structure représentant une règle
typedef struct Rules {
    char* conclusion;
    Fait* premise;
    struct Rules* suivant;
} Rules;

Rules* lireRegles(FILE* fichier, int* nombreRegles) {
    Rules* listeRegles = NULL;

    char conclusion[50];
    while (fscanf(fichier, "%s", conclusion) == 1) {
        ajouterRegle(&listeRegles, conclusion);

        char fait[50];
        while (fscanf(fichier, "%s", fait) == 1 && strcmp(fait, "->") != 0) {
            ajouterFait(listeRegles, fait);
        }
    }

    *nombreRegles = 0;
    Rules* tmp = listeRegles;
    while (tmp != NULL) {
        (*nombreRegles)++;
        tmp = tmp->suivant;
    }

    return listeRegles;
}
// Fonction pour initialiser un fait
Fait* initFait(char* fait) {
    Fait* nouveauFait = malloc(sizeof(Fait));
    if (nouveauFait == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    nouveauFait->fait = strdup(fait);
    nouveauFait->suivant = NULL;
    return nouveauFait;
}

// Fonction pour ajouter un fait à une règle
void ajouterFait(Rules* regle, char* fait) {
    Fait* nouveauFait = initFait(fait);
    nouveauFait->suivant = regle->premise;
    regle->premise = nouveauFait;
}

// Fonction pour initialiser une règle
Rules* initRegle(char* conclusion) {
    Rules* nouvelleRegle = malloc(sizeof(Rules));
    if (nouvelleRegle == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    nouvelleRegle->conclusion = strdup(conclusion);
    nouvelleRegle->premise = NULL;
    nouvelleRegle->suivant = NULL;
    return nouvelleRegle;
}

// Fonction pour ajouter une règle à une liste de règles
void ajouterRegle(Rules** listeRegles, char* conclusion) {
    Rules* nouvelleRegle = initRegle(conclusion);
    nouvelleRegle->suivant = *listeRegles;
    *listeRegles = nouvelleRegle;
}

// Fonction pour afficher les faits
void afficherFaits(Fait* faits) {
    printf("Faits entrés par l'utilisateur :\n");
    while (faits != NULL) {
        printf("Fait : %s\n", faits->fait);
        faits = faits->suivant;
    }
}

// Fonction pour afficher les règles
void afficherRegles(Rules* listeRegles) {
    printf("\nListe des règles :\n");
    while (listeRegles != NULL) {
        Fait* premisses = listeRegles->premise;
        printf("Règle : ");
        while (premisses != NULL) {
            printf("%s ", premisses->fait);
            premisses = premisses->suivant;
        }
        printf("-> %s\n", listeRegles->conclusion);
        listeRegles = listeRegles->suivant;
    }
}

// Fonction pour évaluer les faits avec les règles
void evaluerFaits(Fait* faits, Rules* listeRegles) {
    printf("\nRésultat en fonction des règles :\n");

    while (listeRegles != NULL) {
        Fait* premisses = listeRegles->premise;
        int match = 1;

        while (premisses != NULL) {
            Fait* faitCourant = faits;
            int faitTrouve = 0;

            while (faitCourant != NULL) {
                if (strcmp(faitCourant->fait, premisses->fait) == 0) {
                    faitTrouve = 1;
                    break;
                }
                faitCourant = faitCourant->suivant;
            }

            if (!faitTrouve) {
                match = 0;
                break;
            }

            premisses = premisses->suivant;
        }

        if (match) {
            printf("Règle correspondante trouvée : ");
            Fait* premissesAffichage = listeRegles->premise;
            while (premissesAffichage != NULL) {
                printf("%s ", premissesAffichage->fait);
                premissesAffichage = premissesAffichage->suivant;
            }
            printf("-> %s\n", listeRegles->conclusion);
            printf("Résultat : %s\n", listeRegles->conclusion);
            return;
        }

        listeRegles = listeRegles->suivant;
    }

    printf("Aucune règle correspondante trouvée.\n");
}

// Fonction pour libérer la mémoire des faits
void libererFaits(Fait* faits) {
    while (faits != NULL) {
        Fait* suivant = faits->suivant;
        free(faits->fait);
        free(faits);
        faits = suivant;
    }
}

// Fonction pour libérer la mémoire des règles
void libererRegles(Rules* listeRegles) {
    while (listeRegles != NULL) {
        Rules* suivant = listeRegles->suivant;
        free(listeRegles->conclusion);
        libererFaits(listeRegles->premise);
        free(listeRegles);
        listeRegles = suivant;
    }
}

int main() {
    FILE* fichier = fopen("regle.kbs", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    Rules* listeRegles = NULL;

    char conclusion[50];
    while (fscanf(fichier, "%s", conclusion) == 1) {
        ajouterRegle(&listeRegles, conclusion);

        char fait[50];
        while (fscanf(fichier, "%s", fait) == 1 && strcmp(fait, "->") != 0) {
            ajouterFait(listeRegles, fait);
        }
    }

    fclose(fichier);

    Fait* faits = NULL;
    int nombreFaits;
    printf("Entrez le nombre de faits : ");
    scanf("%d", &nombreFaits);

    for (int i = 0; i < nombreFaits; i++) {
        char fait[50];
        printf("Entrez le fait %d : ", i + 1);
        scanf("%s", fait);
        Fait* nouveauFait = initFait(fait);
        nouveauFait->suivant = faits;
        faits = nouveauFait;
    }

    afficherFaits(faits);
    afficherRegles(listeRegles);
    evaluerFaits(faits, listeRegles);

    libererFaits(faits);
    libererRegles(listeRegles);

    return EXIT_SUCCESS;
}
