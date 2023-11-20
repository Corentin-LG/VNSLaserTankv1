#include <stdio.h>

void supprimerEntreFours(int tableau[], int *curseur);
void supprimer2(int tableau[], int *curseur);

int main()
{
    // int tableau[] = {1, 2, 0, 4, 5, 0, 0, 8, 9, 0, 11, 12, 13, -1, -1};
    // res = {1, 2, 0, 5, 0, 0, 9, 0, 13}

    // int tableau[] = {1, 2, 3, 0, 5, 6, 7, 8, 9, 0, 11, 12, 13, 14, 15, 16, 17, 0, 19, 20, 21};
    // res = {1, 3, 0, 9, 0, 17, 0, 21}

    // int tableau[] = {0, 0, 3, 4, 0, 0, 0, 6, 0, 0, 0};
    // res = {0, 0, 4, 0, 0, 0, 6, 0, 0, 0}

    // int tableau[] = {0, 2, 3, 0, 2, 3, 2, 2, 4, 0, 0, 1, 1, 2, 2, 3, 3, 0, 3, 1, 0};
    // res = {0, 3, 0, 2, 2, 4, 0, 0, 1, 1, 2, 2, 3, 3, 0, 3, 1, 0}

    int tableau[] = {0, 0, 1, 1, 1, 2, 3, 0, 3, 3, 2, 2, 0, 0, 0, 2, 1, 2, 2, 3, 4, 0, 3, 1, 4, 4, 0, 2, 0, 1, 1, 0, 3, 2, 3, 3};
    // res = {0, 0, 1, 1, 1, 3, 0, 3, 3, 2, 2, 0, 0, 0, 2, 2, 4, 0, 4, 0, 2, 0, 1, 1, 0, 3}

    int curseur = sizeof(tableau) / sizeof(tableau[0]);
    int *ptrCurseur = &curseur; // Adresse de la variable curseur
    // Appel de la fonction pour supprimer les nombres entre deux 4
    supprimer2(tableau, ptrCurseur);

    // Affichage du tableau après la suppression
    for (int i = 0; i < curseur; i++)
    {
        printf("%d ", tableau[i]);
    }

    return 0;
}

void supprimerEntreFours(int tableau[], int *curseur)
{
    int startCell = 0;
    int lastFireCell = 0;  // rank
    int lastDirection = 1; // up
    int readingHead = 0;

    for (int i = startCell; i < *curseur; i++)
    {
        printf("|i=%d; startCell =%d; *curseur = %d\n", i, startCell, *curseur);
        if (startCell == *curseur)
        {
            printf("|breek\n");
            break;
        }
        else if (tableau[i] == 0 && lastFireCell == 0)
        {
            lastFireCell = i + 1; // rank
            printf("|lastFireCell=%d __", lastFireCell);
        }
        else if (tableau[i] == 0 && lastFireCell != 0)
        {
            for (int j = 0; j <= *curseur - (i - 1); j++)
            {
                if (i - lastFireCell == 0)
                {
                    lastFireCell = lastFireCell + 1;
                    goto skip;
                }
                if (i - lastFireCell == 1)
                {
                    lastFireCell = 0;
                    goto skip;
                }
                tableau[lastFireCell + j] = tableau[i - 1 + j];
                printf("|j =%d; tableau[%d] =%d __", j, lastFireCell + j, tableau[lastFireCell + j]);
            }
            printf("|curseur%d _ ", *curseur);
            *curseur = lastFireCell + (*curseur - (i - 1));
            printf("|lastFireCell + (*curseur - (i - 1)=%d __", *curseur);
            i = lastFireCell;
            printf("|lastFireCell %d __", lastFireCell);
            lastFireCell = 0;
            startCell = i;
        }
    skip:
    }
}

void supprimer2(int tableau[], int *curseur)
{
    int startCell = 0;
    int lastFireCell = 0;  // rank
    int lastDirection = 1; // up
    int readingHead = 0;
    // int tableau[] = {0, 2, 3, 0, 2, 3, 2, 2, 4, 0, 0, 1, 1, 2, 2, 3, 3, 0, 3, 1, 0};
    // res = {0, 3, 0, 2, 2, 4, 0, 0, 1, 1, 2, 2, 3, 3, 0, 3, 1, 0}
    for (int i = 0; i < *curseur; i++)
    {
        printf("\n|i=%d; *curseur = %d\n", i, *curseur);
        if (i == *curseur)
        {
            printf("|breek\n");
            break;
        }
        else if (tableau[i] == 0 && lastFireCell == 0)
        {
            lastFireCell = i + 1; // rank
            printf("|lF1=%d __", lastFireCell);
        }
        else if (tableau[i] == 0 && lastFireCell != 0)
        {
            if (i - lastFireCell == 0)
            {
                lastFireCell = lastFireCell + 1;
                printf("|lF2=%d __", lastFireCell);
                goto skip;
            }
            if (i - lastFireCell == 1)
            {
                lastFireCell = 0;
                printf("|lF3=%d __", lastFireCell);
                goto skip;
            }
        }
        else if (tableau[i] != 0)
        {
            if (tableau[i] == lastDirection)
            {
                // reset
                lastFireCell = 0;
                printf("|lF4=%d __", lastFireCell);
                // for i -> i+1 next
            }
            else if (tableau[i + 1] != 0 && tableau[i + 1] != lastDirection && tableau[i + 1] != tableau[i])
            {
                // implicite: tableau[i] != lastDirection
                if (lastFireCell != 0)
                {
                    for (int j = 0; j < *curseur - 1; j++)
                    {
                        // erase
                        tableau[lastFireCell + j] = tableau[lastFireCell + 1 + j];
                    }
                }
                else
                {
                    for (int j = 0; j < *curseur - 1; j++)
                    {
                        // erase
                        tableau[i + j] = tableau[i + 1 + j];
                    }
                }
                *curseur = *curseur - 1;
                printf("|c1=%d __", *curseur);
                i = i - 1;
                printf("|i1=%d __", i);
                for (int k = 0; k < *curseur; k++)
                {
                    printf("%d ", tableau[k]);
                }
            }
            else if (tableau[i + 1] == 0)
            {
                // turn
                lastDirection = tableau[i];
                printf("|ld1=%d __", lastDirection);
                // reset
                lastFireCell = 0;
                printf("|lF5=%d __", lastFireCell);
            }
            else if (tableau[i + 1] == lastDirection)
            {
                if (tableau[i + 2] == lastDirection)
                {
                    /* code */
                    printf("|bruh __");
                }
                else if (tableau[i + 2] != lastDirection)
                {
                    // delete 2
                    for (int j = 0; j < *curseur - 2; j++)
                    {
                        // erase
                        tableau[i + j] = tableau[i + 2 + j];
                    }
                    *curseur = *curseur - 2;
                    printf("|c2=%d __", *curseur);
                    i = i - 1; //
                    printf("|i2=%d __", i);
                }
            }
            else if (tableau[i + 1] == tableau[i])
            {
                // turn
                lastDirection = tableau[i];
                printf("|ld2=%d __", lastDirection);
                // reset
                lastFireCell = 0;
                printf("|lF6=%d __", lastFireCell);
            }
        }
    skip:
    }
}
