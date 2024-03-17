#include "expert_system.h"

// Fonction pour ajouter un fait à la liste de faits
void ajouterFait(Fait** tete, char* description) {
    Fait* nouveauFait = (Fait*)malloc(sizeof(Fait));
    if (nouveauFait == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les faits.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nouveauFait->description, description);
    nouveauFait->suivant = *tete;
    *tete = nouveauFait;
}

// Fonction pour ajouter une règle à la liste de règles
void ajouterRegle(Regle** tete, char* condition, char* conclusion) {
    Regle* nouvelleRegle = (Regle*)malloc(sizeof(Regle));
    if (nouvelleRegle == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    nouvelleRegle->condition = (Condition*)malloc(sizeof(Condition));
    if (nouvelleRegle->condition == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    strcpy(nouvelleRegle->condition->condition, condition);
    nouvelleRegle->condition->suivant = NULL;
    strcpy(nouvelleRegle->conclusion, conclusion);
    nouvelleRegle->suivant = NULL;

    if (*tete == NULL) {
        *tete = nouvelleRegle;
    } else {
        Regle* courant = *tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouvelleRegle;
    }
}

// Affiche la liste des règles
void afficherRegles(Regle* regles) {
    printf("Liste des règles :\n");
    Regle* courant = regles;
    int compteur = 0;
    while (courant != NULL) {
        compteur++;
        printf("Règle n°%d) ", compteur);
        printf("%s -> %s\n", courant->condition->condition, courant->conclusion);
        courant = courant->suivant;
    }
}

// Vérifie si un fait existe déjà dans la liste de faits
bool faitExisteDansFaits(const char* descriptionFait, Fait* faits) {
    while (faits != NULL) {
        if (strcmp(faits->description, descriptionFait) == 0) {
            return true;
        }
        faits = faits->suivant;
    }
    return false;
}

// Charge les règles depuis un fichier
void chargerReglesDepuisFichier(Regle** regles) {
    FILE* fichier = fopen("regles.kbs", "r");
    if (!fichier) {
        perror("Impossible d'ouvrir le fichier de règles");
        exit(EXIT_FAILURE);
    }

    char ligne[200];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char condition[100], conclusion[100];
        if (sscanf(ligne, "%99[^-]-> %99[^\n];", condition, conclusion) == 2) {
            ajouterRegle(regles, condition, conclusion);
        }
    }

    fclose(fichier);
}

// Libère la mémoire allouée pour les faits
void libererFaits(Fait* tete) {
    Fait* tmp;
    while (tete != NULL) {
        tmp = tete;
        tete = tete->suivant;
        free(tmp);
    }
}

// Libère la mémoire allouée pour les règles
void libererRegles(Regle* tete) {
    Regle* tmp;
    while (tete != NULL) {
        tmp = tete;
        tete = tete->suivant; // Libère aussi la mémoire allouée pour la condition
        free(tmp);
    }
}

// Sauvegarde les règles actuelles dans un fichier
void sauvegarderReglesDansFichier(Regle* regles) {
    FILE* fichier = fopen("regles.kbs", "w");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
    Regle* courant = regles;
    while (courant != NULL) {
        fprintf(fichier, "%s -> %s\n", courant->condition->condition, courant->conclusion);
        courant = courant->suivant;
    }
    fclose(fichier);
}

// Affiche le menu principal du programme
void afficherMenu() {
    printf("\nMenu Principal\n");
    printf("1. Afficher les règles\n");
    printf("2. Effectuer un chaînage avant\n");
    printf("3. Effectuer un chaînage arrière\n");
    printf("4. Ajouter un nouveau fait\n");
    printf("5. Ajouter une nouvelle règle\n");
    printf("6. Quitter\n");
}


// Gère la saisie de l'utilisateur et les actions correspondantes
void gererSaisieUtilisateur(Fait** faits, Regle** regles) {
    int nombreFaits;
    printf("Entrez le nombre de faits : ");
    scanf("%d", &nombreFaits);
    while (getchar() != '\n');

    for (int i = 0; i < nombreFaits; i++) {
        char fait[100];
        printf("Entrez le fait n°%d : ", i + 1);
        fgets(fait, sizeof(fait), stdin);
        fait[strcspn(fait, "\n")] = 0;
        ajouterFait(faits, fait);
    }
    int choix;
    char objectif[100], condition[100], conclusion[100];

    while(1) {
        afficherMenu();
        printf("Entrez votre choix : ");
        scanf("%d", &choix);
        while(getchar() != '\n');

        switch(choix) {
            case 1:
                afficherRegles(*regles);
                break;
            case 2:
                chainageAvant(faits, *regles);
                break;
            case 3:
                printf("Entrez l'objectif pour le chaînage arrière : ");
                fgets(objectif, sizeof(objectif), stdin);
                objectif[strcspn(objectif, "\n")] = 0;
                chainageArriere(objectif, *faits, *regles, NULL);
                break;
            case 4:
                printf("Entrez le fait à ajouter : ");
                if (fgets(objectif, sizeof(objectif), stdin) != NULL) {
                    objectif[strcspn(objectif, "\n")] = 0;
                    ajouterFait(faits, objectif);
                }
                break;
            case 5:
                printf("Entrez la condition de la nouvelle règle : ");
                fgets(condition, sizeof(condition), stdin);
                condition[strcspn(condition, "\n")] = 0;
                printf("Entrez la conclusion de la nouvelle règle : ");
                fgets(conclusion, sizeof(conclusion), stdin);
                conclusion[strcspn(conclusion, "\n")] = 0;
                ajouterRegle(regles, condition, conclusion);
                sauvegarderReglesDansFichier(*regles);
                break;
            case 6:
                printf("Quitter.\n");
                return;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    }
}
