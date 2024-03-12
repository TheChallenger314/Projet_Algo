#include <stdio.h>
#include <stdlib.h>
FILE* ouverture_fichier()
{
    FILE* fichier = fopen("regle.txt","r");
    return fichier;
}
void test2()
{
    FILE* fichier = ouverture_fichier();
    char ligne[100];
    fgets(ligne, sizeof(ligne), fichier);
    fputs(ligne, stdout);
    fclose(fichier);
}
void affiche_fichier()
{
    char buffer[50];
    size_t octets_lus;
    octets_lus = fread(buffer, 1, sizeof(buffer), ouverture_fichier());
    fwrite(buffer, 1, octets_lus, stdout);
}
void test3() {
    FILE *fichier = fopen("regle.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char *token = strtok(ligne, "->");
        while (token != NULL) {
            char *sous_token = strtok(token, ",");
            while (sous_token != NULL) {
                printf("Element : %s\n", sous_token);
                sous_token = strtok(NULL, ",");
            }
            token = strtok(NULL, "->");
            if (token == NULL) {
                printf("Element après la fleche : %s\n", token);
            }
        }
    }
    fclose(fichier);
}
int main()
{
    affiche_fichier();
    test2();
    test3();
    return 0;
}
