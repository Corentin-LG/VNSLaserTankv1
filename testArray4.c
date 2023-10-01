#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdbool.h>

void changeTab(int ***tableau);

int main()
{
    int **tableau;
    changeTab(&tableau);
    printf("sorti\n");
    printf("coo 0,2 %d\n", tableau[0][2]);
    printf("coo 1,2 %d\n", tableau[1][2]);

    for (int i = 0; i < 3; i++)
    {
        free(tableau[i]);
    }
    free(tableau);

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
