#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Définition d'une structure pour les faits
typedef struct Fait {
    char description[100];
    struct Fait* suivant;
} Fait;

// Définition d'une structure pour les conditions des règles
typedef struct Condition {
    char condition[100];
    struct Condition* suivant;
} Condition;


// Définition d'une structure pour les règles
typedef struct Regle {
    Condition* condition;
    char conclusion[100];
    struct Regle* suivant;
} Regle;


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


// Implémente le chaînage avant pour inférer de nouveaux faits à partir des règles et faits existants
void chainageAvant(Fait** faits, Regle* regles) {
    bool nouveauFaitAjoute = true;
    int iteration = 1;

    printf("Début du chaînage avant :\n");

// Continue tant que de nouveaux faits sont ajoutés à chaque itération
    while (nouveauFaitAjoute) {
        nouveauFaitAjoute = false;
        printf("Itération %d :\n", iteration++);

        Regle* regleCourante = regles;
        bool faitsAjoutesDansIteration = false;
        while (regleCourante != NULL) {
            bool toutesConditionsSatisfaites = true;
            char* jeton = strtok(regleCourante->condition->condition, " ");
            while (jeton != NULL) {
                if (!faitExisteDansFaits(jeton, *faits)) {
                    toutesConditionsSatisfaites = false;
                    break;
                }
                jeton = strtok(NULL, " ");
            }

            if (toutesConditionsSatisfaites) {
                if (!faitExisteDansFaits(regleCourante->conclusion, *faits)) {
                    ajouterFait(faits, regleCourante->conclusion);
                    printf("Nouveau fait ajouté : %s\n", regleCourante->conclusion);
                    nouveauFaitAjoute = true;
                    faitsAjoutesDansIteration = true;
                }
            }
            regleCourante = regleCourante->suivant;
        }

        if (!faitsAjoutesDansIteration) {
            break;
        }
    }

    printf("Fin du chaînage avant.\n");
}


// Fonction de chaînage arrière qui vérifie si un objectif peut être atteint à partir des faits et règles existants
int chainageArriere(char* objectif, Fait* faits, Regle* regles, Fait* faitsVerifies) {
    // Vérifie si l'objectif est déjà un fait
    Fait* faitCourant = faits;
    while (faitCourant != NULL) {
        if (strstr(objectif, faitCourant->description) != NULL) {
            printf("Objectif: %s est vrai\n", objectif);
            return 1;
        }
        faitCourant = faitCourant->suivant;
    }

 // Évite les vérifications répétées pour prévenir les boucles infinies
    Fait* faitVerifie = faitsVerifies;
    while (faitVerifie != NULL) {
        if (strstr(objectif, faitVerifie->description) != NULL) {
            printf("Objectif: %s a déjà été vérifié, évitant les boucles infinies.\n", objectif);
            return 0; // Objectif déjà vérifié
        }
        faitVerifie = faitVerifie->suivant;
    }

    ajouterFait(&faitsVerifies, objectif);

    Regle* regleCourante = regles;
    while (regleCourante != NULL) {
        if (strstr(regleCourante->conclusion, objectif) != NULL) {
            printf("Objectif: %s peut être prouvé par la règle: %s -> %s\n", objectif, regleCourante->condition->condition, regleCourante->conclusion);
            
            char* jeton = strtok(regleCourante->condition->condition, " ");
            bool conditionsProuvees = true;
            while (jeton != NULL) {
                if (!chainageArriere(jeton, faits, regles, faitsVerifies)) {
                    conditionsProuvees = false;
                    printf("La condition %s dans la règle %s -> %s ne peut pas être prouvée.\n", jeton, regleCourante->condition->condition, regleCourante->conclusion);
                    break;
                }
                jeton = strtok(NULL, " ");
            }

            if (conditionsProuvees) {
                printf("Conditions prouvées pour la règle: %s -> %s\n", regleCourante->condition->condition, regleCourante->conclusion);
                printf("Objectif: %s est prouvé\n", objectif);
                return 1; // Toutes les conditions sont prouvées
            }
        }
        regleCourante = regleCourante->suivant;
    }

    printf("Objectif: %s ne peut pas être prouvé\n", objectif);
    return 0; // Objectif ne peut pas être prouvé
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

int main() {
    Fait* faits = NULL;
    Regle* regles = NULL;

    chargerReglesDepuisFichier(&regles);
    gererSaisieUtilisateur(&faits, &regles);

    libererFaits(faits);
    libererRegles(regles);

    return 0;
}
