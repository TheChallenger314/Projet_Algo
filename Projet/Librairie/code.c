#include <stdlib.h>
#include <stdio.h>
#include "GfxLib.h"
#include <string.h>
#include "ESLib.h"

#define LargeurFenetre 800
#define HauteurFenetre 600

char texteAAfficher[100] = ""; // Variable pour stocker le texte à afficher
char texteAEcrire[100] = ""; // Variable pour stocker le texte à écrire

void dessineBouton(char *texte, int x1, int y1, int x2, int y2) {
    // Dessine le rectangle du bouton
    couleurCourante(0, 0, 0);
    rectangle(x1, y1, x2, y2);

    couleurCourante(200, 200, 200);
    rectangle(x1+1, y1+1, x2-1, y2-1);

    couleurCourante(0,0,0);
    afficheChaine(texte, 15, (x1 + x2) / 2 - (strlen(texte) * 3), (y1 + y2) / 2);
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
            dessineBouton("Affiche regle", LargeurFenetre/2 - 100, 50, LargeurFenetre/2 + 100, 150);
            dessineBouton("Chainage avant", LargeurFenetre/2 - 100, 180, LargeurFenetre/2 + 100, 280);
            dessineBouton("Chainage arriere", LargeurFenetre/2 - 300, 50, LargeurFenetre/2 - 100, 150);
            dessineBouton("Quitter&Sauvegarde", LargeurFenetre/2 + 100, 50, LargeurFenetre/2 + 300, 150);
            dessineBouton("Ajouter un Fait", LargeurFenetre/2 - 300, 180, LargeurFenetre/2 - 100, 280);
            dessineBouton("Ajouter une regle", LargeurFenetre/2 + 100, 180, LargeurFenetre/2 + 300, 280);

            // Affiche le texte
            couleurCourante(0, 0, 0);
            epaisseurDeTrait(3);
            afficheChaine(texteAAfficher, 20, LargeurFenetre/2 - 100, HauteurFenetre/2 + 170);
            
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
                if (x >= LargeurFenetre/2 - 100 && x <= LargeurFenetre/2 + 100 && y >= 50 && y <= 150) {
                    strcpy(texteAAfficher, "Affiche regle clique !");
                } else if (x >= LargeurFenetre/2 - 100 && x <= LargeurFenetre/2 + 100 && y >= 180 && y <= 280) {
                    strcpy(texteAAfficher, "Chainage avant clique !");
                } else if (x >= LargeurFenetre/2 - 300 && x <= LargeurFenetre/2 - 100 && y >= 50 && y <= 150) {
                    strcpy(texteAAfficher, "Chainage arriere clique !");
                } else if (x >= LargeurFenetre/2 + 100 && x <= LargeurFenetre/2 + 300 && y >= 50 && y <= 150) {
                    strcpy(texteAAfficher, "Quitter&sauvegarder clique !");
                } else if (x >= LargeurFenetre/2 - 300 && x <= LargeurFenetre/2 - 100 && y >= 180 && y <= 280) {
                    strcpy(texteAAfficher, "Ajouter un Fait clique !");
                } else if (x >= LargeurFenetre/2 + 100 && x <= LargeurFenetre/2 + 300 && y >= 180 && y <= 280) {
                    strcpy(texteAAfficher, "Ajouter une regle clique !");
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

int main(int argc, char **argv) {
    initialiseGfx(argc, argv);
    prepareFenetreGraphique("C'est notre Projet", LargeurFenetre, HauteurFenetre);

    lanceBoucleEvenements();
    return 0;
}
