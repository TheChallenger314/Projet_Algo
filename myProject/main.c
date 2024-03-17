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
Fait* initFait(int nombre);
Rules* initRegle() {
    Rules* regle = malloc(sizeof(Rules));
    if (regle == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    regle->premise = initFait(3);
    regle->conclusion = NULL;
    regle->suivant = NULL;
    return regle;
}

char* convertionRegles(FILE* fichier) {
    // Determine the file size
    fseek(fichier, 0, SEEK_END);
    long file_size = ftell(fichier);
    rewind(fichier);

    // Allocate memory for the char array
    char* ligne = malloc(file_size + 1);
    if (ligne == NULL) {
        perror("Error: Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Read the file into the char array
    fread(ligne, file_size, 1, fichier);

    // Add a null terminator at the end of the char array
    ligne[file_size] = '\0';
    return ligne;
}

// Fonction pour initialiser un fait
Fait* initFait(int nombre) {
    Fait* nouveauFait = malloc(sizeof(Fait));
    if (nouveauFait == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    if (nombre == 0) {
        nouveauFait->fait = "";
        nouveauFait->suivant = NULL;
    } else {
        nouveauFait->fait = "";
        nouveauFait->suivant = initFait(nombre - 1);
    }
    return nouveauFait;
}

// Fonction pour ajouter un fait à une règle
void ajouterFait(Rules* regle, int nombre) {
    Fait* nouveauFait = initFait(nombre);
    nouveauFait->suivant = regle->premise;
    regle->premise = nouveauFait;
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

Rules* lireRegles(Rules* list, char* ligne)
{
    int i=0;
    char* buffer = malloc(strlen(ligne) + 1);
    memset(buffer, 0, strlen(ligne) + 1);
    Rules* current = list;
    while (ligne[i] != '\0') {
        if (ligne[i] == ' ') {
            if (current->premise == NULL) {
                current->premise = malloc(sizeof(char) * (strlen(buffer) + 1));
                if (current->premise == NULL) {
                    perror("Erreur lors de l'allocation de mémoire");
                    exit(EXIT_FAILURE);
                }
                strcpy(current->premise->fait, buffer);
            } else {
                Fait* newFait = malloc(sizeof(Fait));
                if (newFait == NULL) {
                    perror("Erreur lors de l'allocation de mémoire");
                    exit(EXIT_FAILURE);
                }
                newFait->fait = malloc(strlen(buffer) + 1);
                if (newFait->fait == NULL) {
                    perror("Erreur lors de l'allocation de mémoire");
                    exit(EXIT_FAILURE);
                }
                strcpy(newFait->fait, buffer);
                newFait->suivant = current->premise;
                current->premise = newFait;
            }
            buffer[0] = '\0';
        } else if (ligne[i] == '-' && ligne[i+1] == '>') {
            current->conclusion = strdup(buffer);
            i += 3;
            buffer[0] = '\0';
        }
        if (ligne[i+1] == '\0' || ligne[i+2] == '\0') {
            current->suivant = initRegle();
            current = current->suivant;
        }
        if (ligne[i] != ' ' && ligne[i] != '-' && ligne[i] != '>') {
            strncat(buffer, ligne + i, 1);
        }
        i++;
    }
    free(buffer);
    return list;
}
// Fonction pour évaluer les faits avec les règles
void evaluerFaits(Fait* faits, Rules* listeRegles) {
    printf("\nRésultat en fonction des règles :\n");

    while (listeRegles != NULL) {
        Fait* premisses = listeRegles->premise;
        int match = 1;

        while (premisses != NULL) {Fait* faitCourant = faits;
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

    Fait* faits = NULL;
    int nombreFaits;
    printf("Entrez le nombre de faits : ");
    scanf("%d", &nombreFaits);

    char* ligne = convertionRegles(fichier);
    Rules* listeRegles = initRegle();
    listeRegles = lireRegles(listeRegles, ligne);

    fclose(fichier);

    for (int i = 0; i < nombreFaits; i++) {
        char fait[50];
        printf("Entrez le fait %d : ", i + 1);
        scanf("%s", fait);
        ajouterFait(listeRegles, nombreFaits);
    }

    afficherFaits(faits);
    afficherRegles(listeRegles);
    evaluerFaits(faits, listeRegles);

    libererRegles(listeRegles);

    return EXIT_SUCCESS;
}