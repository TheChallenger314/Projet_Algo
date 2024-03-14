#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct Rules
{
    char *conclusion;
    Premise *premise;
    struct Rules *suiv;
}Rules;
typedef struct Premise
{
    char *head;
    struct Premise *suiv;
}Premise;
char *back(char *but,Rules *listr,char *listf)
{
    bool result = false;
    Rules *newrules =(Rules*)malloc(sizeof(Rules)); 
    if(!contain(but,listf))
    {
        newrules = listr;
        while(newrules != NULL && result==false)
        {
            if(newrules->conclusion == but)
        }
    }
}
bool contain(char *result,char *listf)
{
    int longueur = strlen(listf);
    for(int i;i)
    return false;
}