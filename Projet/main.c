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
                printf("Element apr�s la fleche : %s\n", token);
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


#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 1000

void extractVariables(char *rules) {
    int isVariable[26] = {0}; // To track which characters (variables) have been printed
    for (int i = 0; i < strlen(rules); i++) {
        if (isalpha(rules[i])) {
            // Convert the character to its alphabetical index (0 for 'a' or 'A', 1 for 'b' or 'B', etc.)
            int index = tolower(rules[i]) - 'a';
            if (!isVariable[index]) {
                printf("%c ", rules[i]);
                isVariable[index] = 1; // Mark this character as printed
            }
        }
    }
    printf("\n");
}

int main() {
    char rules[MAX_LENGTH] = "b d e -> f; g d -> a; f c -> a; b -> x; e -> d; a x -> h; c -> d; x c -> a; x b -> d;";
    extractVariables(rules);
    return 0;
}
