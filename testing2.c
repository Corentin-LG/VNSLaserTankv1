#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <wchar.h>
#include <string.h>

//////////////////////////////////////////////////////////////////
// Global Functions //
int getRandomMove();

//////////////////////////////////////////////////////////////////
// Array Functions //
// depreciated
bool firstTankPosition(int id, int *currentTankDirection);

//////////////////////////////////////////////////////////////////
// Elements Functions //

// isSomething
bool isFloor(int floorID);
bool isMovable(int movableID);
bool isShootable(int shootableID);
bool isUnMovable(int unMovableID);
bool isLegalMove(int **arrayTankCell, int moveID, int **arrayGrid, int nbRows, int nbColumns);

// Ground
bool nextFloor(int **arrayTankCell, int moveID, int **arrayGrid);

// Tank
bool moveTank(int **tankPosition, int testMoveID, int **gridWorked, int **gridGround);

// Grids
void resetGridWorked(int **gridOrigin, int **gridWorked, int numRows, int numCols);
void resetGridGround(int **gridOrigin, int **gridGround, int numRows, int numCols);
void resetGridMovables(int **gridOrigin, int **gridMovables, int numRows, int numCols);
// void resetDeplacement(int **deplacementArray);

// depreci
void moveOnGrid(int **arrayTankCell, int moveID, int **arrayGrid);
bool legalFire(int **arrayTankCell, int moveID, int **arrayGrid);
bool autoKill(int **arrayTankCell, int positionID, int **arrayGrid);
//////////////////////////////////////////////////////////////////
// Reading Functions //
void printArray(int **array, int rows, int cols);
void displayMovingLetters(int *array);

//////////////////////////////////////////////////////////////////
// Structs //
struct ConversionTable
{
    const wchar_t *lettre;
    int valeur;
};

struct ConversionTable tableConversionSimple[] = {
    {L"Tu", 1}, {L"Tright", 2}, {L"Tdown", 3}, {L"Tleft", 4}, {L"D", 5}, {L"b", 6}, {L"w", 7}, {L"Bs", 8}, {L"Bm", 9}, {L"B", 10}, {L"Au", 11}, {L"Ar", 12}, {L"Ad", 13}, {L"Al", 14}, {L"Mur", 15}, {L"Mdr", 16}, {L"Mdl", 17}, {L"Mul", 18}, {L"Wu", 19}, {L"Wr", 20}, {L"Wd", 21}, {L"Wl", 22}, {L"C", 23}, {L"Rur", 24}, {L"Rdr", 25}, {L"Rdl", 26}, {L"Rul", 27}, {L"I", 28}, {L"i", 29}, {L"Tr", 30}, {L"Tg", 31}, {L"Tb", 32}, {L"Tc", 33}, {L"Ty", 34}, {L"Tp", 35}, {L"Tw", 36}, {L"Td", 37}};

struct ConversionTable tableConversionBis[] = {
    {L"Tu\n", 1}, {L"Tright\n", 2}, {L"Tdown\n", 3}, {L"Tleft\n", 4}, {L"D\n", 5}, {L"b\n", 6}, {L"w\n", 7}, {L"Bs\n", 8}, {L"Bm\n", 9}, {L"B\n", 10}, {L"Au\n", 11}, {L"Ar\n", 12}, {L"Ad\n", 13}, {L"Al\n", 14}, {L"Mur\n", 15}, {L"Mdr\n", 16}, {L"Mdl\n", 17}, {L"Mul\n", 18}, {L"Wu\n", 19}, {L"Wr\n", 20}, {L"Wd\n", 21}, {L"Wl\n", 22}, {L"C\n", 23}, {L"Rur\n", 24}, {L"Rdr\n", 25}, {L"Rdl\n", 26}, {L"Rul\n", 27}, {L"I\n", 28}, {L"i\n", 29}, {L"Tr\n", 30}, {L"Tg\n", 31}, {L"Tb\n", 32}, {L"Tc\n", 33}, {L"Ty\n", 34}, {L"Tp\n", 35}, {L"Tw\n", 36}, {L"Td\n", 37}};

//////////////////////////////////////////////////////////////////
// Enums //
enum gameElement
{
    NOTHING,
    TANKUP,
    TANKRIGHT,
    TANKDOWN,
    TANKLEFT,
    DIRT,
    BASE,
    WATER,
    SOLIDBLOCK,
    MOVABLEBLOC,
    BRICKS,
    ANTITANKUP,
    ANTITANKRIGHT,
    ANTITANKDOWN,
    ANTITANKLEFT,
    MIRRORUPRIGHT,
    MIRRORRIGHTDOWN,
    MIRRORDOWNLEFT,
    MIRRORLEFTUP,
    WAYUP,
    WAYRIGHT,
    WAYDOWN,
    WAYLEFT,
    CRYSTALBLOCK,
    ROTATIVEMIRRORUPRIGHT,
    ROTATIVEMIRRORRIGHTDOWN,
    ROTATIVEMIRRORDOWNLEFT,
    ROTATIVEMIRRORLEFTUP,
    ICE,
    THINICE,
    TUNNELRED,
    TUNNELGREEN,
    TUNNELBLUE,
    TUNNELCYAN,
    TUNNELYELLOW,
    TUNNELPINK,
    TUNNELWHITE,
    TUNNELDARK
};

enum gameMoving
{
    FIRE,
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NOMOVE = 404
};

//////////////////////////////////////////////////////////////////
// Main //
int main()
{
    //////////////////////////////////////////////////////////////////
    // Global Var //
    srand(time(NULL));
    setlocale(LC_ALL, "");
    // const char *filename = "Special-I.lt4";
    // const char *filename = "Sokoban-II.lt4";
    // const char *filename = "No_HS-LPB.lt4";
    // const char *filename = "LaserTank.lt4";
    // const char *filename = "Gary-II.lt4";
    // const char *filename = "Challenge-IV.lt4";
    // const char *filename = "Beginner-II.lt4";
    const char *filename = "testing2.lt4";

    printf("%s\n", filename);

    int **tankPosition = (int **)malloc((2) * sizeof(int *));
    for (int i = 0; i < 2; i++)
    {
        tankPosition[i] = (int *)malloc((2) * sizeof(int));
    }

    int **basesPosition = (int **)malloc((5) * sizeof(int *));
    for (int i = 0; i < 5; i++)
    {
        basesPosition[i] = (int *)malloc((2) * sizeof(int));
    }

    int *deplacementsHypothese = (int *)malloc((10000) * sizeof(int));
    int *deplacementsHypotheseAutreMetaHeuristique = (int *)malloc((10000) * sizeof(int));
    int *deplacementsRetenu = (int *)malloc((10000) * sizeof(int));

    int *curseurDeplacementsHypothese = malloc(sizeof(int));
    int *curseurDeplacementsAutreMetaHeuristique = malloc(sizeof(int));
    int *curseurDeplacementsRetenu = malloc(sizeof(int));

    size_t deplacementsSize = sizeof(int) * 10000;
    wchar_t header[10000];
    //////////////////////////////////////////////////////////////////
    // Annexe Var //
    int numRows = 0;
    int numColumns = 0;
    int numBases = 0;
    int *currentTankDirection = 0;
    //////////////////////////////////////////////////////////////////
    // Open File //
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        return 1;
    }

    // Search Lines and Columns//
    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Rows: %d", &numRows) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de lignes\n");
        fclose(file);
        return 1;
    }

    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Cols: %d", &numColumns) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de colonnes\n");
        fclose(file);
        return 1;
    }

    // Initiate Grid //
    int **gridOrigin = (int **)malloc((numRows) * sizeof(int *));
    for (int i = 0; i < numRows; i++)
    {
        gridOrigin[i] = (int *)malloc((numColumns) * sizeof(int));
    }

    int **gridWorked = (int **)malloc((numRows) * sizeof(int *));
    for (int i = 0; i < numRows; i++)
    {
        gridWorked[i] = (int *)malloc((numColumns) * sizeof(int));
    }

    int **gridGround = (int **)malloc((numRows) * sizeof(int *));
    for (int i = 0; i < numRows; i++)
    {
        gridGround[i] = (int *)malloc((numColumns) * sizeof(int));
    }
    for (int i = 0; i < numRows; i++)
    {
        memset(gridGround[i], NOTHING, numColumns * sizeof(int));
    }

    int **gridMovables = (int **)malloc((numRows) * sizeof(int *));
    for (int i = 0; i < numRows; i++)
    {
        gridMovables[i] = (int *)malloc((numColumns) * sizeof(int));
    }
    for (int i = 0; i < numRows; i++)
    {
        memset(gridMovables[i], NOTHING, numColumns * sizeof(int));
    }

    for (int i = 0; i < 5; i++)
    {
        fgetws(header, sizeof(header) / sizeof(header[0]), file);
    }

    // Complete Grid //
    int k = 0;
    wchar_t *token;
    wchar_t *tokenInterm;
    wchar_t *tabConvInterm;
    wchar_t *tabNConvInterm;

    for (int i = 0; i < numRows; i++)
    {
        fgetws(header, sizeof(header) / sizeof(header[0]), file);
        token = wcstok(header, L" ");

        // wprintf(L"En-tête complet : %ls\n", header);
        for (int j = 0; j < numColumns; j++)
        {
            // wprintf(L"Token complet : %ls et plus si affinité\n", token);
            k = 0;
            while (token != NULL)
            {
                tokenInterm = token;
                tabConvInterm = tableConversionSimple[k].lettre;
                tabNConvInterm = tableConversionBis[k].lettre;

                if (wcscmp(tokenInterm, tabConvInterm) == 0 || wcscmp(tokenInterm, tabNConvInterm) == 0)
                {
                    // printf("yes\n");
                    gridOrigin[i][j] = tableConversionSimple[k].valeur;
                    gridWorked[i][j] = tableConversionSimple[k].valeur;
                    if (tableConversionSimple[k].valeur == TANKUP)
                    {
                        tankPosition[0][0] = i;
                        // printf("tu00 = %d; ", tankPosition[0][0]);
                        tankPosition[1][0] = i;
                        // printf("tu10 = %d; ", tankPosition[1][0]);
                        tankPosition[0][1] = j;
                        // printf("tu01 = %d; ", tankPosition[0][1]);
                        tankPosition[1][1] = j;
                        // printf("tu11 = %d\n", tankPosition[1][1]);
                        currentTankDirection = tableConversionSimple[k].valeur;
                        // printf("ctp = %d\n", currentTankDirection);
                        // assume tank spawn on dirt
                        gridGround[i][j] = DIRT;
                    }
                    if (tableConversionSimple[k].valeur == BASE)
                    {
                        basesPosition[numBases][0] = i;
                        basesPosition[numBases][1] = j;
                        numBases = numBases + 1;
                    }
                    if (isFloor(tableConversionSimple[k].valeur))
                    {
                        gridGround[i][j] = tableConversionSimple[k].valeur;
                    }
                    if (isMovable(tableConversionSimple[k].valeur))
                    {
                        gridMovables[i][j] = tableConversionSimple[k].valeur;
                    }
                    break;
                }
                k = k + 1;
                // wprintf(L"%ls\n", token);
            }
            if (token != NULL)
            {
                token = wcstok(NULL, L" ");
            }
        }
    }
    // printArray(tableau, numRows, numColumns);
    fclose(file);
    //////////////////////////////////////////////////////////////////
    // First "Solution" //
    // depreciated

    // int verticalDeplacement = 0;
    // int horizontalDeplacement = 0;
    // horizontalDeplacement = basesPosition[0][1] - tankPosition[0][1]; // deltaColomns
    // verticalDeplacement = basesPosition[0][0] - tankPosition[0][0];   // deltaLignes
    // printf("verticalDeplacement = %d, horizontalDeplacement = %d \n", verticalDeplacement, horizontalDeplacement);
    // printf("basesPosition00 %d; basesPosition01 %d\n", basesPosition[0][0], basesPosition[0][1]);

    // int curseur = 0;
    // // make rotation first
    // if (horizontalDeplacement > 0)
    // {
    //     for (int i = horizontalDeplacement; i >= 0; i--)
    //     {
    //         deplacementsHypothese[curseur] = RIGHT;
    //         deplacementsRetenu[curseur] = RIGHT;
    //         curseur++;
    //     }
    // }
    // else
    // {
    //     for (int i = horizontalDeplacement; i <= 0; i++)
    //     {
    //         deplacementsHypothese[curseur] = LEFT;
    //         deplacementsRetenu[curseur] = LEFT;
    //         curseur++;
    //     }
    // }
    // if (verticalDeplacement > 0)
    // {
    //     for (int i = verticalDeplacement; i >= 0; i--)
    //     {
    //         deplacementsHypothese[curseur] = DOWN;
    //         deplacementsRetenu[curseur] = DOWN;
    //         curseur++;
    //     }
    // }
    // else
    // {
    //     for (int i = verticalDeplacement; i <= 0; i++)
    //     {
    //         deplacementsHypothese[curseur] = UP;
    //         deplacementsRetenu[curseur] = UP;
    //         curseur++;
    //     }
    // }
    // *curseurDeplacementsHypothese = curseur;
    // *curseurDeplacementsRetenu = curseur;
    // displayMovingLetters(deplacementsHypothese);
    // displayMovingLetters(deplacementsRetenu);
    //////////////////////////////////////////////////////////////////
    // Heuristic //
    // 2e grille
    printf("test\n");
    // memset(deplacementsHypothese, -1, deplacementsSize);
    // displayMovingLetters(deplacementsHypothese);

    int curseur = 0;
    int turnNumber = 0;

    // resetGridWorked(gridOrigin, gridWorked, numRows, numColumns);
    // resetGridGround(gridOrigin, gridGround, numRows, numColumns);
    // resetGridMovables(gridOrigin, gridMovables, numRows, numColumns);
    memset(deplacementsHypothese, -1, deplacementsSize);
    // memset(deplacementsRetenu, -1, deplacementsSize);
    // memset(deplacementsHypotheseAutreMetaHeuristique, -1, deplacementsSize);

    while (!(tankPosition[0][0] == basesPosition[0][0] &&
             tankPosition[0][1] == basesPosition[0][1]))
    {
        int testMove = getRandomMove();
        while (!(isLegalMove(tankPosition, testMove, gridWorked, numRows, numColumns)))
        {
            testMove = getRandomMove();
        }

        if (gridWorked[tankPosition[0][0]][tankPosition[0][1]] != testMove)
        {
            gridWorked[tankPosition[0][0]][tankPosition[0][1]] = testMove;
            turnNumber--;
        }
        else
        {
            if (moveTank(tankPosition, testMove, gridWorked, gridGround))
            {
                // printf("yeah nb %d\n", turnNumber);
            }
        }

        deplacementsHypothese[curseur] = testMove;
        deplacementsRetenu[curseur] = testMove;
        
        for (int w = 0; w < curseur; w++)
        {
            printf("%d", *deplacementsHypothese[w]);
        }
        printf("\n");

        curseur++;
        turnNumber++;
        
    }

    // do
    // {
    // resetGridWorked(gridOrigin, gridWorked, numRows, numColumns);
    // resetGridGround(gridOrigin, gridGround, numRows, numColumns);
    // resetGridMovables(gridOrigin, gridMovables, numRows, numColumns);
    // printf("dsdsd\n");
    // turnNumber = 0;
    // curseur = 0;
    // memset(deplacementsHypotheseAutreMetaHeuristique, -1, deplacementsSize);
    // // tant que les positions ne sont pas les mêmes
    // while (!(tankPosition[0][0] == basesPosition[0][0] &&
    //          tankPosition[0][1] == basesPosition[0][1]))
    // {
    //     // je veux faire un move
    //     int testMove = getRandomMove();
    //     // si ce move est possible
    //     // cas pas de case/out of born // relunch
    //     // cas case // c1 c'est dirt/base pour commencer sinon ne rien faire
    //     while (!(isLegalMove(tankPosition, testMove, gridWorked, numRows, numColumns)))
    //     {
    //         testMove = getRandomMove();
    //         printf("dsdsd2\n");
    //     }

    //     if (gridWorked[tankPosition[0][0]][tankPosition[0][1]] != testMove)
    //     {
    //         gridWorked[tankPosition[0][0]][tankPosition[0][1]] = testMove;
    //         turnNumber--;
    //         printf("dsdsd3\n");
    //     }
    //     else
    //     {
    //         if (moveTank(tankPosition, testMove, gridWorked, gridGround))
    //         {
    //             // printf("yeah nb %d\n", turnNumber);
    //             printf("dsdsd4\n");
    //         }
    //     }

    //     // deplacementsHypotheseAutreMetaHeuristique[turnNumber] = testMove;
    //     printf("dsdsd5\n");
    //     curseur++;
    //     turnNumber++;
    // }
    // *curseurDeplacementsAutreMetaHeuristique = curseur;
    // printf("turnNumber = %d; curseur = %d\n", turnNumber, curseur);
    // if (*curseurDeplacementsAutreMetaHeuristique < *curseurDeplacementsHypothese)
    // {
    //     memset(deplacementsHypothese, -1, deplacementsSize);
    //     for (int i = 0; i < sizeof(deplacementsHypothese) / sizeof(deplacementsHypothese[0]); i++)
    //     {
    //         deplacementsHypothese[i] = curseurDeplacementsAutreMetaHeuristique[i];
    //         *curseurDeplacementsAutreMetaHeuristique = *curseurDeplacementsHypothese;
    //     }
    // }
    // } while (!(*curseurDeplacementsAutreMetaHeuristique <= *curseurDeplacementsHypothese)); // wip

    // printf("curseurDeplacementsAutreMetaHeuristique = %d ; curseurDeplacementsHypothese = %d\n", *curseurDeplacementsAutreMetaHeuristique, *curseurDeplacementsHypothese);
    // printf("tankPosition00 %d; tankPosition01 %d; tankPosition10 %d; tankPosition11 %d\n", tankPosition[0][0], tankPosition[0][1], tankPosition[1][0], tankPosition[1][1]);
    // printf("what found\n");
    // displayMovingLetters(deplacementsHypotheseAutreMetaHeuristique);
    // printf("what found\n");
    //////////////////////////////////////////////////////////////////
    // Write Output //

    //////////////////////////////////////////////////////////////////
    // Free Memory //
    if (gridOrigin != NULL)
    {
        printf("gridOrigin = lignes : %d et colonnes : %d\n", numRows, numColumns);
        printArray(gridOrigin, numRows, numColumns);
        for (int i = 0; i < numRows; i++)
        {
            free(gridOrigin[i]);
        }
        free(gridOrigin);
    }

    if (gridWorked != NULL)
    {
        printf("gridWorked = lignes : %d et colonnes : %d\n", numRows, numColumns);
        printArray(gridWorked, numRows, numColumns);
        for (int i = 0; i < numRows; i++)
        {
            free(gridWorked[i]);
        }
        free(gridWorked);
    }

    if (gridGround != NULL)
    {
        printf("gridGround = lignes : %d et colonnes : %d\n", numRows, numColumns);
        printArray(gridGround, numRows, numColumns);
        for (int i = 0; i < numRows; i++)
        {
            free(gridGround[i]);
        }
        free(gridGround);
    }

    if (gridMovables != NULL)
    {
        printf("gridMovables = lignes : %d et colonnes : %d\n", numRows, numColumns);
        printArray(gridMovables, numRows, numColumns);
        for (int i = 0; i < numRows; i++)
        {
            free(gridMovables[i]);
        }
        free(gridMovables);
    }

    if (tankPosition != NULL)
    {
        printf("tankp\n");
        printArray(tankPosition, 2, 2);
        for (int i = 0; i < 2; i++)
        {
            free(tankPosition[i]);
        }
        free(tankPosition);
    }

    if (basesPosition != NULL)
    {
        printf("basesp\n");
        printArray(basesPosition, numBases, 2);
        for (int i = 0; i < 5; i++)
        {
            free(basesPosition[i]);
        }
        free(basesPosition);
    }
    printf("meta\n");
    displayMovingLetters(deplacementsHypotheseAutreMetaHeuristique);
    free(deplacementsHypotheseAutreMetaHeuristique);
    printf("hyp\n");
    displayMovingLetters(deplacementsHypothese);
    free(deplacementsHypothese);
    printf("retenu\n");
    displayMovingLetters(deplacementsRetenu);
    free(deplacementsRetenu);

    free(curseurDeplacementsHypothese);
    free(curseurDeplacementsAutreMetaHeuristique);
    free(curseurDeplacementsRetenu);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Reading Functions //
void printArray(int **array, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }
}

void displayMovingLetters(int *array)
{
    int i = 0;
    while (array[i] > 0 && array[i] <= 5)
    {
        switch (array[i])
        {
        case FIRE:
            printf("F");
            break;
        case UP:
            printf("U");
            break;
        case RIGHT:
            printf("R");
            break;
        case DOWN:
            printf("D");
            break;
        case LEFT:
            printf("L");
            break;
        default:
            printf("X%d ", array[i]);
            break;
        }
        i++;
    }
    printf("\n");
}

void firstHeuristique(int **tank, int **bases, int baseNumber)
{
}

//////////////////////////////////////////////////////////////////
// Array Functions //
bool firstTankPosition(int id, int *currentTankDirection)
{
    switch (id)
    {
    case TANKUP:
        *currentTankDirection = TANKUP;
        return true;
    case TANKRIGHT:
        *currentTankDirection = TANKRIGHT;
        return true;
    case TANKDOWN:
        *currentTankDirection = TANKDOWN;
        return true;
    case TANKLEFT:
        *currentTankDirection = TANKLEFT;
        return true;
    default:
        return false;
    }
}

//////////////////////////////////////////////////////////////////
// Global Functions //
int getRandomMove()
{
    // int randomNumber = rand() % 5;
    // // enum 1 à 5 intéressant donc faire 0 à 4 +1
    // randomNumber++;
    // // printf("randomNumber = %d\n", randomNumber);

    int randomNumber = rand() % 5;
    printf("randomNumber = %d\n", randomNumber);
    return randomNumber;
}

//////////////////////////////////////////////////////////////////
// Elements Functions //
bool isLegalMove(int **arrayTankCell, int moveID, int **arrayGrid, int nbRows, int nbColumns)
{
    // il faut trouver si oui ou non, le tank peut se déplacer
    // printf("movID %d\n", moveID);
    switch (moveID)
    {
    case FIRE:
        // printf("F");
        return true;
    case UP:
        // printf("[UP]\n");
        if (arrayTankCell[0][0] == 0)
        {
            return false;
        }
        else
        {
            if (nextFloor(arrayTankCell, moveID, arrayGrid))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    case RIGHT:
        // printf("[RIGHT]\n");
        if (arrayTankCell[0][1] == nbColumns - 1)
        {
            return false;
        }
        else
        {
            if (nextFloor(arrayTankCell, moveID, arrayGrid))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    case DOWN:
        // printf("[DOWN]\n");
        if (arrayTankCell[0][0] == nbRows - 1)
        {
            return false;
        }
        else
        {
            if (nextFloor(arrayTankCell, moveID, arrayGrid))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    case LEFT:
        // printf("[LEFT]\n");
        if (arrayTankCell[0][1] == 0)
        {
            return false;
        }
        else
        {
            if (nextFloor(arrayTankCell, moveID, arrayGrid))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    default:
        printf("illegalMove = %d\n", moveID);
        return false;
    }
    return false;
}

bool legalFire(int **arrayTankCell, int moveID, int **arrayGrid)
{
    // il faut trouver si oui ou non, le tank peut faire son tire
}

bool autoKill(int **arrayTankCell, int positionID, int **arrayGrid)
{
    // il faut trouver si oui ou non, le tank peut faire son tire
}

void moveOnGrid(int **arrayTankCell, int moveID, int **arrayGrid)
{
    // faire le déplacement
}

bool nextFloor(int **arrayTankCell, int moveID, int **arrayGrid)
{
    // arrayGrid[arrayTankCell[0][0]-1][arrayTankCell[0][1]]
    switch (moveID)
    {
    case FIRE:
        // printf("F");
        return true;
    case UP:
        if (isFloor(arrayGrid[arrayTankCell[0][0] - 1][arrayTankCell[0][1]]))
        {
            // printf("yes Up\n");
            return true;
        }
        else
        {
            return false;
        }
    case RIGHT:
        if (isFloor(arrayGrid[arrayTankCell[0][0]][arrayTankCell[0][1] + 1]))
        {
            // printf("yes Right\n");
            return true;
        }
        else
        {
            return false;
        }
    case DOWN:
        if (isFloor(arrayGrid[arrayTankCell[0][0] + 1][arrayTankCell[0][1]]))
        {
            // printf("yes Down\n");
            return true;
        }
        else
        {
            return false;
        }
    case LEFT:
        if (isFloor(arrayGrid[arrayTankCell[0][0]][arrayTankCell[0][1] - 1]))
        {
            // printf("yes Left\n");
            return true;
        }
        else
        {
            return false;
        }
    default:
        printf("X%d ", moveID);
        return false;
    }
    return false;
}

bool isFloor(int floorID)
{
    switch (floorID)
    {
    case DIRT:
        return true;
    case BASE:
        return true;
    case WAYUP:
        return true;
    case WAYRIGHT:
        return true;
    case WAYDOWN:
        return true;
    case WAYLEFT:
        return true;
    case ICE:
        return true;
    case THINICE:
        return true;
    case TUNNELRED:
        return true;
    case TUNNELGREEN:
        return true;
    case TUNNELBLUE:
        return true;
    case TUNNELCYAN:
        return true;
    case TUNNELYELLOW:
        return true;
    case TUNNELPINK:
        return true;
    case TUNNELWHITE:
        return true;
    case TUNNELDARK:
        return true;
    default:
        // printf("other floor %d\n", floorID);
        return false;
    }
    return false;
}

bool isMovable(int movableID)
{
    switch (movableID)
    {
    case MOVABLEBLOC:
        return false;
    case ANTITANKUP:
        return false;
    case ANTITANKRIGHT:
        return false;
    case ANTITANKDOWN:
        return false;
    case ANTITANKLEFT:
        return false;
    default:
        // printf("other movable %d\n", movableID);
        return false;
    }
    return false;
}

bool isShootable(int shootableID)
{
    switch (shootableID)
    {
    case MOVABLEBLOC:
        return true;
    case BRICKS:
        return true;
    case ANTITANKUP:
        return true;
    case ANTITANKRIGHT:
        return true;
    case ANTITANKDOWN:
        return true;
    case ANTITANKLEFT:
        return true;
    case MIRRORUPRIGHT:
        return true;
    case MIRRORRIGHTDOWN:
        return true;
    case MIRRORDOWNLEFT:
        return true;
    case MIRRORLEFTUP:
        return true;
    case CRYSTALBLOCK:
        return true;
    case ROTATIVEMIRRORUPRIGHT:
        return true;
    case ROTATIVEMIRRORRIGHTDOWN:
        return true;
    case ROTATIVEMIRRORDOWNLEFT:
        return true;
    case ROTATIVEMIRRORLEFTUP:
        return true;
    default:
        // printf("other shootable %d\n", shootableID);
        return false;
    }
    return false;
}

bool isUnMovable(int unMovableID)
{
    switch (unMovableID)
    {
    case SOLIDBLOCK:
        return true;
    case BRICKS:
        return true;
    case MIRRORUPRIGHT:
        return true;
    case MIRRORRIGHTDOWN:
        return true;
    case MIRRORDOWNLEFT:
        return true;
    case MIRRORLEFTUP:
        return true;
    case CRYSTALBLOCK:
        return true;
    case ROTATIVEMIRRORUPRIGHT:
        return true;
    case ROTATIVEMIRRORRIGHTDOWN:
        return true;
    case ROTATIVEMIRRORDOWNLEFT:
        return true;
    case ROTATIVEMIRRORLEFTUP:
        return true;
    default:
        // printf("other movable %d\n", unMovableID);
        return false;
    }
    return false;
}

bool moveTank(int **tankPosition, int testMoveID, int **gridWorked, int **gridGround)
{
    switch (testMoveID)
    {
    case FIRE:
        return true;
    case UP:
        gridWorked[tankPosition[0][0]][tankPosition[0][1]] = gridGround[tankPosition[0][0]][tankPosition[0][1]];
        tankPosition[0][0] = tankPosition[0][0] - 1;
        gridWorked[tankPosition[0][0]][tankPosition[0][1]] = UP;
        return true;
    case RIGHT:
        gridWorked[tankPosition[0][0]][tankPosition[0][1]] = gridGround[tankPosition[0][0]][tankPosition[0][1]];
        tankPosition[0][1] = tankPosition[0][1] + 1;
        gridWorked[tankPosition[0][0]][tankPosition[0][1]] = RIGHT;
        return true;
    case DOWN:
        gridWorked[tankPosition[0][0]][tankPosition[0][1]] = gridGround[tankPosition[0][0]][tankPosition[0][1]];
        tankPosition[0][0] = tankPosition[0][0] + 1;
        gridWorked[tankPosition[0][0]][tankPosition[0][1]] = DOWN;
        return true;
    case LEFT:
        gridWorked[tankPosition[0][0]][tankPosition[0][1]] = gridGround[tankPosition[0][0]][tankPosition[0][1]];
        tankPosition[0][1] = tankPosition[0][1] - 1;
        gridWorked[tankPosition[0][0]][tankPosition[0][1]] = LEFT;
        return true;
    }
    return false;
}

void resetGridWorked(int **gridOrigin, int **gridWorked, int numRows, int numCols)
{
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            gridWorked[i][j] = gridOrigin[i][j];
        }
    }
}

void resetGridGround(int **gridOrigin, int **gridGround, int numRows, int numCols)
{
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            gridGround[i][j] = 0;
            if (isFloor(gridOrigin[i][j]))
            {
                gridGround[i][j] = gridOrigin[i][j];
            }
            if (gridOrigin[i][j] == 1)
            {
                gridGround[i][j] = 5;
            }
        }
    }
}

void resetGridMovables(int **gridOrigin, int **gridMovables, int numRows, int numCols)
{
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            gridMovables[i][j] = 0;
            if (isMovable(gridOrigin[i][j]))
            {
                gridMovables[i][j] = gridOrigin[i][j];
            }
        }
    }
}