#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <wchar.h>
#include <string.h>

//////////////////////////////////////////////////////////////////
// Global Functions //
int randomMove();

//////////////////////////////////////////////////////////////////
// Array Functions //
bool firstTankPosition(int id, int *currentTankDirection);

//////////////////////////////////////////////////////////////////
// Elements Functions //
bool legalMove(int **arrayTankCell, int moveID, int **arrayGrid, int nbRows, int nbColumns);
bool nextFloor(int **arrayTankCell, int moveID, int **arrayGrid);
bool legalFire(int **arrayTankCell, int moveID, int **arrayGrid);
bool autoKill(int **arrayTankCell, int positionID, int **arrayGrid);
void moveOnGrid(int **arrayTankCell, int moveID, int **arrayGrid);
bool isFloor(int floorID);
bool isMovable(int movableID);
bool isShootable(int shootableID);
bool isUnMovable(int unMovableID);

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

struct ConversionTable tableConversion[] = {
    {L"Tu", 1}, {L"Tright", 2}, {L"Tdown", 3}, {L"Tleft", 4}, {L"D", 5}, {L"b", 6}, {L"w", 7}, {L"Bs", 8}, {L"Bm", 9}, {L"B", 10}, {L"Au", 11}, {L"Ar", 12}, {L"Ad", 13}, {L"Al", 14}, {L"Mur", 15}, {L"Mdr", 16}, {L"Mdl", 17}, {L"Mul", 18}, {L"Wu", 19}, {L"Wr", 20}, {L"Wd", 21}, {L"Wl", 22}, {L"C", 23}, {L"Rur", 24}, {L"Rdr", 25}, {L"Rdl", 26}, {L"Rul", 27}, {L"I", 28}, {L"i", 29}, {L"Tr", 30}, {L"Tg", 31}, {L"Tb", 32}, {L"Tc", 33}, {L"Ty", 34}, {L"Tp", 35}, {L"Tw", 36}, {L"Td", 37}};

struct ConversionTable tableNConversion[] = {
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
    NOMOVE,
    FIRE,
    UP,
    RIGHT,
    DOWN,
    LEFT
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

    int *deplacementsHypothese = (int *)malloc((1000) * sizeof(int));
    int *deplacementsRetenu = (int *)malloc((1000) * sizeof(int));

    size_t deplacementsSize = sizeof(int) * 1000;
    wchar_t header[100000];
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
    int **gridOrigin = (int **)malloc((numRows + 1) * sizeof(int *));
    for (int i = 0; i < numRows; i++)
    {
        gridOrigin[i] = (int *)malloc((numColumns + 1) * sizeof(int));
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
                tabConvInterm = tableConversion[k].lettre;
                tabNConvInterm = tableNConversion[k].lettre;

                if (wcscmp(tokenInterm, tabConvInterm) == 0 || wcscmp(tokenInterm, tabNConvInterm) == 0)
                {
                    // printf("yes\n");
                    gridOrigin[i][j] = tableConversion[k].valeur;
                    gridWorked[i][j] = tableConversion[k].valeur;
                    if (tableConversion[k].valeur == TANKUP)
                    {
                        tankPosition[0][0] = i;
                        printf("tu00 = %d; ", tankPosition[0][0]);
                        tankPosition[1][0] = i;
                        printf("tu10 = %d; ", tankPosition[1][0]);
                        tankPosition[0][1] = j;
                        printf("tu01 = %d; ", tankPosition[0][1]);
                        tankPosition[1][1] = j;
                        printf("tu11 = %d\n", tankPosition[1][1]);
                        currentTankDirection = tableConversion[k].valeur + 1;
                        printf("ctp = %d\n", currentTankDirection);
                    }
                    if (tableConversion[k].valeur == BASE)
                    {
                        basesPosition[numBases][0] = i;
                        basesPosition[numBases][1] = j;
                        numBases = numBases + 1;
                    }
                    if (isFloor(tableConversion[k].valeur))
                    {
                        gridGround[i][j] = tableConversion[k].valeur;
                    }
                    if (isMovable(tableConversion[k].valeur))
                    {
                        gridMovables[i][j] = tableConversion[k].valeur;
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

    int verticalDeplacement = 0;
    int horizontalDeplacement = 0;
    horizontalDeplacement = basesPosition[0][1] - tankPosition[0][1]; // deltaColomns
    verticalDeplacement = basesPosition[0][0] - tankPosition[0][0];   // deltaLignes
    printf("v = %d, h = %d \n", verticalDeplacement, horizontalDeplacement);
    printf("b00 %d; b01 %d\n", basesPosition[0][0], basesPosition[0][1]);
    int curseur = 0;
    if (verticalDeplacement > 0)
    {
        for (int i = verticalDeplacement; i > 0; i--)
        {
            deplacementsHypothese[curseur] = DOWN;
            deplacementsRetenu[curseur] = DOWN;
            curseur++;
        }
    }
    else
    {
        for (int i = verticalDeplacement; i < 0; i++)
        {
            deplacementsHypothese[curseur] = UP;
            deplacementsRetenu[curseur] = UP;
            curseur++;
        }
    }
    if (horizontalDeplacement > 0)
    {
        for (int i = horizontalDeplacement; i > 0; i--)
        {
            deplacementsHypothese[curseur] = RIGHT;
            deplacementsRetenu[curseur] = RIGHT;
            curseur++;
        }
    }
    else
    {
        for (int i = horizontalDeplacement; i < 0; i++)
        {
            deplacementsHypothese[curseur] = LEFT;
            deplacementsRetenu[curseur] = LEFT;
            curseur++;
        }
    }
    displayMovingLetters(deplacementsHypothese);
    displayMovingLetters(deplacementsRetenu);
    //////////////////////////////////////////////////////////////////
    // Heuristic //
    // 2e grille
    printf("test\n");
    memset(deplacementsHypothese, -1, deplacementsSize);
    displayMovingLetters(deplacementsHypothese);

    // printf("rand =%d\n", randomMove());
    int turnNumber = 0;
    // tant que les positions ne sont pas les mêmes
    // while (!(tankPosition[0][0] == basesPosition[0][0] &&
    //        tankPosition[0][1] == basesPosition[0][1]))
    // {
    //     // je veux faire un move
    //     int testMove = randomMove();
    //     // si ce move est possible
    //     // cas pas de case/out of born // relunch
    //     // cas case // c1 c'est dirt/base pour commencer sinon ne rien faire

    //     // le faire
    //     // sinon retester un move différent
    //     turnNumber++;
    // }

    // test
    //  bool tankValid = firstTankPosition(1, &currentTankDirection);
    //  printf("cr = %d, truc =%d\n", currentTankDirection, tankValid);
    //  tankValid = firstTankPosition(10, &currentTankDirection);
    //  printf("cr = %d, truc =%d\n", currentTankDirection, tankValid);
    printf("t00 %d; t01 %d; t10 %d; t11 %d\n", tankPosition[0][0], tankPosition[0][1], tankPosition[1][0], tankPosition[1][1]);
    printf("test2\n");

    // bool tankValid = legalMove(&tankPosition, 2, &gridWorked, numRows, numColumns);
    //  si vrai ajouter le move à la liste

    // printf("valid = %d\n", tankValid);
    printf("t00 %d; t01 %d; t10 %d; t11 %d\n", tankPosition[0][0], tankPosition[0][1], tankPosition[1][0], tankPosition[1][1]);

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
        printf("tankp");
        printArray(tankPosition, 2, 2);
        for (int i = 0; i < 2; i++)
        {
            free(tankPosition[i]);
        }
        free(tankPosition);
    }

    if (basesPosition != NULL)
    {
        printf("basesp");
        printArray(basesPosition, numBases, 2);
        for (int i = 0; i < 5; i++)
        {
            free(basesPosition[i]);
        }
        free(basesPosition);
    }

    free(deplacementsHypothese);
    free(deplacementsRetenu);

    return 0;
}

//////////////////////////////////////////////////////////////////
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
int randomMove()
{
    int randomNumber = rand() % 5;
    // enum 1 à 5 intéressant donc faire 0 à 4 +1
    randomNumber++;
    printf("randomNumber = %d\n", randomNumber);
    return randomNumber;
}

//////////////////////////////////////////////////////////////////
// Elements Functions //
bool legalMove(int **arrayTankCell, int moveID, int **arrayGrid, int nbRows, int nbColumns)
{
    // il faut trouver si oui ou non, le tank peut se déplacer
    switch (moveID)
    {
    case FIRE:
        printf("F ");
        return true;
        break;
    case UP:
        printf("LegalM\n");
        // upMove()
        if (arrayTankCell[0][0] == 0)
        {
            return false;
            break;
        }
        else
        {
            // arrayGrid[arrayTankCell[0][0]-1][arrayTankCell[0][1]]//cible
            // grille, coo atterrissage, -> à appliquer, avoir la grille de modifsS
            if (nextFloor(&arrayTankCell, moveID, &arrayGrid))
            {
                // faut mais c'est le début
                arrayTankCell[0][0] = arrayTankCell[0][0] - 1;
                arrayGrid[arrayTankCell[0][0] - 1][arrayTankCell[0][1]] = 1;
                // arrayTankCell[0][0] = arrayTankCell[0][0]
                //  faudra comparer avec les autres grilles
                return true;
                break;
            }
            else
            {
                // arrayGrid[arrayTankCell[0][0]-1][arrayTankCell[0][1]]//cible
                printf("truc = %d", arrayTankCell[0][0]);
                return false;
                break;
            }
            printf("bidule = %d", arrayTankCell[0][0]);
            return true;
            break;
        }
        break;
    case RIGHT:
        printf("R");
        return true;
        break;
    case DOWN:
        printf("D");
        return true;
        break;
    case LEFT:
        printf("L");
        return true;
        break;
    default:
        printf("X");
        return false;
        break;
    }
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
        printf("F");
        return true;
        break;
    case UP:
        printf("nextF\n");
        // xt+1 détecte case type
        // sub function return
        if (isFloor(arrayGrid[arrayTankCell[0][0] - 1][arrayTankCell[0][1]]))
        {
            //
            return true;
        }
        else
        {
            return false;
        }

        break;
    case RIGHT:
        printf("R");
        return true;
        break;
    case DOWN:
        printf("D");
        return true;
        break;
    case LEFT:
        printf("L");
        return true;
        break;
    default:
        printf("X%d ", moveID);
        return false;
        break;
    }
    return false;
}

bool isFloor(int floorID)
{
    switch (floorID)
    {
    case DIRT:
        return true;
        break;
    case BASE:
        return true;
        break;
    case WAYUP:
        return true;
        break;
    case WAYRIGHT:
        return true;
        break;
    case WAYDOWN:
        return true;
        break;
    case WAYLEFT:
        return true;
        break;
    case ICE:
        return true;
        break;
    case THINICE:
        return true;
        break;
    case TUNNELRED:
        return true;
        break;
    case TUNNELGREEN:
        return true;
        break;
    case TUNNELBLUE:
        return true;
        break;
    case TUNNELCYAN:
        return true;
        break;
    case TUNNELYELLOW:
        return true;
        break;
    case TUNNELPINK:
        return true;
        break;
    case TUNNELWHITE:
        return true;
        break;
    case TUNNELDARK:
        return true;
        break;
    default:
        printf("other floor %d\n", floorID);
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
        break;
    case ANTITANKUP:
        return false;
        break;
    case ANTITANKRIGHT:
        return false;
        break;
    case ANTITANKDOWN:
        return false;
        break;
    case ANTITANKLEFT:
        return false;
        break;
    default:
        printf("other movable %d\n", movableID);
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
        break;
    case BRICKS:
        return true;
        break;
    case ANTITANKUP:
        return true;
        break;
    case ANTITANKRIGHT:
        return true;
        break;
    case ANTITANKDOWN:
        return true;
        break;
    case ANTITANKLEFT:
        return true;
        break;
    case MIRRORUPRIGHT:
        return true;
        break;
    case MIRRORRIGHTDOWN:
        return true;
        break;
    case MIRRORDOWNLEFT:
        return true;
        break;
    case MIRRORLEFTUP:
        return true;
        break;
    case CRYSTALBLOCK:
        return true;
        break;
    case ROTATIVEMIRRORUPRIGHT:
        return true;
        break;
    case ROTATIVEMIRRORRIGHTDOWN:
        return true;
        break;
    case ROTATIVEMIRRORDOWNLEFT:
        return true;
        break;
    case ROTATIVEMIRRORLEFTUP:
        return true;
        break;
    default:
        printf("other shootable %d\n", shootableID);
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
        break;
    case BRICKS:
        return true;
        break;
    case MIRRORUPRIGHT:
        return true;
        break;
    case MIRRORRIGHTDOWN:
        return true;
        break;
    case MIRRORDOWNLEFT:
        return true;
        break;
    case MIRRORLEFTUP:
        return true;
        break;
    case CRYSTALBLOCK:
        return true;
        break;
    case ROTATIVEMIRRORUPRIGHT:
        return true;
        break;
    case ROTATIVEMIRRORRIGHTDOWN:
        return true;
        break;
    case ROTATIVEMIRRORDOWNLEFT:
        return true;
        break;
    case ROTATIVEMIRRORLEFTUP:
        return true;
        break;
    default:
        printf("other movable %d\n", unMovableID);
        return false;
    }
    return false;
}