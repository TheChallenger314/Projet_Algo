#include <stdlib.h>
#include <stdio.h>
#include "GfxLib.h"
#include <string.h>

#define LargeurFenetre 800
#define HauteurFenetre 600

char texteAAfficher[100] = ""; // Variable pour stocker le texte à afficher

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

    // Affichage des rectangles
    couleurCourante(200, 0, 0);
    rectangle(LargeurFenetre/2 - 100, HauteurFenetre/2 + 70, LargeurFenetre/2 + 100, HauteurFenetre/2 + 180);
    couleurCourante(0, 200, 0);
    rectangle(LargeurFenetre/2 - 100, HauteurFenetre/2 - 60, LargeurFenetre/2 + 100, HauteurFenetre/2 + 40);
    couleurCourante(0, 0, 200);
    rectangle(LargeurFenetre/2 - 300, HauteurFenetre/2 + 70, LargeurFenetre/2 - 100, HauteurFenetre/2 + 180);
    couleurCourante(200, 200, 200);
    rectangle(LargeurFenetre/2 + 100, HauteurFenetre/2 + 70, LargeurFenetre/2 + 300, HauteurFenetre/2 + 180);
    couleurCourante(0,0,0);
    rectangle(LargeurFenetre/2 + 100, HauteurFenetre/2 - 60, LargeurFenetre/2 + 300, HauteurFenetre/2 + 40);
    couleurCourante(100,100,200 );
    rectangle(LargeurFenetre/2 + 100, HauteurFenetre/2 - 160, LargeurFenetre/2 + 300, HauteurFenetre/2 - 40);


    // Affichage du texte
    couleurCourante(0, 0, 0);
    epaisseurDeTrait(3);
    afficheChaine(texteAAfficher, 20, LargeurFenetre/2 - 100, HauteurFenetre/2 + 170);
    couleurCourante(100, 250, 250);
    afficheChaine("Affiche regle", 15, LargeurFenetre/2 - 60, HauteurFenetre/2 );
    afficheChaine("Chainage avant", 15, LargeurFenetre/2 - 60, HauteurFenetre/2 - 110);
    afficheChaine("Chainage arriere", 15, LargeurFenetre/2 - 260, HauteurFenetre/2 );
    afficheChaine("Ajouter une regle", 15, LargeurFenetre/2 + 140, HauteurFenetre/2 );
    afficheChaine("Ajouter un Fait", 15, LargeurFenetre/2 - 260, HauteurFenetre/2 - 110);
    afficheChaine("Quitter&sauvegarde", 15, LargeurFenetre/2 + 140 , HauteurFenetre/2 - 110);
    break;

        case Clavier:
            break;

        case ClavierSpecial:
            break;

        case BoutonSouris:
            if (etatBoutonSouris() == GaucheAppuye) {
                int x = abscisseSouris();
                int y = ordonneeSouris();
                if (x >= LargeurFenetre/2 - 100 && x <= LargeurFenetre/2 + 100 && y >= HauteurFenetre/2 - 50 && y <= HauteurFenetre/2 + 50) {
                    strcpy(texteAAfficher, "Bouton 1 clique !");
                } else if (x >= LargeurFenetre/2 - 100 && x <= LargeurFenetre/2 + 100 && y >= HauteurFenetre/2 - 150 && y <= HauteurFenetre/2 - 50) {
                    strcpy(texteAAfficher, "Bouton 2 clique !");
                } else if (x >= LargeurFenetre/2 - 300 && x <= LargeurFenetre/2 - 100 && y >= HauteurFenetre/2 - 50 && y <= HauteurFenetre/2 + 50) {
                    strcpy(texteAAfficher, "Bouton 3 clique !");
                } else if (x >= LargeurFenetre/2 + 100 && x <= LargeurFenetre/2 + 300 && y >= HauteurFenetre/2 - 50 && y <= HauteurFenetre/2 + 50) {
                    
					strcpy(texteAAfficher, "Bouton 4 clique !");
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
