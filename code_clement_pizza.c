#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_INGREDIENTS 10
#define MAX_LINE_LENGTH 256
#define MAX_PIZZAS 20
char *filename = "pizza.kbs";
typedef struct {
    char name[50];
    char ingredients[MAX_INGREDIENTS][50];
    int ingredientCount;
} Pizza;

void toLowerCase(char *str) {
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

void trimNewline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

int loadPizzas(Pizza pizzas[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open the file");
        exit(1);
    }

    int pizzaCount = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) && pizzaCount < MAX_PIZZAS) {
        trimNewline(line); // Remove newline character

        char *ingredientPart = strtok(line, "->");
        char *namePart = strtok(NULL, ";");
        if (ingredientPart && namePart) {
            toLowerCase(ingredientPart);
            toLowerCase(namePart);
            trimNewline(namePart); // Additional trim in case of spaces around '->'

            // Process ingredients
            char *ingredientToken = strtok(ingredientPart, " ");
            int i = 0;
            while (ingredientToken && i < MAX_INGREDIENTS) {
                strcpy(pizzas[pizzaCount].ingredients[i], ingredientToken);
                trimNewline(pizzas[pizzaCount].ingredients[i]); // Trim potential spaces
                toLowerCase(pizzas[pizzaCount].ingredients[i]); // Ensure lowercase
                ingredientToken = strtok(NULL, " ");
                i++;
            }
            pizzas[pizzaCount].ingredientCount = i;

            // Save pizza name
            strcpy(pizzas[pizzaCount].name, namePart);
            trimNewline(pizzas[pizzaCount].name); // Trim potential spaces
            toLowerCase(pizzas[pizzaCount].name); // Ensure lowercase

            pizzaCount++;
        }
    }

    fclose(file);
    return pizzaCount;
}

void findPizzaByIngredients(Pizza pizzas[], int pizzaCount) {
    printf("Enter ingredients (separated by spaces): ");
    char input[256];
    scanf(" %[^\n]", input); // Lire une ligne entière, incluant les espaces
    toLowerCase(input);

    char *searchIngredients[MAX_INGREDIENTS];
    int nIngredients = 0;
    char *token = strtok(input, ",");
    while (token != NULL && nIngredients < MAX_INGREDIENTS) {
        searchIngredients[nIngredients++] = token;
        token = strtok(NULL, ",");
    }

    printf("Pizzas containing all specified ingredients:\n");
    for (int i = 0; i < pizzaCount; i++) {
        bool containsAll = true;
        for (int j = 0; j < nIngredients; j++) {
            bool ingredientFound = 0;
            for (int k = 0; k < pizzas[i].ingredientCount; k++) {
                char ingredientLower[50];
                strcpy(ingredientLower, pizzas[i].ingredients[k]);
                toLowerCase(ingredientLower);
                if (strstr(ingredientLower, searchIngredients[j]) != NULL) {
                    ingredientFound = 1;
                    break;
                }
            }
            if (!ingredientFound) {
                containsAll = 0;
                break;
            }
        }
        if (containsAll) {
            printf("%s\n", pizzas[i].name);
        }
    }
}


void findIngredientsByPizzaName(Pizza pizzas[], int pizzaCount) {
    char searchName[50];
    printf("Enter pizza name: ");
    scanf("%49s", searchName);
    getchar(); // Consommer le caractère de nouvelle ligne restant
    toLowerCase(searchName);

    for (int i = 0; i < pizzaCount; i++) {
        char pizzaNameLower[50];
        strcpy(pizzaNameLower, pizzas[i].name);
        toLowerCase(pizzaNameLower);

        if (strcmp(pizzaNameLower, searchName) == 0) {
            printf("Ingredients for '%s': ", pizzas[i].name);
            for (int j = 0; j < pizzas[i].ingredientCount; j++) {
                printf("%s", pizzas[i].ingredients[j]);
                if (j < pizzas[i].ingredientCount - 1) {
                    printf(", ");
                }
            }
            printf("\n");
            return;
        }
    }
    printf("Pizza not found.\n");
}


int main() {
    Pizza pizzas[MAX_PIZZAS];
    int pizzaCount = loadPizzas(pizzas);

    int choice;
    do {
        printf("\n1. Find pizza by ingredients\n2. Find ingredients by pizza name\n0. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character to avoid skipping inputs

        switch (choice) {
            case 1:
                findPizzaByIngredients(pizzas, pizzaCount);
                break;
            case 2:
                findIngredientsByPizzaName(pizzas, pizzaCount);
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 0);

    return 0;
}
