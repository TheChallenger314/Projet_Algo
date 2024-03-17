#include <stdlib.h>
#include <stdio.h>
#include "GfxLib.h"
#include <string.h>
#include "ESLib.h"
#include "main.h"
#include "fonction.c"

char texteAAfficher[2000] = ""; // Variable pour stocker le texte à afficher
Fact* facts = NULL;
Rule* rules = NULL;
char goal[100], condition[100], conclusion[100];

void dessineBouton(char *texte, int x1, int y1, int x2, int y2) {
    // Dessine le rectangle du bouton
    couleurCourante(0, 0, 0);
    rectangle(x1, y1, x2, y2);

    couleurCourante(200, 200, 200);
    rectangle(x1 + 1, y1 + 1, x2 - 1, y2 - 1);

    couleurCourante(0, 0, 0);
    afficheChaine(texte, 15, (x1 + x2) / 2 - (strlen(texte) * 3), (y1 + y2) / 2);
}

void print_rules_on_screen(Rule* rules) {
    effaceFenetre(255, 255, 255); // Efface la fenêtre avant d'afficher les règles

    // Affiche le titre
    couleurCourante(0, 0, 0);
    afficheChaine("Liste des règles :", 20, 100, HauteurFenetre - 50);

    Rule* current = rules;
    int count = 0;
    int y = HauteurFenetre - 100; // Position verticale de la première règle

    while (current != NULL) {
        count++;
        char rule_text[200]; // Chaîne pour stocker le texte de la règle
        sprintf(rule_text, "n°%d) %.50s -> %.50s", count, current->condition->condition, current->conclusion);
        // Affiche la règle avec un décalage vertical pour chaque nouvelle règle
        afficheChaine(rule_text, 15, 100, y);
        y -= 20; // Déplace la position verticale pour la règle suivante

        current = current->next;
    }
}



void gestionEvenement(EvenementGfx evenement) {
    switch (evenement) {
        case Initialisation:
            demandeTemporisation(20);
            break;

        case Temporisation:
            rafraichisFenetre();
            break;

        case Affichage:
            effaceFenetre(255, 255, 255); // Efface la fenêtre

            // Dessine les boutons et affiche le texte
            dessineBouton("Affiche regle", LargeurFenetre / 2 - 100, 50, LargeurFenetre / 2 + 100, 150);
            dessineBouton("Chainage avant", LargeurFenetre / 2 - 100, 180, LargeurFenetre / 2 + 100, 280);
            dessineBouton("Chainage arriere", LargeurFenetre / 2 - 300, 50, LargeurFenetre / 2 - 100, 150);
            dessineBouton("Quitter&Sauvegarde", LargeurFenetre / 2 + 100, 50, LargeurFenetre / 2 + 300, 150);
            dessineBouton("Ajouter un Fait", LargeurFenetre / 2 - 300, 180, LargeurFenetre / 2 - 100, 280);
            dessineBouton("Ajouter une regle", LargeurFenetre / 2 + 100, 180, LargeurFenetre / 2 + 300, 280);

            // Affiche le texte
            couleurCourante(0, 0, 0);
            epaisseurDeTrait(3);
            afficheChaine(texteAAfficher, 10, LargeurFenetre / 2 - 100, HauteurFenetre / 2 + 170);

            // Affiche le texte d'invitation pour l'écriture

            break;

        case Clavier:
            break;

        case ClavierSpecial:
            break;

        case BoutonSouris:
            if (etatBoutonSouris() == GaucheAppuye) {
                int x = abscisseSouris();
                int y = ordonneeSouris();
                if (x >= LargeurFenetre / 2 - 100 && x <= LargeurFenetre / 2 + 100 && y >= 50 && y <= 150) {
                    strcpy(texteAAfficher, "Affiche regle clique !");
                    print_rules(rules);
                } else if (x >= LargeurFenetre / 2 - 100 && x <= LargeurFenetre / 2 + 100 && y >= 180 && y <= 280) {
                    strcpy(texteAAfficher, "Chainage avant clique !");
                    forwardChaining(&facts, rules);
                } else if (x >= LargeurFenetre / 2 - 300 && x <= LargeurFenetre / 2 - 100 && y >= 50 && y <= 150) {
                    strcpy(texteAAfficher, "Chainage arriere clique !");
                    printf("Entrez le goal pour le chaînage arrière : ");
                    scanf("%99s", goal);
                    backwardChaining(goal, facts, rules);
                } else if (x >= LargeurFenetre / 2 + 100 && x <= LargeurFenetre / 2 + 300 && y >= 50 && y <= 150) {
                    strcpy(texteAAfficher, "Quitter&sauvegarder clique !");
                    print_rules_to_file(rules, "couleur.kbs");
                    termineBoucleEvenements();
                } else if (x >= LargeurFenetre / 2 - 300 && x <= LargeurFenetre / 2 - 100 && y >= 180 && y <= 280) {
                    strcpy(texteAAfficher, "Ajouter un Fait clique !");
                    printf("Entrez le fait à ajouter : ");
                    if (fgets(goal, sizeof(goal), stdin) != NULL) {
                        goal[strcspn(goal, "\n")] = 0;
                        addFact(&facts, goal);
                    }
                } else if (x >= LargeurFenetre / 2 + 100 && x <= LargeurFenetre / 2 + 300 && y >= 180 && y <= 280) {
                    strcpy(texteAAfficher, "Ajouter une regle clique !");
                    printf("Entrez la condition de la nouvelle règle : ");
                    fgets(condition, sizeof(condition), stdin);
                    condition[strcspn(condition, "\n")] = 0;
                    printf("Entrez la conclusion de la nouvelle règle : ");
                    fgets(conclusion, sizeof(conclusion), stdin);
                    conclusion[strcspn(conclusion, "\n")] = 0;
                    addRule(&rules, condition, conclusion);
                }
            }
            break;

        case Souris:
            break;

        case Inactivite:
            break;

        case Redimensionnement:
            break;
    }
}

void initialisation() {
    int nbFaits;
    printf("Entrez le nombre de faits : ");
    scanf("%d", &nbFaits);
    while (getchar() != '\n'); // Nettoyer le buffer d'entrée après la lecture du nombre

    for (int i = 0; i < nbFaits; i++) {
        char fait[100];
        printf("Entrez le fait n°%d : ", i + 1);
        fgets(fait, sizeof(fait), stdin);
        fait[strcspn(fait, "\n")] = 0;
        addFact(&facts, fait);
    }
}

int main(int argc, char **argv) {
    initialiseGfx(argc, argv);
    loadRulesFromFile(&rules);
    initialisation();
    prepareFenetreGraphique("Projet de moteur d'inference", LargeurFenetre, HauteurFenetre);
    lanceBoucleEvenements();
    return 0;
}
