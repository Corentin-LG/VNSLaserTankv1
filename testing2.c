#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <wchar.h>
#include <string.h>
// !! wayURDR as floor for movables
//////////////////////////////////////////////////////////////////
// Global Functions //

int getRandomMove();
//////////////////////////////////////////////////////////////////
// Array Functions //

//////////////////////////////////////////////////////////////////
// Elements Functions //

// isSomething
// grid

bool isFloor(int elementID);
bool isOutOfBorder(int **objectPosition, int objectCoo, int *numRows, int *numColumns);
// tank

bool isLegalMove(int **arrayTankCell, int tankCoo, int moveID, int **arrayGrid, int *nbRows, int *nbColumns);
bool onFirstHighWay(int **tankPosition, int moveID, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
bool isHighWay(int elementID);
// elements

bool isMovable(int elementID, int *positionID);
bool isMovableAtBeginning(int elementID);
bool isShootable(int elementID, int *positionID);
bool isTurnable(int elementID, int *positionID);
bool isDeathDestination(int curentTile);
bool nextHighWay(int **arrayTankCell, int tankCoo, int moveID, int **arrayGrid);
// fire

bool isFireTrought(int elementID);
bool isFireDeflect(int elementID, int *positionID);
bool isFireStop(int elementID);
bool antiTankAction(int **tankPosition, int **gridWorked, int *numRows, int *numColumns);

void getFirstShootNextCoo(int **tankPosition, int **firePosition, int *currentTankDirection);
void shotableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
// Ground

bool nextFloor(int **arrayTankCell, int tankCoo, int moveID, int **arrayGrid);
// Tank

bool moveTank(int **tankPosition, int tankCoo, int testMoveID, int **gridWorked, int **gridGround);
void mirrorPosition(int **tankPosition, int fromCoo, int toCoo);
// Movables

bool movableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
// Reflectables

bool deflectableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
bool turnableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
// Grids

void resetGridWorked(int **gridOrigin, int **gridWorked, int *numRows, int *numCols);
void resetGridGround(int **gridOrigin, int **gridGround, int *numRows, int *numCols);
void resetGridMovables(int **gridOrigin, int **gridMovables, int **gridGround, int *numRows, int *numCols);
void mirrorGrid(int **gridFrom, int **gridTo, int *numRows, int *numCols);
// depreci

bool autoKill(int **arrayTankCell, int positionID, int **arrayGrid);
//////////////////////////////////////////////////////////////////
// Reading Functions //

void printArray(int **array, int *rows, int *cols);
void print2Array(int **array1, int **array2, int *rows, int *cols);
void print3Array(int **array1, int **array2, int **array3, int *rows, int *cols);
void printArrayBraket(int **array1, int *rows, int *cols, int bRow, int bCol);
void print2ArrayBraket(int **array1, int **array2, int *rows, int *cols, int bRow, int bCol);
void print3ArrayBraket(int **array1, int **array2, int **array3, int *rows, int *cols, int bRow, int bCol);
void printLittleArray(int **array, int rows, int cols);
void printBaseArray(int **array, int *rows, int cols);
void printMovingLetters(int *array, int *curseur);
void printMovingLettersWithoutPointor(int *array, int curseur);
int replayDeplacements(char deplacementLetter);
void erazeUselessTurn(int *vector, int *curseur);
//////////////////////////////////////////////////////////////////
// Structs //
struct ConversionTable
{
    const wchar_t *lettre;
    int valeur;
};

struct ConversionTable tableConversionSimple[] = {
    {L"Tu", 1}, {L"Tright", 2}, {L"Tdown", 3}, {L"Tleft", 4}, {L"D", 5}, {L"b", 6}, {L"w", 7}, {L"Bs", 8}, {L"Bm", 9}, {L"B", 10}, {L"Au", 11}, {L"Ar", 12}, {L"Ad", 13}, {L"Al", 14}, {L"Mur", 15}, {L"Mdr", 16}, {L"Mdl", 17}, {L"Mul", 18}, {L"Wu", 19}, {L"Wr", 20}, {L"Wd", 21}, {L"Wl", 22}, {L"C", 23}, {L"Rur", 24}, {L"Rdr", 25}, {L"Rdl", 26}, {L"Rul", 27}, {L"I", 28}, {L"i", 29}, {L"Tr", 30}, {L"Tg", 31}, {L"Tb", 32}, {L"Tc", 33}, {L"Ty", 34}, {L"Tp", 35}, {L"Tw", 36}, {L"Td", 37}, {L"Aud", 38}, {L"Ard", 39}, {L"Add", 40}, {L"Ald", 41}, {L"wF", 42}};

struct ConversionTable tableConversionBis[] = {
    {L"Tu\n", 1}, {L"Tright\n", 2}, {L"Tdown\n", 3}, {L"Tleft\n", 4}, {L"D\n", 5}, {L"b\n", 6}, {L"w\n", 7}, {L"Bs\n", 8}, {L"Bm\n", 9}, {L"B\n", 10}, {L"Au\n", 11}, {L"Ar\n", 12}, {L"Ad\n", 13}, {L"Al\n", 14}, {L"Mur\n", 15}, {L"Mdr\n", 16}, {L"Mdl\n", 17}, {L"Mul\n", 18}, {L"Wu\n", 19}, {L"Wr\n", 20}, {L"Wd\n", 21}, {L"Wl\n", 22}, {L"C\n", 23}, {L"Rur\n", 24}, {L"Rdr\n", 25}, {L"Rdl\n", 26}, {L"Rul\n", 27}, {L"I\n", 28}, {L"i\n", 29}, {L"Tr\n", 30}, {L"Tg\n", 31}, {L"Tb\n", 32}, {L"Tc\n", 33}, {L"Ty\n", 34}, {L"Tp\n", 35}, {L"Tw\n", 36}, {L"Td\n", 37}, {L"Aud\n", 38}, {L"Ard\n", 39}, {L"Add\n", 40}, {L"Ald\n", 41}, {L"wF\n", 42}};

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
    TUNNELDARK,
    ANTITANKUPDEAD,
    ANTITANKRIGHTDEAD,
    ANTITANKDOWNDEAD,
    ANTITANKLEFTDEAD,
    WATERFULL
};

enum gameMoving
{
    FIRE,
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NOMOVE = 66
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Main //
int main()
{
    //////////////////////////////////////////////////////////////////
    // Global Var //
    srand(time(NULL));
    setlocale(LC_ALL, "");
    // const char *filename = ".\\Grids\\Special-I.lt4";
    // const char *filename = ".\\Grids\\Sokoban-II.lt4";
    // const char *filename = ".\\Grids\\No_HS-LPB.lt4";
    // const char *filename = ".\\Grids\\LaserTank.lt4";
    // const char *filename = ".\\Grids\\Gary-II.lt4";
    // const char *filename = ".\\Grids\\Challenge-IV.lt4";
    // const char *filename = ".\\Grids\\Beginner-II.lt4";
    const char *filename = ".\\TestingGrids\\testing7.lt4";
    const int CYCLES = 5;

    printf("%s\n", filename);

    int **tankPosition = (int **)malloc((2) * sizeof(int *));
    for (int i = 0; i < 3; i++)
    {
        tankPosition[i] = (int *)malloc((2) * sizeof(int));
    }

    int **basesPosition = (int **)malloc((5) * sizeof(int *));
    for (int i = 0; i < 5; i++)
    {
        basesPosition[i] = (int *)malloc((2) * sizeof(int));
    }

    int **firePosition = (int **)malloc((2) * sizeof(int *));
    for (int i = 0; i < 2; i++)
    {
        firePosition[i] = (int *)malloc((2) * sizeof(int));
    }

    int *deplacementsHypothese = (int *)malloc((1000000) * sizeof(int));
    int *deplacementsHypotheseMH = (int *)malloc((1000000) * sizeof(int));
    int *deplacementsRetenu = (int *)malloc((1000000) * sizeof(int));

    int *curseurDeplacementsHypothese = (int *)malloc(sizeof(int));
    int *curseurDeplacementsMH = (int *)malloc(sizeof(int));
    int *curseurDeplacementsRetenu = (int *)malloc(sizeof(int));

    int *objectiveFunctionHypothese = (int *)malloc(sizeof(int));
    int *objectiveFunctionMH = (int *)malloc(sizeof(int));
    int *objectiveFunctionRetenu = (int *)malloc(sizeof(int));

    size_t deplacementsSize = sizeof(int) * 1000000;
    wchar_t header[1000];
    //////////////////////////////////////////////////////////////////
    // Annexe Var //

    int *numRows = (int *)malloc(sizeof(int));
    int *numColumns = (int *)malloc(sizeof(int));
    int *numBases = (int *)malloc(sizeof(int));

    int *currentTankDirection = (int *)malloc(sizeof(int));
    int *currentFireDirection = (int *)malloc(sizeof(int));

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
    if (swscanf(header, L"Rows: %d", numRows) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de lignes\n");
        fclose(file);
        return 1;
    }

    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Cols: %d", numColumns) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de colonnes\n");
        fclose(file);
        return 1;
    }

    // Initiate Grid //
    int **gridOrigin = (int **)malloc((*numRows) * sizeof(int *));
    for (int i = 0; i < *numRows; i++)
    {
        gridOrigin[i] = (int *)malloc((*numColumns) * sizeof(int));
    }
    for (int i = 0; i < *numRows; i++)
    {
        memset(gridOrigin[i], NOTHING, *numColumns * sizeof(int));
    }

    int **gridWorked = (int **)malloc((*numRows) * sizeof(int *));
    for (int i = 0; i < *numRows; i++)
    {
        gridWorked[i] = (int *)malloc((*numColumns) * sizeof(int));
    }
    for (int i = 0; i < *numRows; i++)
    {
        memset(gridWorked[i], NOTHING, (*numColumns) * sizeof(int));
    }

    int **gridWorkedCopy = (int **)malloc((*numRows) * sizeof(int *));
    for (int i = 0; i < *numRows; i++)
    {
        gridWorkedCopy[i] = (int *)malloc((*numColumns) * sizeof(int));
    }
    for (int i = 0; i < *numRows; i++)
    {
        memset(gridWorkedCopy[i], NOTHING, (*numColumns) * sizeof(int));
    }

    int **gridGround = (int **)malloc((*numRows) * sizeof(int *));
    for (int i = 0; i < *numRows; i++)
    {
        gridGround[i] = (int *)malloc((*numColumns) * sizeof(int));
    }
    for (int i = 0; i < *numRows; i++)
    {
        memset(gridGround[i], NOTHING, (*numColumns) * sizeof(int));
    }

    int **gridGroundCopy = (int **)malloc((*numRows) * sizeof(int *));
    for (int i = 0; i < *numRows; i++)
    {
        gridGroundCopy[i] = (int *)malloc((*numColumns) * sizeof(int));
    }
    for (int i = 0; i < *numRows; i++)
    {
        memset(gridGroundCopy[i], NOTHING, (*numColumns) * sizeof(int));
    }

    int **gridMovables = (int **)malloc((*numRows) * sizeof(int *));
    for (int i = 0; i < *numRows; i++)
    {
        gridMovables[i] = (int *)malloc((*numColumns) * sizeof(int));
    }
    for (int i = 0; i < *numRows; i++)
    {
        memset(gridMovables[i], NOTHING, (*numColumns) * sizeof(int));
    }

    int **gridMovablesCopy = (int **)malloc((*numRows) * sizeof(int *));
    for (int i = 0; i < *numRows; i++)
    {
        gridMovablesCopy[i] = (int *)malloc((*numColumns) * sizeof(int));
    }
    for (int i = 0; i < *numRows; i++)
    {
        memset(gridMovablesCopy[i], NOTHING, (*numColumns) * sizeof(int));
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

    for (int i = 0; i < *numRows; i++)
    {
        fgetws(header, sizeof(header) / sizeof(header[0]), file);
        token = wcstok(header, L" ");

        for (int j = 0; j < *numColumns; j++)
        {
            k = 0;
            while (token != NULL)
            {
                tokenInterm = token;
                tabConvInterm = tableConversionSimple[k].lettre;
                tabNConvInterm = tableConversionBis[k].lettre;

                if (wcscmp(tokenInterm, tabConvInterm) == 0 || wcscmp(tokenInterm, tabNConvInterm) == 0)
                {
                    gridOrigin[i][j] = tableConversionSimple[k].valeur;
                    gridWorked[i][j] = tableConversionSimple[k].valeur;
                    if (tableConversionSimple[k].valeur == TANKUP)
                    {
                        tankPosition[0][0] = i;
                        tankPosition[0][1] = j;
                        mirrorPosition(tankPosition, 0, 1);
                        mirrorPosition(tankPosition, 0, 2);
                        // assume tank spawn on dirt
                        gridGround[i][j] = DIRT;
                    }
                    if (tableConversionSimple[k].valeur == BASE)
                    {
                        basesPosition[*numBases][0] = i;
                        basesPosition[*numBases][1] = j;
                        *numBases = *numBases + 1;
                    }
                    if (isFloor(tableConversionSimple[k].valeur))
                    {
                        gridGround[i][j] = tableConversionSimple[k].valeur;
                    }
                    if (isMovableAtBeginning(tableConversionSimple[k].valeur))
                    {
                        gridMovables[i][j] = tableConversionSimple[k].valeur;
                        gridGround[i][j] = DIRT;
                    }
                    break;
                }
                k = k + 1;
            }
            if (token != NULL)
            {
                token = wcstok(NULL, L" ");
            }
        }
    }
    fclose(file);

    printf("lignes = %d, colonnes = %d, bases = %d\n", *numRows, *numColumns, *numBases);
    printf("o\n");
    printArrayBraket(gridOrigin, numRows, numColumns, tankPosition[0][0], tankPosition[0][1]);
    printf("w\n");
    printArrayBraket(gridWorked, numRows, numColumns, tankPosition[0][0], tankPosition[0][1]);
    printf("m\n");
    printArrayBraket(gridMovables, numRows, numColumns, tankPosition[0][0], tankPosition[0][1]);
    printf("g\n");
    printArrayBraket(gridGround, numRows, numColumns, tankPosition[0][0], tankPosition[0][1]);
    printf("b\n");
    printBaseArray(basesPosition, numBases, 2);
    printf("t\n");
    printLittleArray(tankPosition, 3, 2);
    printf("f\n");
    printLittleArray(firePosition, 2, 2);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // First "Solution" //
    printf("FirstSolution\n");

    int curseur = 0;
    int turnNumber = 0;

    // here, if blanc it's cool
    for (int w = 0; w < curseur; w++)
    {
        printf("%d|\n", deplacementsHypothese[w]);
    }
    printf("\n");
    memset(deplacementsHypothese, -1, deplacementsSize);
    for (int w = 0; w < curseur; w++)
    {
        printf("%d|\n", deplacementsHypothese[w]);
    }
    printf("go first\n");

    *curseurDeplacementsHypothese = 0;
    *curseurDeplacementsRetenu = 0;
    printf("curserH %d ; curserR %d\n", *curseurDeplacementsHypothese, *curseurDeplacementsRetenu);
    ///////////////////////////
    // MHeuristique
    bool fireDead = false;
    int intermEphemere = 0;
    int firedTileID = 0;
    *currentTankDirection = gridWorked[tankPosition[0][0]][tankPosition[0][1]];

    do
    {
        // reset grid
        resetGridWorked(gridOrigin, gridWorked, numRows, numColumns);
        resetGridGround(gridOrigin, gridGround, numRows, numColumns);
        resetGridMovables(gridOrigin, gridMovables, gridGround, numRows, numColumns);
        resetGridWorked(gridOrigin, gridWorkedCopy, numRows, numColumns);
        resetGridGround(gridOrigin, gridGroundCopy, numRows, numColumns);
        resetGridMovables(gridOrigin, gridMovablesCopy, gridGround, numRows, numColumns);
        // reset cmpt
        turnNumber = 0;
        curseur = 0;
        *curseurDeplacementsMH = 0;
        *objectiveFunctionMH = 0;
        // reset HP
        memset(deplacementsHypotheseMH, -1, deplacementsSize);
        // reset Tank
        mirrorPosition(tankPosition, 2, 0);
        mirrorPosition(tankPosition, 2, 1);
        // stop if tank == base
        // while
        while (!(tankPosition[0][0] == basesPosition[0][0] &&
                 tankPosition[0][1] == basesPosition[0][1]))
        {
            int testMove = getRandomMove();

            if (testMove == FIRE)
            {
                // printf("fire cursor = %d\n\n", curseur);
                // reset position at origin
                firePosition[0][0] = tankPosition[0][0];
                firePosition[0][1] = tankPosition[0][1];
                firePosition[1][0] = tankPosition[0][0];
                firePosition[1][1] = tankPosition[0][1];

                // set aim direction
                *currentTankDirection = gridWorked[tankPosition[0][0]][tankPosition[0][1]];
                *currentFireDirection = *currentTankDirection;

                // position + 1
                getFirstShootNextCoo(tankPosition, firePosition, currentTankDirection);
                // if (turnNumber > 50) {
                //     goto bugtest;
                // }
                //         turnNumber++; //sup
                fireDead = true;
                if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
                {
                    firedTileID = gridWorked[firePosition[0][0]][firePosition[0][1]];
                    // printf("ft %d tn %d\n", firedTileID, turnNumber);
                    fireDead = false;
                }

                while (!(isOutOfBorder(firePosition, 0, numRows, numColumns) || isFireStop(firedTileID) || fireDead))
                {
                    fireDead = false;
                    if (isFireTrought(firedTileID))
                    {
                        // printf("throught %d\n", turnNumber);
                        getFirstShootNextCoo(firePosition, firePosition, currentFireDirection);
                        fireDead = false;
                        goto nextFirePosition;
                    }
                    // or explode
                    else if (isShootable(firedTileID, currentFireDirection))
                    {
                        printf("bang , cursor = %d\n", curseur);
                        shotableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns);
                        if (antiTankAction(tankPosition, gridWorked, numRows, numColumns))
                        {
                            mirrorGrid(gridWorkedCopy, gridWorked, numRows, numColumns);
                            mirrorGrid(gridMovablesCopy, gridMovables, numRows, numColumns);
                            mirrorGrid(gridGroundCopy, gridGround, numRows, numColumns);
                            fireDead = false;
                            goto nextFirePosition;
                        }
                        fireDead = true;
                        goto nextFirePosition;
                    }
                    // or move
                    else if (isMovable(firedTileID, currentFireDirection) || firedTileID == MOVABLEBLOC)
                    {
                        switch (firedTileID)
                        {
                        case MOVABLEBLOC:
                        case MIRRORUPRIGHT:
                        case MIRRORRIGHTDOWN:
                        case MIRRORDOWNLEFT:
                        case MIRRORLEFTUP:
                            printf("move !!! , cursor = %d\n", curseur);
                            printf("f00 = %d, f01 %d, forientation = %d\n", firePosition[0][0], firePosition[0][1], *currentFireDirection);
                            if (movableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                            {
                                printf("moved ok \n");
                            }
                            else
                            {
                                printf("moved NO \n");
                            }
                            break;
                        case ANTITANKUP:
                        case ANTITANKRIGHT:
                        case ANTITANKDOWN:
                        case ANTITANKLEFT:
                            printf("move at ! , cursor = %d\n", curseur);
                            printf("f00 = %d, f01 %d, forientation = %d\n", firePosition[0][0], firePosition[0][1], *currentFireDirection);
                            if (movableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                            {
                                printf("moved ok \n");
                            }
                            else
                            {
                                printf("moved NO \n");
                            }
                            break;
                        default:
                            printf("ismov firID %d\n", firedTileID);
                            break;
                        }
                        if (antiTankAction(tankPosition, gridWorked, numRows, numColumns))
                        {
                            mirrorGrid(gridWorkedCopy, gridWorked, numRows, numColumns);
                            mirrorGrid(gridMovablesCopy, gridMovables, numRows, numColumns);
                            mirrorGrid(gridGroundCopy, gridGround, numRows, numColumns);
                            fireDead = false;
                            goto nextFirePosition;
                        }
                        fireDead = true;
                        goto nextFirePosition;
                    }
                    // or deflect
                    else if (isFireDeflect(firedTileID, currentFireDirection))
                    {
                        printf("deflected, cursor = %d\n", curseur);
                        if (deflectableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                        {
                            printf("deflected ok \n");
                        }
                        else
                        {
                            printf("deflected NO \n");
                        }
                        fireDead = false; // only to test :-> erase
                        goto nextFirePosition;
                    }
                    // or turn
                    else if (isTurnable(firedTileID, currentFireDirection))
                    {
                        printf("turned, cursor = %d\n", curseur);
                        if (turnableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                        {
                            printf("turned ok \n");
                        }
                        else
                        {
                            printf("turned NO \n");
                        }
                        fireDead = true; // only to test :-> erase
                        goto nextFirePosition;
                    }
                    // or error...
                    else
                    {
                        printf("elif fireTiled firedTileID=%d ; currentFireDirection=%d\n", firedTileID, *currentFireDirection);
                        printf("elif fp00=%d ; fp01=%d\n", firePosition[0][0], firePosition[0][1]);
                        resetGridWorked(gridOrigin, gridWorked, numRows, numColumns);
                        resetGridWorked(gridOrigin, gridWorkedCopy, numRows, numColumns);
                        printArray(gridWorked, numRows, numColumns);
                        fireDead = true;
                        goto nextFirePosition;
                    }
                nextFirePosition:
                    // printf("endFire %d\n", turnNumber);
                    if (!isOutOfBorder(firePosition, 0, numRows, numColumns) && !fireDead)
                    {
                        // printf("NotOut fp00 = %d, fp01 = %d, cursor = %d\n", firePosition[0][0], firePosition[0][1], curseur);
                        firedTileID = gridWorked[firePosition[0][0]][firePosition[0][1]];
                        fireDead = false;
                    }
                    else
                    {
                        // printf("\nyou're finished :\n");
                        // printf("Out fp00 = %d, fp01 = %d, cursor = %d\n\n", firePosition[0][0], firePosition[0][1], curseur);
                    }
                }
                deplacementsHypotheseMH[curseur] = testMove;
                curseur++;
                turnNumber++;
                *objectiveFunctionMH = *objectiveFunctionMH - 2;
            }
            else if (gridWorked[tankPosition[0][0]][tankPosition[0][1]] != testMove && testMove != FIRE)
            {
                // printf("turn cursor = %d\n\n", curseur);
                gridWorked[tankPosition[0][0]][tankPosition[0][1]] = testMove;
                *currentTankDirection = gridWorked[tankPosition[0][0]][tankPosition[0][1]];
                deplacementsHypotheseMH[curseur] = testMove;
                turnNumber--;
                curseur++;
                turnNumber++;
            }
            else
            {
                printf("move cursor = %d\n\n", curseur);
                // here cases with ice, way, enemy, tunnel
                // wip
                // if n+1 ok
                if (isLegalMove(tankPosition, 0, testMove, gridWorked, numRows, numColumns))
                {
                    // mirror to begin
                    mirrorGrid(gridWorked, gridWorkedCopy, numRows, numColumns);
                    mirrorGrid(gridMovables, gridMovablesCopy, numRows, numColumns);
                    mirrorGrid(gridGround, gridGroundCopy, numRows, numColumns);
                    mirrorPosition(tankPosition, 0, 1);
                    printf("begin hiw\n");
                    // check n+1 = H
                    if (nextHighWay(tankPosition, 1, testMove, gridWorked))
                    {
                        // ok so move on H
                        if (moveTank(tankPosition, 1, testMove, gridWorked, gridGround))
                        {
                            // you r on way so what way ?
                            if (onFirstHighWay(tankPosition, testMove, gridWorked, gridMovables, gridGround, numRows, numColumns))
                            {
                                if (antiTankAction(tankPosition, gridWorked, numRows, numColumns))
                                {
                                    goto deadHighWay;
                                }
                                mirrorPosition(tankPosition, 1, 0);
                                printf("testmove %d\n", testMove);
                                printMovingLettersWithoutPointor(deplacementsHypotheseMH, curseur); // bug
                                deplacementsHypotheseMH[curseur] = testMove;
                                curseur++;
                                printMovingLettersWithoutPointor(deplacementsHypotheseMH, curseur);
                                turnNumber++;
                                *objectiveFunctionMH = *objectiveFunctionMH - 1;
                                gridWorked[tankPosition[0][0]][tankPosition[0][1]] = testMove;
                                *currentTankDirection = testMove;
                                printf("currentTankDirection %d\n", *currentTankDirection);
                                goto afterHighWay;
                            }
                            else
                            {
                            // forget all
                            deadHighWay:
                                mirrorGrid(gridWorkedCopy, gridWorked, numRows, numColumns);
                                mirrorGrid(gridMovablesCopy, gridMovables, numRows, numColumns);
                                mirrorGrid(gridGroundCopy, gridGround, numRows, numColumns);
                                mirrorPosition(tankPosition, 0, 1);
                                goto afterHighWay;
                            }
                        }
                    }
                    else
                    {
                        if (moveTank(tankPosition, 0, testMove, gridWorked, gridGround))
                        {
                            deplacementsHypotheseMH[curseur] = testMove;
                            curseur++;
                            turnNumber++;
                            *objectiveFunctionMH = *objectiveFunctionMH - 1;
                        }
                    }
                }
                else
                {
                    // printf("nonlegal %d\n", turnNumber);
                }
            }
        afterHighWay:
        nextAction:
            mirrorGrid(gridWorked, gridWorkedCopy, numRows, numColumns);
            mirrorGrid(gridMovables, gridMovablesCopy, numRows, numColumns);
            mirrorGrid(gridGround, gridGroundCopy, numRows, numColumns);
        }
        *curseurDeplacementsMH = curseur;
        printf("2curserMH %d ; 2curserH %d ; turnNB %d\n", *curseurDeplacementsMH, *curseurDeplacementsHypothese, turnNumber);

    } while (*curseurDeplacementsMH > 500 && turnNumber < 500);
    // > 20 not fast at all, 30 -> 5 seconds
    // while (curseurDeplacementsMH > curseurDeplacementsHypothese);
    // bugtest:
    *curseurDeplacementsMH = curseur;
    printf("curserHM %d\n", *curseurDeplacementsMH);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Degrossissement
    printf("hyp\n");
    printMovingLetters(deplacementsHypotheseMH, curseurDeplacementsMH);
    erazeUselessTurn(deplacementsHypotheseMH, curseurDeplacementsMH);
    printMovingLetters(deplacementsHypotheseMH, curseurDeplacementsMH);

    // //////////////////////////////////////////////////////////////////
    // // Replay //
    // resetGridWorked(gridOrigin, gridWorked, numRows, numColumns);
    // resetGridMovables(gridOrigin, gridMovables, gridGround, numRows, numColumns);
    // resetGridGround(gridOrigin, gridGround, numRows, numColumns);
    // mirrorGrid(gridWorked, gridWorkedCopy, numRows, numColumns);
    // mirrorGrid(gridMovables, gridMovablesCopy, numRows, numColumns);
    // mirrorGrid(gridGround, gridGroundCopy, numRows, numColumns);
    // mirrorPosition(tankPosition, 2, 0);
    // mirrorPosition(tankPosition, 2, 1);
    // *objectiveFunctionMH = 0;
    // *currentTankDirection = 1;
    // int lastDirection2 = UP;
    // int testMove2;
    // // ko
    // for (int curseur2 = 0; curseur2 < *curseurDeplacementsMH; curseur2++)
    // {
    //     testMove2 = deplacementsHypotheseMH[curseur2];
    //     if (testMove2 == FIRE)
    //     {
    //         firePosition[0][0] = tankPosition[0][0];
    //         firePosition[0][1] = tankPosition[0][1];
    //         firePosition[1][0] = tankPosition[0][0];
    //         firePosition[1][1] = tankPosition[0][1];
    //         printf("curseur2=%d; bf td=%d; gW:T0=%d\n", curseur2, *currentTankDirection, gridWorked[tankPosition[0][0]][tankPosition[0][1]]);
    //         *currentTankDirection = gridWorked[tankPosition[0][0]][tankPosition[0][1]];
    //         *currentFireDirection = *currentTankDirection;
    //         getFirstShootNextCoo(tankPosition, firePosition, currentTankDirection);
    //         fireDead = true;
    //         if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
    //         {
    //             firedTileID = gridWorked[firePosition[0][0]][firePosition[0][1]];
    //             fireDead = false;
    //         }
    //         while (!(isOutOfBorder(firePosition, 0, numRows, numColumns) || isFireStop(firedTileID) || fireDead))
    //         {
    //             fireDead = false;
    //             if (isFireTrought(firedTileID))
    //             {
    //                 getFirstShootNextCoo(firePosition, firePosition, currentFireDirection);
    //                 fireDead = false;
    //                 goto nextFirePosition2;
    //             }
    //             else if (isShootable(firedTileID, currentFireDirection))
    //             {
    //                 shotableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridGround, numRows, numColumns);
    //                 fireDead = true;
    //                 goto nextFirePosition2;
    //             }
    //             // or move
    //             else if (isMovable(firedTileID, currentFireDirection) || firedTileID == MOVABLEBLOC)
    //             {
    //                 switch (firedTileID)
    //                 {
    //                 case MOVABLEBLOC:
    //                 case MIRRORUPRIGHT:
    //                 case MIRRORRIGHTDOWN:
    //                 case MIRRORDOWNLEFT:
    //                 case MIRRORLEFTUP:
    //                     movableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns);
    //                     break;
    //                 default:
    //                     printf("ismov firID %d\n", firedTileID);
    //                     break;
    //                 }
    //                 fireDead = true;
    //                 goto nextFirePosition2;
    //             }
    //             else if (isFireDeflect(firedTileID, currentFireDirection))
    //             {
    //                 deflectableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns);
    //                 fireDead = false;
    //                 goto nextFirePosition2;
    //             }
    //             else if (isTurnable(firedTileID, currentFireDirection))
    //             {
    //                 turnableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns);
    //                 fireDead = true;
    //                 goto nextFirePosition2;
    //             }
    //             else
    //             {
    //                 printf("elif fireTiled firedTileID=%d ; currentFireDirection=%d\n", firedTileID, *currentFireDirection);
    //                 printf("elif fp00=%d ; fp01=%d\n", firePosition[0][0], firePosition[0][1]);
    //                 resetGridWorked(gridOrigin, gridWorked, numRows, numColumns);
    //                 resetGridWorked(gridOrigin, gridWorkedCopy, numRows, numColumns);
    //                 printArray(gridWorked, numRows, numColumns);
    //                 fireDead = true;
    //                 goto nextFirePosition2;
    //             }
    //         nextFirePosition2:
    //             // printf("endFire %d\n", turnNumber);
    //             if (!isOutOfBorder(firePosition, 0, numRows, numColumns) && !fireDead)
    //             {
    //                 // printf("NotOut fp00 = %d, fp01 = %d, cursor = %d\n", firePosition[0][0], firePosition[0][1], curseur);
    //                 firedTileID = gridWorked[firePosition[0][0]][firePosition[0][1]];
    //                 fireDead = false;
    //             }
    //             else
    //             {
    //                 // printf("\nyou're finished :\n");
    //                 // printf("Out fp00 = %d, fp01 = %d, cursor = %d\n\n", firePosition[0][0], firePosition[0][1], curseur);
    //             }
    //         }
    //         *objectiveFunctionMH = *objectiveFunctionMH - 2;
    //     }

    //     else if (gridWorked[tankPosition[0][0]][tankPosition[0][1]] != testMove2 && testMove2 != FIRE)
    //     {
    //         // printf("turn cursor = %d\n\n", curseur);
    //         gridWorked[tankPosition[0][0]][tankPosition[0][1]] = testMove2;
    //         *currentTankDirection = gridWorked[tankPosition[0][0]][tankPosition[0][1]];
    //     }
    //     else
    //     {
    //         if (isLegalMove(tankPosition, 0, testMove2, gridWorked, numRows, numColumns))
    //         {
    //             // mirror to begin
    //             mirrorGrid(gridWorked, gridWorkedCopy, numRows, numColumns);
    //             mirrorGrid(gridMovables, gridMovablesCopy, numRows, numColumns);
    //             mirrorGrid(gridGround, gridGroundCopy, numRows, numColumns);
    //             mirrorPosition(tankPosition, 0, 1);
    //             if (nextHighWay(tankPosition, 1, testMove2, gridWorked))
    //             {
    //                 // ok so move on H
    //                 if (moveTank(tankPosition, 1, testMove2, gridWorked, gridGround))
    //                 {
    //                     // you r on way so what way ?
    //                     if (onFirstHighWay(tankPosition, testMove2, gridWorked, gridMovables, gridGround, numRows, numColumns))
    //                     {
    //                         mirrorPosition(tankPosition, 1, 0);
    //                         *objectiveFunctionMH = *objectiveFunctionMH - 1;
    //                         gridWorked[tankPosition[0][0]][tankPosition[0][1]] = testMove2;
    //                         *currentTankDirection = testMove2;
    //                         goto afterHighWay2;
    //                     }
    //                     else
    //                     {
    //                         // forget all
    //                         mirrorGrid(gridWorkedCopy, gridWorked, numRows, numColumns);
    //                         mirrorGrid(gridMovablesCopy, gridMovables, numRows, numColumns);
    //                         mirrorGrid(gridGroundCopy, gridGround, numRows, numColumns);
    //                         mirrorPosition(tankPosition, 0, 1);
    //                         goto afterHighWay2;
    //                     }
    //                 }
    //             }
    //             else
    //             {
    //                 if (moveTank(tankPosition, 0, testMove2, gridWorked, gridGround))
    //                 {
    //                     *objectiveFunctionMH = *objectiveFunctionMH - 1;
    //                 }
    //             }
    //         }
    //         else
    //         {
    //             // printf("nonlegal %d\n", turnNumber);
    //         }
    //     }
    // afterHighWay2:
    // nextAction2:
    //     print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
    // }
    // // bug !!!

    mirrorGrid(gridWorked, gridWorkedCopy, numRows, numColumns);
    mirrorGrid(gridMovables, gridMovablesCopy, numRows, numColumns);
    mirrorGrid(gridGround, gridGroundCopy, numRows, numColumns);
    mirrorPosition(tankPosition, 2, 0);
    mirrorPosition(tankPosition, 2, 1);
    //////////////////////////////////////////////////////////////////
    // Display //
    printf("gridOrigin = lignes : %d et colonnes : %d\n", *numRows, *numColumns);
    printArray(gridOrigin, numRows, numColumns);
    printf("gridWorked = lignes : %d et colonnes : %d\n", *numRows, *numColumns);
    printArray(gridWorked, numRows, numColumns);
    printf("gridGround = lignes : %d et colonnes : %d\n", *numRows, *numColumns);
    printArray(gridGround, numRows, numColumns);
    printf("gridMovables = lignes : %d et colonnes : %d\n", *numRows, *numColumns);
    printArray(gridMovables, numRows, numColumns);
    printf("tankp\n");
    printLittleArray(tankPosition, 3, 2);
    printf("firep\n");
    printLittleArray(firePosition, 2, 2);
    printf("basesp\n");
    printBaseArray(basesPosition, numBases, 2);
    printf("meta\n");
    printMovingLetters(deplacementsHypotheseMH, curseurDeplacementsMH);
    printf("hyp\n");
    printMovingLetters(deplacementsHypothese, curseurDeplacementsHypothese);
    printf("retenu\n");
    printMovingLetters(deplacementsRetenu, curseurDeplacementsRetenu);

    //////////////////////////////////////////////////////////////////
    // Free Memory //
    printf("free grids\n");
    if (gridOrigin != NULL)
    {
        for (int i = 0; i < *numRows; i++)
        {
            free(gridOrigin[i]);
        }
        free(gridOrigin);
    }

    if (gridWorked != NULL)
    {
        for (int i = 0; i < *numRows; i++)
        {
            free(gridWorked[i]);
        }
        free(gridWorked);
    }

    if (gridWorkedCopy != NULL)
    {
        for (int i = 0; i < *numRows; i++)
        {
            free(gridWorkedCopy[i]);
        }
        free(gridWorkedCopy);
    }

    if (gridGround != NULL)
    {
        for (int i = 0; i < *numRows; i++)
        {
            free(gridGround[i]);
        }
        free(gridGround);
    }

    if (gridGroundCopy != NULL)
    {
        for (int i = 0; i < *numRows; i++)
        {
            free(gridGroundCopy[i]);
        }
        free(gridGroundCopy);
    }

    if (gridMovables != NULL)
    {
        for (int i = 0; i < *numRows; i++)
        {
            free(gridMovables[i]);
        }
        free(gridMovables);
    }

    if (gridMovablesCopy != NULL)
    {
        for (int i = 0; i < *numRows; i++)
        {
            free(gridMovablesCopy[i]);
        }
        free(gridMovablesCopy);
    }
    printf("free pos\n");

    if (tankPosition != NULL)
    {
        for (int i = 0; i < 3; i++)
        {
            free(tankPosition[i]);
        }
        free(tankPosition);
    }

    printf("free posf\n");

    if (firePosition != NULL)
    {
        for (int i = 0; i < 2; i++)
        {
            free(firePosition[i]);
        }
        free(firePosition);
    }
    printf("free posb\n");
    if (basesPosition != NULL)
    {
        for (int i = 0; i < 5; i++)
        {
            free(basesPosition[i]);
        }
        free(basesPosition);
    }
    printf("free deplac\n");
    free(deplacementsHypotheseMH);
    free(deplacementsHypothese);
    free(deplacementsRetenu);
    printf("free curs\n"); // bug
    free(curseurDeplacementsHypothese);
    free(curseurDeplacementsMH);
    free(curseurDeplacementsRetenu);
    printf("free objectivs\n");
    free(objectiveFunctionHypothese);
    free(objectiveFunctionMH);
    free(objectiveFunctionRetenu);
    printf("free current\n");
    free(currentTankDirection);
    free(currentFireDirection);
    printf("free nums\n");
    free(numRows);
    free(numColumns);
    free(numBases);

    printf("finish\n");

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Reading Functions //

// Display a grid
void printArray(int **array, int *rows, int *cols)
{
    for (int i = 0; i < *rows; i++)
    {
        for (int j = 0; j < *cols; j++)
        {
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }
}

// Display a grid with brakets
void printArrayBraket(int **array, int *rows, int *cols, int bRow, int bCol)
{
    for (int i = 0; i < *rows; i++)
    {
        for (int j = 0; j < *cols; j++)
        {
            if (i == bRow && j == bCol)
            {
                printf("[%d] ", array[i][j]);
            }
            else
            {
                printf("%3d ", array[i][j]);
            }
        }
        printf("\n");
    }
}

// Display 2 grids
void print2Array(int **array1, int **array2, int *rows, int *cols)
{
    printf("array1\n");
    printArray(array1, rows, cols);
    printf("array2\n");
    printArray(array2, rows, cols);
    printf("\n");
}

// Display 2 grids with brakets
void print2ArrayBraket(int **array1, int **array2, int *rows, int *cols, int bRow, int bCol)
{
    printf("array1\n");
    printArrayBraket(array1, rows, cols, bRow, bCol);
    printf("array2\n");
    printArrayBraket(array2, rows, cols, bRow, bCol);
    printf("\n");
}

// Display 3 grids
void print3Array(int **array1, int **array2, int **array3, int *rows, int *cols)
{
    printf("array1\n");
    printArray(array1, rows, cols);
    printf("array2\n");
    printArray(array2, rows, cols);
    printf("array3\n");
    printArray(array3, rows, cols);
    printf("\n");
}

// Display 3 grids with brakets
void print3ArrayBraket(int **array1, int **array2, int **array3, int *rows, int *cols, int bRow, int bCol)
{
    printf("array1\n");
    printArrayBraket(array1, rows, cols, bRow, bCol);
    printf("array2\n");
    printArrayBraket(array2, rows, cols, bRow, bCol);
    printf("array3\n");
    printArrayBraket(array3, rows, cols, bRow, bCol);
    printf("\n");
}

// Display pear(s) of Coordinates
void printLittleArray(int **array, int rows, int cols)
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

// Display only Bases' pear(s) of Coordinates
void printBaseArray(int **array, int *rows, int cols)
{
    for (int i = 0; i < *rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }
}

// Display Moving's vector
void printMovingLetters(int *array, int *curseur)
{
    for (int i = 0; i < *curseur; i++)
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
    }
    printf("\n");
}

// Display Moving's vector without pointor
void printMovingLettersWithoutPointor(int *array, int curseur)
{
    for (int i = 0; i < curseur; i++)
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
    }
    printf("\n");
}

void firstHeuristique(int **tank, int **bases, int baseNumber)
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// Array Functions //

// apply next coordinates for fire from tank
// UP to LEFT : 1 to 4; not FIRE and not 5
void getFirstShootNextCoo(int **tankPosition, int **firePosition, int *currentTankDirection)
{
    switch (*currentTankDirection)
    {
    case UP:
        firePosition[0][0] = tankPosition[0][0] - 1;
        break;
    case RIGHT:
        firePosition[0][1] = tankPosition[0][1] + 1;
        break;
    case DOWN:
        firePosition[0][0] = tankPosition[0][0] + 1;
        break;
    case LEFT:
        firePosition[0][1] = tankPosition[0][1] - 1;
        break;
    default:
        printf("errorMove %d\n", *currentTankDirection);
        break;
    }
}

//////////////////////////////////////////////////////////////////
// Global Functions //

// return 0 to 4
int getRandomMove()
{
    int randomNumber = rand() % 5;
    return randomNumber;
}

//////////////////////////////////////////////////////////////////
// Elements Functions //

// know next move legality for parametric array
bool isLegalMove(int **arrayTankCell, int tankCoo, int moveID, int **arrayGrid, int *nbRows, int *nbColumns)
{
    // il faut trouver si oui ou non, le tank peut se déplacer
    switch (moveID)
    {
    case FIRE:
        // printf("F");
        return true;
    case UP:
        // printf("[UP]\n");
        // Already at max up position
        if (arrayTankCell[tankCoo][0] == 0)
        {
            return false;
        }
        else
        {
            if (nextFloor(arrayTankCell, tankCoo, moveID, arrayGrid))
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
        if (arrayTankCell[tankCoo][1] == *nbColumns - 1)
        {
            return false;
        }
        else
        {
            if (nextFloor(arrayTankCell, tankCoo, moveID, arrayGrid))
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
        if (arrayTankCell[tankCoo][0] == *nbRows - 1)
        {
            return false;
        }
        else
        {
            if (nextFloor(arrayTankCell, tankCoo, moveID, arrayGrid))
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
        if (arrayTankCell[tankCoo][1] == 0)
        {
            return false;
        }
        else
        {
            if (nextFloor(arrayTankCell, tankCoo, moveID, arrayGrid))
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

bool autoKill(int **arrayTankCell, int positionID, int **arrayGrid)
{
    // il faut trouver si oui ou non, le tank peut faire son tire
}

// know if next tile is floor for parametric array
bool nextFloor(int **arrayTankCell, int tankCoo, int moveID, int **arrayGrid)
{
    switch (moveID)
    {
    case UP:
        // check if future ground is ok
        if (isFloor(arrayGrid[arrayTankCell[tankCoo][0] - 1][arrayTankCell[tankCoo][1]]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case RIGHT:
        if (isFloor(arrayGrid[arrayTankCell[tankCoo][0]][arrayTankCell[tankCoo][1] + 1]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case DOWN:
        if (isFloor(arrayGrid[arrayTankCell[tankCoo][0] + 1][arrayTankCell[tankCoo][1]]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case LEFT:
        if (isFloor(arrayGrid[arrayTankCell[tankCoo][0]][arrayTankCell[tankCoo][1] - 1]))
        {
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

bool isFloor(int elementID)
{
    switch (elementID)
    {
    case DIRT:
        return true;
    case BASE:
        return true;
    case WAYUP: // attention
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
    case WATERFULL:
        return true;
    default:
        // printf("other floor %d\n", floorID);
        return false;
    }
    return false;
}

// only mirror and antitank ~treat Bm different ?
bool isMovable(int elementID, int *positionID)
{
    switch (elementID)
    {
    // case MOVABLEBLOC:
    //     return true;
    // need n+1
    case ANTITANKUP:
        // positionID point to distination
        if (*positionID == DOWN)
        {
            // hear, this is killed
            return false;
        }
        // hear, this is moved
        return true;
    case ANTITANKRIGHT:
        if (*positionID == LEFT)
        {
            return false;
        }
        return true;
    case ANTITANKDOWN:
        if (*positionID == UP)
        {
            return false;
        }
        return true;
    case ANTITANKLEFT:
        if (*positionID == RIGHT)
        {
            return false;
        }
        return true;
    case MIRRORUPRIGHT:
        // positionID point to distination
        if (*positionID == DOWN || *positionID == LEFT)
        {
            // is deflected
            return false;
        }
        // isMoved
        return true;
    case MIRRORRIGHTDOWN:
        if (*positionID == LEFT || *positionID == UP)
        {
            return false;
        }
        return true;
    case MIRRORDOWNLEFT:
        if (*positionID == UP || *positionID == RIGHT)
        {
            return false;
        }
        return true;
    case MIRRORLEFTUP:
        if (*positionID == RIGHT || *positionID == DOWN)
        {
            return false;
        }
        return true;
    default:
        // printf("other movable %d\n", movableID);
        return false;
    }
    return false;
}

// only Bm, antitank and mirror
bool isMovableAtBeginning(int elementID)
{
    switch (elementID)
    {
    case MOVABLEBLOC:
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
    default:
        // printf("other movable %d\n", movableID);
        return false;
    }
    return false;
}

// only bric and antitank
bool isShootable(int elementID, int *positionID)
{
    switch (elementID)
    {
    case BRICKS:
        printf("isBrick\n");
        return true;
    // positionID point to distination
    case ANTITANKUP:
        if (*positionID == DOWN)
        {
            // hear, this is killed
            return true;
        }
        return false;
    case ANTITANKRIGHT:
        if (*positionID == LEFT)
        {
            return true;
        }
        return false;
    case ANTITANKDOWN:
        if (*positionID == UP)
        {
            return true;
        }
        return false;
    case ANTITANKLEFT:
        if (*positionID == RIGHT)
        {
            return true;
        }
        return false;
    default:
        // printf("other shootable %d\n", shootableID);
        return false;
    }
    return false;
}

bool isFireTrought(int elementID)
{
    switch (elementID)
    {
    case DIRT:
        return true;
    case BASE:
        return true;
    case WATER:
        return true;
    case WAYUP:
        return true;
    case WAYRIGHT:
        return true;
    case WAYDOWN:
        return true;
    case WAYLEFT:
        return true;
    case CRYSTALBLOCK:
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
        // printf("other shootable %d\n", shootableID);
        return false;
    }
    return false;
}

bool isFireDeflect(int elementID, int *positionID)
{
    switch (elementID)
    {
    case MIRRORUPRIGHT:
        // positionID point to distination
        if (*positionID == UP || *positionID == RIGHT)
        {
            // is moved
            return false;
        }
        // isDeflect
        return true;
    case MIRRORRIGHTDOWN:
        if (*positionID == RIGHT || *positionID == DOWN)
        {
            return false;
        }
        return true;
    case MIRRORDOWNLEFT:
        if (*positionID == DOWN || *positionID == LEFT)
        {
            return false;
        }
        return true;
    case MIRRORLEFTUP:
        if (*positionID == LEFT || *positionID == UP)
        {
            return false;
        }
        return true;
    case ROTATIVEMIRRORUPRIGHT:
        // positionID point to distination
        if (*positionID == UP || *positionID == RIGHT)
        {
            // is turned
            return false;
        }
        // isDeflect
        return true;
    case ROTATIVEMIRRORRIGHTDOWN:
        if (*positionID == RIGHT || *positionID == DOWN)
        {
            return false;
        }
        return true;
    case ROTATIVEMIRRORDOWNLEFT:
        if (*positionID == DOWN || *positionID == LEFT)
        {
            return false;
        }
        return true;
    case ROTATIVEMIRRORLEFTUP:
        if (*positionID == LEFT || *positionID == UP)
        {
            return false;
        }
        return true;
    default:
        printf("other deflect %d\n", *positionID);
        return false;
    }
    return false;
}

bool isTurnable(int elementID, int *positionID)
{
    switch (elementID)
    {
    case ROTATIVEMIRRORUPRIGHT:
        // positionID point to distination
        if (*positionID == UP || *positionID == RIGHT)
        {
            // isTurned
            return true;
        }
        // is deflected
        return false;
    case ROTATIVEMIRRORRIGHTDOWN:
        if (*positionID == RIGHT || *positionID == DOWN)
        {
            return true;
        }
        return false;
    case ROTATIVEMIRRORDOWNLEFT:
        if (*positionID == DOWN || *positionID == LEFT)
        {
            return true;
        }
        return false;
    case ROTATIVEMIRRORLEFTUP:
        if (*positionID == LEFT || *positionID == UP)
        {
            return true;
        }
        return false;
    default:
        // printf("other shootable %d\n", shootableID);
        return false;
    }
    return false;
}

// n : Ground -> Work ; n+1 : URDL ~if tank/mobale ?
bool moveTank(int **tankPosition, int tankCoo, int testMoveID, int **gridWorked, int **gridGround)
{
    switch (testMoveID)
    {
    case FIRE:
        return true;
    case UP:
        if (gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] == THINICE)
        {
            gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = WATER;
            gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = WATER;
        }
        else
        {
            gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]];
        }
        tankPosition[tankCoo][0] = tankPosition[tankCoo][0] - 1;
        gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = UP;
        return true;
    case RIGHT:
        if (gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] == THINICE)
        {
            gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = WATER;
            gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = WATER;
        }
        else
        {
            gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]];
        }
        tankPosition[tankCoo][1] = tankPosition[tankCoo][1] + 1;
        gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = RIGHT;
        return true;
    case DOWN:
        if (gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] == THINICE)
        {
            gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = WATER;
            gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = WATER;
        }
        else
        {
            gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]];
        }
        tankPosition[tankCoo][0] = tankPosition[tankCoo][0] + 1;
        gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = DOWN;
        return true;
    case LEFT:
        if (gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] == THINICE)
        {
            gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = WATER;
            gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = WATER;
        }
        else
        {
            gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]];
        }
        tankPosition[tankCoo][1] = tankPosition[tankCoo][1] - 1;
        gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = LEFT;
        return true;
    }
    return false;
}

// copy from -> paste to
void mirrorGrid(int **gridFrom, int **gridTo, int *numRows, int *numCols)
{
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numCols; j++)
        {
            gridTo[i][j] = gridFrom[i][j];
        }
    }
}

// copy from gridOrigin
void resetGridWorked(int **gridOrigin, int **gridWorked, int *numRows, int *numCols)
{
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numCols; j++)
        {
            gridWorked[i][j] = gridOrigin[i][j];
        }
    }
}

// copy from gridOrigin -> paste to gridGround
void resetGridGround(int **gridOrigin, int **gridGround, int *numRows, int *numCols)
{
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numCols; j++)
        {
            gridGround[i][j] = 0;
            if (isFloor(gridOrigin[i][j]))
            {
                gridGround[i][j] = gridOrigin[i][j];
            }
            if (gridOrigin[i][j] == 1 || isMovableAtBeginning(gridOrigin[i][j]))
            {
                gridGround[i][j] = DIRT;
            }
        }
    }
}

// copy from gridOrigin -> paste to gridMovable // isMovableAtBeginning
void resetGridMovables(int **gridOrigin, int **gridMovables, int **gridGround, int *numRows, int *numCols)
{
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numCols; j++)
        {
            gridMovables[i][j] = 0;
            if (isMovableAtBeginning(gridOrigin[i][j]))
            {
                gridMovables[i][j] = gridOrigin[i][j];
                gridGround[i][j] = DIRT;
            }
        }
    }
}

// depreciated : char to number
int replayDeplacements(char deplacementLetter)
{
    switch (deplacementLetter)
    {
    case 'F':
        return FIRE;
    case 'U':
        return UP;
    case 'R':
        return RIGHT;
    case 'D':
        return DOWN;
    case 'L':
        return LEFT;
    default:
        return NOMOVE;
    }
}

// only solid block and atdead
bool isFireStop(int elementID)
{
    switch (elementID)
    {
    case SOLIDBLOCK:
        return true;
    case ANTITANKUPDEAD:
        return true;
    case ANTITANKRIGHTDEAD:
        return true;
    case ANTITANKDOWNDEAD:
        return true;
    case ANTITANKLEFTDEAD:
        return true;
    default:
        return false;
    }
}

// true for >=-1 and <=numX; easier for fire calc
bool isOutOfBorder(int **objectPosition, int objectCoo, int *numRows, int *numColumns)
{
    if (objectPosition[objectCoo][0] < 0 || objectPosition[objectCoo][0] >= *numRows || objectPosition[objectCoo][1] < 0 || objectPosition[objectCoo][1] >= *numColumns)
    {
        return true;
    }
    return false;
}

// brick and at ~at not implemented
void shotableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns)
{
    // brick and antitank
    switch (firedTileID)
    {
    case BRICKS:
        printArray(gridWorked, numRows, numColumns);
        printf("\n");
        printArray(gridGround, numRows, numColumns);
        printf("\n");
        // printf("fp00 %d, fp01 %d bricks\n", firePosition[0][0], firePosition[0][1]);
        gridGround[firePosition[0][0]][firePosition[0][1]] = DIRT;
        gridWorked[firePosition[0][0]][firePosition[0][1]] = DIRT;
        printArray(gridWorked, numRows, numColumns);
        printf("\n");
        printArray(gridGround, numRows, numColumns);
        printf("findbricks\n");
        break;
        // only 2 kill
    case ANTITANKUP:
        if (*currentFireDirection == DOWN)
        {
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKUPDEAD;
            gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKUPDEAD;
            gridGround[firePosition[0][0]][firePosition[0][1]] = ANTITANKUPDEAD; // to be sure
            print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
            break;
        }
        else
        {
            printf("atu not kill ?\n");
        }
        break;
    case ANTITANKRIGHT:
        if (*currentFireDirection == LEFT)
        {
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHTDEAD;
            gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHTDEAD;
            gridGround[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHTDEAD;
            print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
            break;
        }
        else
        {
            printf("atr not kill ?\n");
        }
        break;
    case ANTITANKDOWN:
        if (*currentFireDirection == UP)
        {
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWNDEAD;
            gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWNDEAD;
            gridGround[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWNDEAD;
            print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
            break;
        }
        else
        {
            printf("atd not kill ?\n");
        }
        break;
    case ANTITANKLEFT:
        if (*currentFireDirection == RIGHT)
        {
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFTDEAD;
            gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFTDEAD;
            gridGround[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFTDEAD;
            print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
            break;
        }
        else
        {
            printf("atl not kill ?\n");
        }
        break;
    default:
        printf("bang ft= %d fp00 %d fp01 %d\n", firedTileID, firePosition[0][0], firePosition[0][1]);
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Bm+mirror+AT : water case ok ~missing way and ice
bool movableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns)
{
    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
    ////////// !!!!!!!!!! attention no WAYurdl and ice
    // brick and antitank
    switch (firedTileID)
    {
    case MOVABLEBLOC:

        switch (*currentFireDirection)
        {
        case UP:
            // check if future ground is ok
            firePosition[0][0] = firePosition[0][0] - 1;
            // assume is next block position
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            { // attention no WAYurdl
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    /// !!! if ice !!!
                    // replace element in new location
                    gridGround[firePosition[0][0]][firePosition[0][1]] = WATERFULL;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = WATERFULL;
                    // erase movable block
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    // copy ground at before location
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case RIGHT:
            firePosition[0][1] = firePosition[0][1] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridGround[firePosition[0][0]][firePosition[0][1]] = WATERFULL;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = WATERFULL;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case DOWN:
            firePosition[0][0] = firePosition[0][0] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridGround[firePosition[0][0]][firePosition[0][1]] = WATERFULL;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = WATERFULL;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case LEFT:
            firePosition[0][1] = firePosition[0][1] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridGround[firePosition[0][0]][firePosition[0][1]] = WATERFULL;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = WATERFULL;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("findmovable\n");
        return false;
    case MIRRORUPRIGHT:
        switch (*currentFireDirection)
        {
        case UP:
            firePosition[0][0] = firePosition[0][0] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    /// !!! ice !!!!
                    // erase movable block
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    // copy ground at before location
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case RIGHT:
            firePosition[0][1] = firePosition[0][1] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("findmovable\n");
        return false;
    case MIRRORRIGHTDOWN:

        switch (*currentFireDirection)
        {
        case RIGHT:
            firePosition[0][1] = firePosition[0][1] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case DOWN:
            firePosition[0][0] = firePosition[0][0] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("findmovable\n");
        return false;
    case MIRRORDOWNLEFT:

        switch (*currentFireDirection)
        {
        case DOWN:
            firePosition[0][0] = firePosition[0][0] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case LEFT:
            firePosition[0][1] = firePosition[0][1] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("findmovable\n");
        return false;
    case MIRRORLEFTUP:

        switch (*currentFireDirection)
        {
        case UP:
            firePosition[0][0] = firePosition[0][0] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case LEFT:
            firePosition[0][1] = firePosition[0][1] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("findmovable\n");
        return false;
        //////////wip and revert true/false
        // int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns
    case ANTITANKUP:
        // positionID point to distination
        switch (*currentFireDirection)
        {
        case UP:
            printf("err mov atu case up\n");
            return false;
        case RIGHT:
            firePosition[0][1] = firePosition[0][1] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case DOWN:
            firePosition[0][0] = firePosition[0][0] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case LEFT:
            firePosition[0][1] = firePosition[0][1] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        ///
    case ANTITANKRIGHT:
        switch (*currentFireDirection)
        {
        case UP:
            firePosition[0][0] = firePosition[0][0] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case RIGHT:
            printf("err mov atr case r\n");
            return false;
        case DOWN:
            firePosition[0][0] = firePosition[0][0] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case LEFT:
            firePosition[0][1] = firePosition[0][1] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
    case ANTITANKDOWN:
        switch (*currentFireDirection)
        {
        case UP:
            firePosition[0][0] = firePosition[0][0] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case RIGHT:
            firePosition[0][1] = firePosition[0][1] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case DOWN:
            printf("err mov atd case d\n");
            return false;
        case LEFT:
            firePosition[0][1] = firePosition[0][1] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
    case ANTITANKLEFT:
        switch (*currentFireDirection)
        {
        case UP:
            firePosition[0][0] = firePosition[0][0] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case RIGHT:
            firePosition[0][1] = firePosition[0][1] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case DOWN:
            firePosition[0][0] = firePosition[0][0] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                if (gridGround[firePosition[0][0]][firePosition[0][1]] == WATER)
                {
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                    return true;
                }
            }
            return false;
        case LEFT:
            printf("err mov atl case l\n");
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
    default:
        // printf("bangid = %d tn %d\n", firedTileID, turnNumber);
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// mirror and rmirror ok
bool deflectableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns)
{
    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
    switch (firedTileID)
    {
    case MIRRORUPRIGHT:
    case ROTATIVEMIRRORUPRIGHT:

        switch (*currentFireDirection)
        {
        case DOWN:
            *currentFireDirection = RIGHT;
            firePosition[0][1] = firePosition[0][1] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                return true;
            }
            return false;
        case LEFT:
            *currentFireDirection = UP;
            firePosition[0][0] = firePosition[0][0] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                return true;
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("finddeflectable\n");
        return false;
    case MIRRORRIGHTDOWN:
    case ROTATIVEMIRRORRIGHTDOWN:
        switch (*currentFireDirection)
        {
        case LEFT:
            *currentFireDirection = DOWN;
            firePosition[0][0] = firePosition[0][0] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                return true;
            }
            return false;
        case UP:
            *currentFireDirection = RIGHT;
            firePosition[0][1] = firePosition[0][1] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                return true;
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("finddeflectable\n");
        return false;
    case MIRRORDOWNLEFT:
    case ROTATIVEMIRRORDOWNLEFT:
        switch (*currentFireDirection)
        {
        case UP:
            *currentFireDirection = LEFT;
            firePosition[0][1] = firePosition[0][1] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                return true;
            }
            return false;
        case RIGHT:
            *currentFireDirection = DOWN;
            firePosition[0][0] = firePosition[0][0] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                return true;
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("finddeflectable\n");
        return false;
    case MIRRORLEFTUP:
    case ROTATIVEMIRRORLEFTUP:
        switch (*currentFireDirection)
        {
        case RIGHT:
            *currentFireDirection = UP;
            firePosition[0][0] = firePosition[0][0] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                return true;
            }
            return false;
        case DOWN:
            *currentFireDirection = LEFT;
            firePosition[0][1] = firePosition[0][1] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
                return true;
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("finddeflectable\n");
        return false;
    default:
        // printf("bangid = %d tn %d\n", firedTileID, turnNumber);
        return false;
    }
}

bool turnableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns)
{
    print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
    switch (firedTileID)
    {
    case ROTATIVEMIRRORDOWNLEFT:

        switch (*currentFireDirection)
        {
        case DOWN:
        case LEFT:
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ROTATIVEMIRRORLEFTUP;
            print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
            return true;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("finddeflectable\n");
        return false;
    case ROTATIVEMIRRORLEFTUP:

        switch (*currentFireDirection)
        {
        case LEFT:
        case UP:
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ROTATIVEMIRRORUPRIGHT;
            print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
            return true;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("finddeflectable\n");
        return false;
    case ROTATIVEMIRRORUPRIGHT:
        switch (*currentFireDirection)
        {
        case UP:
        case RIGHT:
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ROTATIVEMIRRORRIGHTDOWN;
            print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
            return true;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("finddeflectable\n");
        return false;
    case ROTATIVEMIRRORRIGHTDOWN:

        switch (*currentFireDirection)
        {
        case RIGHT:
        case DOWN:
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ROTATIVEMIRRORDOWNLEFT;
            print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
            return true;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3Array(gridWorked, gridMovables, gridGround, numRows, numColumns);
        printf("finddeflectable\n");
        return false;
    default:
        // printf("bangid = %d tn %d\n", firedTileID, turnNumber);
        return false;
    }
}

// copy from -> paste to
void mirrorPosition(int **tankPosition, int fromCoo, int toCoo)
{
    tankPosition[toCoo][0] = tankPosition[fromCoo][0];
    tankPosition[toCoo][1] = tankPosition[fromCoo][1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// recursive ok ~missing ice and Bm/Mirror
bool onFirstHighWay(int **tankPosition, int moveID, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns)
{
    bool isOnHighWay = true;
    // you r on n+1
    printf("onfirstHW %d\n", moveID);
    // here on way
    printf("start highway %d\n", gridWorked[tankPosition[1][0]][tankPosition[1][1]]);
    print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
    // follow n+1 tile where u are
    switch (gridGround[tankPosition[1][0]][tankPosition[1][1]])
    {
    case WAYUP:
        tankPosition[1][0] = tankPosition[1][0] - 1;
        // check whatif is on grid n+2
        if (!(isOutOfBorder(tankPosition, 1, numRows, numColumns)))
        {
            // other tunnel/enemy way to die // !!! prevent circle WIP
            if (isDeathDestination(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
            {
                printf("death\n"); // plouf
                print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
                return false;
            }
            // check you can move properly //ok so backward ur sigth
            tankPosition[1][0] = tankPosition[1][0] + 1;
            // if ok dirt/base/way
            if (isLegalMove(tankPosition, 1, UP, gridWorked, numRows, numColumns))
            {
                // next way or tile // move in n+2
                moveTank(tankPosition, 1, UP, gridWorked, gridGround);
                printf("legalmove\n");
                // check if n+2 is HW
                if (isHighWay(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
                {
                    // check for n+2
                    if (!onFirstHighWay(tankPosition, UP, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("%d %d %d\n", gridGround[tankPosition[1][0]][tankPosition[1][1]], tankPosition[1][0], tankPosition[1][1]);
                        printf("return false\n");
                        print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
                        return false;
                    }
                }
                else if (isFloor(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
                {
                    printf("its floor\n");
                    return true;
                }
            }
            else
            {
                // you were already blocked by stop tile
                printf("illegal move\n");
                print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
                return true;
            }
        }
        else
        {
            // you were already blocked by grid's borders
            tankPosition[1][0] = tankPosition[1][0] + 1;
            printf("outborder\n");
            print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
            return true;
        }
        break;
    case WAYRIGHT:
        tankPosition[1][1] = tankPosition[1][1] + 1;
        if (!(isOutOfBorder(tankPosition, 1, numRows, numColumns)))
        {
            if (isDeathDestination(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
            {
                printf("%d %d %d\n", gridGround[tankPosition[1][0]][tankPosition[1][1]], tankPosition[1][0], tankPosition[1][1]);
                printf("death\n"); // plouf
                return false;
            }
            tankPosition[1][1] = tankPosition[1][1] - 1;
            if (isLegalMove(tankPosition, 1, RIGHT, gridWorked, numRows, numColumns))
            {
                moveTank(tankPosition, 1, RIGHT, gridWorked, gridGround);
                printf("legalmove\n");
                if (isHighWay(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
                {
                    if (!onFirstHighWay(tankPosition, RIGHT, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("%d %d %d\n", gridGround[tankPosition[1][0]][tankPosition[1][1]], tankPosition[1][0], tankPosition[1][1]);
                        printf("return false\n");
                        print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
                        return false;
                    }
                }
                else if (isFloor(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
                {
                    return true;
                }
            }
            else
            {
                printf("illegal move\n");
                print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
                return true;
            }
        }
        else
        {
            tankPosition[1][1] = tankPosition[1][1] - 1;
            printf("outborder\n");
            print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
            return true;
        }
        break;
    case WAYDOWN:
        tankPosition[1][0] = tankPosition[1][0] + 1;
        if (!(isOutOfBorder(tankPosition, 1, numRows, numColumns)))
        {
            if (isDeathDestination(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
            {
                printf("%d %d %d\n", gridGround[tankPosition[1][0]][tankPosition[1][1]], tankPosition[1][0], tankPosition[1][1]);
                printf("death\n"); // plouf
                return false;
            }
            tankPosition[1][0] = tankPosition[1][0] - 1;
            if (isLegalMove(tankPosition, 1, DOWN, gridWorked, numRows, numColumns))
            {
                moveTank(tankPosition, 1, DOWN, gridWorked, gridGround);
                printf("legalmove\n");
                if (isHighWay(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
                {
                    if (!onFirstHighWay(tankPosition, DOWN, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("%d %d %d\n", gridGround[tankPosition[1][0]][tankPosition[1][1]], tankPosition[1][0], tankPosition[1][1]);
                        printf("return false\n");
                        print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
                        return false;
                    }
                }
                else if (isFloor(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
                {
                    return true;
                }
            }
            else
            {
                printf("illegal move\n");
                print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
                return true;
            }
        }
        else
        {
            tankPosition[1][0] = tankPosition[1][0] - 1;
            printf("outborder\n");
            print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
            return true;
        }
        break;
    case WAYLEFT:
        tankPosition[1][1] = tankPosition[1][1] - 1;
        if (!(isOutOfBorder(tankPosition, 1, numRows, numColumns)))
        {
            if (isDeathDestination(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
            {
                printf("%d %d %d\n", gridGround[tankPosition[1][0]][tankPosition[1][1]], tankPosition[1][0], tankPosition[1][1]);
                printf("death\n"); // plouf
                return false;
            }
            tankPosition[1][1] = tankPosition[1][1] + 1;
            if (isLegalMove(tankPosition, 1, LEFT, gridWorked, numRows, numColumns))
            {
                moveTank(tankPosition, 1, LEFT, gridWorked, gridGround);
                printf("legalmove\n");
                if (isHighWay(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
                {
                    if (!onFirstHighWay(tankPosition, LEFT, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("%d %d %d\n", gridGround[tankPosition[1][0]][tankPosition[1][1]], tankPosition[1][0], tankPosition[1][1]);
                        printf("return false\n");
                        print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
                        return false;
                    }
                }
                else if (isFloor(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
                {
                    return true;
                }
            }
            else
            {
                printf("illegal move\n");
                print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
                return true;
            }
        }
        else
        {
            tankPosition[1][1] = tankPosition[1][1] + 1;
            printf("outborder\n");
            print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
            return true;
        }
        break;
    case ICE:
        if (isLegalMove(tankPosition, 1, moveID, gridWorked, numRows, numColumns))
        {
            moveTank(tankPosition, 1, moveID, gridWorked, gridGround);
            printf("legalICEmove\n");
            if (isHighWay(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
            {
                if (!onFirstHighWay(tankPosition, moveID, gridWorked, gridMovables, gridGround, numRows, numColumns))
                {
                    printf("%d %d %d\n", gridGround[tankPosition[1][0]][tankPosition[1][1]], tankPosition[1][0], tankPosition[1][1]);
                    printf("return false\n");
                    print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
                    return false;
                }
            }
            else if (isFloor(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
            {
                return true;
            }
        }

        break;
    case THINICE:
        // move tank if its legal same but replace by water
        if (isLegalMove(tankPosition, 1, moveID, gridWorked, numRows, numColumns))
        {
            moveTank(tankPosition, 1, moveID, gridWorked, gridGround);
            printf("legalICEmove\n");
            if (isHighWay(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
            {
                if (!onFirstHighWay(tankPosition, moveID, gridWorked, gridMovables, gridGround, numRows, numColumns))
                {
                    printf("%d %d %d\n", gridGround[tankPosition[1][0]][tankPosition[1][1]], tankPosition[1][0], tankPosition[1][1]);
                    printf("return false\n");
                    print3ArrayBraket(gridWorked, gridMovables, gridGround, numRows, numColumns, tankPosition[1][0], tankPosition[1][1]);
                    return false;
                }
            }
            else if (isFloor(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
            {
                return true;
            }
        }
        break;
    default:
        printf("stop highway %d\n", gridWorked[tankPosition[1][0]][tankPosition[1][1]]);
        isOnHighWay = false;
        break;
    }
    // }
    printf("end of highway\n");
    return true;
    // }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// only water ~missing tunnel
bool isDeathDestination(int curentTile)
{
    switch (curentTile)
    {
    case WATER:
        return true;
    // case TUNNELRED:
    // case TUNNELGREEN:
    // case TUNNELBLUE:
    // case TUNNELCYAN:
    // case TUNNELYELLOW:
    // case TUNNELPINK:
    // case TUNNELWHITE:
    // case TUNNELDARK:
    default:
        return false;
        break;
    }
}

// True if Way or Ice
bool isHighWay(int elementID)
{
    switch (elementID)
    {
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
    default:
        printf("not highway %d\n", elementID);
        return false;
    }
    return false;
}

// true if next tile is way
bool nextHighWay(int **arrayTankCell, int tankCoo, int moveID, int **arrayGrid)
{
    switch (moveID)
    {
    case UP:
        // check if future ground is ok
        if (isHighWay(arrayGrid[arrayTankCell[tankCoo][0] - 1][arrayTankCell[tankCoo][1]]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case RIGHT:
        if (isHighWay(arrayGrid[arrayTankCell[tankCoo][0]][arrayTankCell[tankCoo][1] + 1]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case DOWN:
        if (isHighWay(arrayGrid[arrayTankCell[tankCoo][0] + 1][arrayTankCell[tankCoo][1]]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case LEFT:
        if (isHighWay(arrayGrid[arrayTankCell[tankCoo][0]][arrayTankCell[tankCoo][1] - 1]))
        {
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

void erazeUselessTurn(int *vector, int *curseur)
{
    int lastFireCell = 0;  // rank
    int lastDirection = 1; // up

    for (int i = 0; i < *curseur; i++)
    {
        // printf("\n|i=%d; *curseur = %d\n", i, *curseur);
        if (i == *curseur)
        {
            printf("|breek\n");
            break;
        }
        else if (vector[i] == 0 && lastFireCell == 0)
        {
            lastFireCell = i + 1; // rank
            // printf("|lF1=%d __", lastFireCell);
        }
        else if (vector[i] == 0 && lastFireCell != 0)
        {
            if (i - lastFireCell == 0)
            {
                lastFireCell = lastFireCell + 1;
                // printf("|lF2=%d __", lastFireCell);
                goto skip;
            }
            if (i - lastFireCell == 1)
            {
                lastFireCell = 0;
                // printf("|lF3=%d __", lastFireCell);
                goto skip;
            }
        }
        else if (vector[i] != 0)
        {
            if (vector[i] == lastDirection)
            {
                // reset
                lastFireCell = 0;
                // printf("|lF4=%d __", lastFireCell);
                // for i -> i+1 next
            }
            else if (vector[i + 1] != 0 && vector[i + 1] != lastDirection && vector[i + 1] != vector[i])
            {
                // implicite: vector[i] != lastDirection
                if (lastFireCell != 0)
                {
                    for (int j = 0; j < *curseur - 1; j++)
                    {
                        // erase
                        vector[lastFireCell + j] = vector[lastFireCell + 1 + j];
                    }
                }
                else
                {
                    for (int j = 0; j < *curseur - 1; j++)
                    {
                        // erase
                        vector[i + j] = vector[i + 1 + j];
                    }
                }
                *curseur = *curseur - 1;
                // printf("|c1=%d __", *curseur);
                i = i - 1;
                // printf("|i1=%d __", i);
                // for (int k = 0; k < *curseur; k++)
                // {
                //     printf("%d ", tableau[k]);
                // }
            }
            else if (vector[i + 1] == 0)
            {
                // turn
                lastDirection = vector[i];
                // printf("|ld1=%d __", lastDirection);
                // reset
                lastFireCell = 0;
                // printf("|lF5=%d __", lastFireCell);
            }
            else if (vector[i + 1] == lastDirection)
            {
                if (vector[i + 2] == lastDirection)
                {
                    /* code */
                    printf("vi-1=%d, vi=%d, vi+1=%d, vi+2=%d, ld=%d\n", vector[i - 1], vector[i], vector[i + 1], vector[i + 2], lastDirection);
                    printf("|bruh __\n");
                }
                else if (vector[i + 2] != lastDirection)
                {
                    // delete 2
                    for (int j = 0; j < *curseur - 2; j++)
                    {
                        // erase
                        vector[i + j] = vector[i + 2 + j];
                    }
                    *curseur = *curseur - 2;
                    // printf("|c2=%d __", *curseur);
                    i = i - 1; //
                    // printf("|i2=%d __", i);
                }
            }
            else if (vector[i + 1] == vector[i])
            {
                // turn
                lastDirection = vector[i];
                // printf("|ld2=%d __", lastDirection);
                // reset
                lastFireCell = 0;
                // printf("|lF6=%d __", lastFireCell);
            }
        }
    skip:
    }
}

bool antiTankAction(int **tankPosition, int **gridWorked, int *numRows, int *numColumns)
{
    int rangeAT[1][2];
    rangeAT[0][0] = tankPosition[0][0];
    rangeAT[0][1] = tankPosition[0][1];

    for (int i = UP; i <= LEFT; i++)
    {
        switch (i)
        {
        case UP:
            if (tankPosition[0][0] > 0)
            {
                for (int j = 1; j <= tankPosition[0][0]; j++)
                {
                    rangeAT[0][0] = tankPosition[0][0] - j;
                    if (gridWorked[rangeAT[0][0]][rangeAT[0][1]] == ANTITANKDOWN)
                    {
                        return true;
                    }
                    if (!isFireTrought(gridWorked[rangeAT[0][0]][rangeAT[0][1]]))
                    {
                        printf("no upped at bc bolcked\n");
                        return false;
                    }
                }
            }
            else
            {
                printf("no upped at\n");
                return false;
            }
            break;

        case RIGHT:
            if (tankPosition[0][1] > *numColumns - 1)
            {
                for (int j = 1; j <= *numColumns - 1 - tankPosition[0][1]; j++)
                {
                    rangeAT[0][1] = tankPosition[0][1] + j;
                    if (gridWorked[rangeAT[0][0]][rangeAT[0][1]] == ANTITANKLEFT)
                    {
                        return true;
                    }
                    if (!isFireTrought(gridWorked[rangeAT[0][0]][rangeAT[0][1]]))
                    {
                        printf("no righted at bc bolcked\n");
                        return false;
                    }
                }
            }
            else
            {
                printf("no righted at\n");
                return false;
            }
            break;

        case DOWN:
            if (tankPosition[0][0] > *numRows - 1)
            {
                for (int j = 1; j <= *numRows - 1 - tankPosition[0][0]; j++)
                {
                    rangeAT[0][0] = tankPosition[0][0] + j;
                    if (gridWorked[rangeAT[0][0]][rangeAT[0][1]] == ANTITANKUP)
                    {
                        return true;
                    }
                    if (!isFireTrought(gridWorked[rangeAT[0][0]][rangeAT[0][1]]))
                    {
                        printf("no downed at bc bolcked\n");
                        return false;
                    }
                }
            }
            else
            {
                printf("no downed at\n");
                return false;
            }
            break;
        case LEFT:
            if (tankPosition[0][1] > 0)
            {
                for (int j = 1; j <= tankPosition[0][1]; j++)
                {
                    rangeAT[0][1] = tankPosition[0][1] - j;
                    if (gridWorked[rangeAT[0][0]][rangeAT[0][1]] == ANTITANKRIGHT)
                    {
                        return true;
                    }
                    if (!isFireTrought(gridWorked[rangeAT[0][0]][rangeAT[0][1]]))
                    {
                        printf("no lefted at bc bolcked\n");
                        return false;
                    }
                }
            }
            else
            {
                printf("no lefted at\n");
                return false;
            }
            break;
        default:
            printf("err move atAction %d\n", i);
            return false;
        }
    }
    printf("no AT action\n");
    return false;
}
