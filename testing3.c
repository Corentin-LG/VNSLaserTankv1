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

bool tankAction(int **gridOrigin, int **gridWorked, int **gridMovables,
                int **gridGround, int **gridWorkedCopy, int **gridGroundCopy, int **gridMovablesCopy,
                int *numRows, int *numColumns,
                int **tankPosition, int **basesPosition, int **firePosition,
                int *currentTankDirection, int *currentFireDirection,
                int *curseurDeplacementsHypothese, int *curseurDeplacementsRetenu, int *curseurDeplacementsMH,
                int *objectiveFunctionMH,
                int *testMove, int *curseur)
{
    bool fireDead = false;
    int firedTileID = 0;
    if (testMove == 0)
    {
        firePosition[0][0] = tankPosition[0][0];
        firePosition[0][1] = tankPosition[0][1];
        firePosition[1][0] = tankPosition[0][0];
        firePosition[1][1] = tankPosition[0][1];

        *currentTankDirection = gridWorked[tankPosition[0][0]][tankPosition[0][1]];
        *currentFireDirection = *currentTankDirection;
        getFirstShootNextCoo(tankPosition, firePosition, currentTankDirection);
        
        fireDead = true;
        if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
        {
            firedTileID = gridWorked[firePosition[0][0]][firePosition[0][1]];
            fireDead = false;
        }
        else
        {
            return false;
        }

        while (!(isOutOfBorder(firePosition, 0, numRows, numColumns) || isFireStop(firedTileID) || fireDead))
        {
            fireDead = false;

            if (firePosition[0][0] == tankPosition[0][0] && firePosition[0][1] == tankPosition[0][1])
            {
                return false;
            }
            else if (isFireTrought(firedTileID))
            {
                getFirstShootNextCoo(firePosition, firePosition, currentFireDirection);
                fireDead = false;
                goto nextFirePosition;
            }
            else if (isShootable(firedTileID, currentFireDirection))
            {
                shotableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns);
                if (antiTankAction(tankPosition, 0, gridWorked, numRows, numColumns))
                {
                    mirror3Grids(gridWorkedCopy, gridWorked, gridMovablesCopy, gridMovables, gridGroundCopy, gridGround, numRows, numColumns);
                    fireDead = false;
                    return false;
                }
                fireDead = true;
                return true;
            }
            else
            {
                resetGridWorked(gridOrigin, gridWorked, numRows, numColumns);
                resetGridWorked(gridOrigin, gridWorkedCopy, numRows, numColumns);
                fireDead = true;
                return false;
            }
        nextFirePosition:
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns) && !fireDead)
            {
                firedTileID = gridWorked[firePosition[0][0]][firePosition[0][1]];
                fireDead = false;
            }
            else
            {
                return false;
            }
        }
    return true;
    }
}