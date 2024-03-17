#include "expert_system.h"

int main(int argc, char *argv[]) {
    Fait* faits = NULL;
    Regle* regles = NULL;

    initialiseGfx(argc, argv);
    chargerReglesDepuisFichier(&regles);
    initialisation();
    gererSaisieUtilisateur(&faits, &regles);
    prepareFenetreGraphique("Projet de moteur d'inference", LargeurFenetre, HauteurFenetre);
    lanceBoucleEvenements();
    libererFaits(faits);
    libererRegles(regles);

    return 0;
}
