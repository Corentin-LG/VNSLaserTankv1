#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void changeTab(int ***tableau);
void changeTabChar(char ****tableauChar);

int main()
{
    int **tableau;
    char ***tableauChar;
    changeTab(&tableau);
    changeTabChar(&tableauChar);
    printf("sorti\n");
    printf("coo 0,2 %d\n", tableau[0][2]);
    printf("coo 1,2 %d\n", tableau[1][2]);

    printf("sorti char\n");

    printf("tbc coo 0,2 %s\n", tableauChar[0][2]);
    printf("tbc coo 1,2 %s\n", tableauChar[1][2]);

    // Libérer la mémoire de tableau
    for (int i = 0; i < 3; i++)
    {
        free(tableau[i]);
    }
    free(tableau);

    // Libérer la mémoire de tableauChar
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            free((*tableauChar)[i][j]);
        }
        free((*tableauChar)[i]);
    }
    free(*tableauChar);

    return 0;
}

void changeTab(int ***tableau)
{
    *tableau = (int **)malloc(3 * sizeof(int *));
    for (int i = 0; i < 3; i++)
    {
        (*tableau)[i] = (int *)malloc(5 * sizeof(int));
    }
    (*tableau)[0][1] = 2; // Initialisez chaque élément individuellement
    (*tableau)[0][2] = 5;
    (*tableau)[1][1] = 12;
    (*tableau)[1][2] = 15;

    printf("coo 0,1 %d\n", (*tableau)[0][1]);
    printf("coo 1,1 %d\n", (*tableau)[1][1]);
}

void changeTabChar(char ****tableauChar)
{
    *tableauChar = (char ***)malloc(3 * sizeof(char **));
    for (int i = 0; i < 3; i++)
    {
        (*tableauChar)[i] = (char **)malloc(5 * sizeof(char *));
    }
    (*tableauChar)[0][1] = "a";                              // Initialisez chaque élément individuellement avec une chaîne de caractères
    (*tableauChar)[0][2] = (char *)malloc(4 * sizeof(char)); // Allouez de la mémoire pour "bdf"
    strcpy((*tableauChar)[0][2], "bdf");                     // Copiez "bdf" dans (*tableauChar)[0][2]
    (*tableauChar)[1][1] = "c";
    (*tableauChar)[1][2] = "d";

    printf("tbc coo 0,1 %s\n", (*tableauChar)[0][1]);
    printf("tbc coo 1,1 %s\n", (*tableauChar)[1][1]);
    printf("tbc coo 1,2 %s\n", (*tableauChar)[1][2]);
}
