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
bool tankAction(int **gridOrigin, int **gridWorked, int **gridMovables,
                int **gridGround, int **gridWorkedCopy, int **gridGroundCopy, int **gridMovablesCopy,
                int *numRows, int *numColumns,
                int **tankPosition, int **basesPosition, int **firePosition,
                int *currentTankDirection, int *currentFireDirection,
                int *curseurDeplacementsHypothese, int *curseurDeplacementsRetenu, int *curseurDeplacementsMH,
                int *objectiveFunctionMH,
                int *testMove, int *curseur);
//////////////////////////////////////////////////////////////////
// Array Functions //

//////////////////////////////////////////////////////////////////
// Elements Functions //

// isSomething
// grid

bool isFloor(int elementID);
bool isTunnel(int elementID);
bool isOutOfBorder(int **objectPosition, int objectCoo, int *numRows, int *numColumns);
// tank

bool isLegalMove(int **arrayTankCell, int tankCoo, int moveID, int **arrayGrid, int *nbRows, int *nbColumns);
bool onFirstHighWay(int **tankPosition, int moveID, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
bool isHighWay(int elementID);
bool tunnelTPTank(int **tankPosition, int tankCoo, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
// elements

bool isMovable(int elementID, int *positionID);
bool isMovableAtBeginning(int elementID);
bool isShootable(int elementID, int *positionID);
bool isTurnable(int elementID, int *positionID);
bool isDeathDestination(int curentTile);
bool nextHighWay(int **arrayTankCell, int tankCoo, int moveID, int **arrayGrid);
bool tunnelTPMovables(int movableX, int movableY, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
// fire

bool isFireTrought(int elementID);
bool isFireDeflect(int elementID, int *positionID);
bool isFireStop(int elementID);
bool antiTankAction(int **tankPosition, int tankCoo, int **gridWorked, int *numRows, int *numColumns);

void getFirstShootNextCoo(int **tankPosition, int **firePosition, int *currentTankDirection);
void shotableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
// Ground

bool nextFloor(int **arrayTankCell, int tankCoo, int moveID, int **arrayGrid);
// Tank

bool moveTank(int **tankPosition, int tankCoo, int testMoveID, int **gridWorked, int **gridGround);
void mirrorPosition(int **tankPosition, int fromCoo, int toCoo);
// Movables

bool movableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
bool movableOnIce(int movableX, int movableY, int moveID, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
bool isMovableFloor(int elementID);
bool isLegalMVMove(int movableX, int movableY, int moveID, int **arrayGrid, int *nbRows, int *nbColumns);
bool nextMoovableFloor(int movableX, int movableY, int moveID, int **arrayGrid);
bool moveMovable(int movableX, int movableY, int testMoveID, int **gridWorked, int **gridMovables, int **gridGround);
void advanceMovableCoo(int *movableX, int *movableY, int moveID);
bool isIce(int elementID);
// Reflectables

bool deflectableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
bool turnableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
// Grids

void resetGridWorked(int **gridOrigin, int **gridWorked, int *numRows, int *numColumns);
void resetGridGround(int **gridOrigin, int **gridGround, int *numRows, int *numColumns);
void resetGridMovables(int **gridOrigin, int **gridMovables, int **gridGround, int *numRows, int *numColumns);
void mirrorGrid(int **gridFrom, int **gridTo, int *numRows, int *numColumns);
void mirror3Grids(int **gridFrom1, int **gridTo1, int **gridFrom2, int **gridTo2, int **gridFrom3, int **gridTo3, int *numRows, int *numColumns);
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
void printArrayTarget(int **array, int *rows, int *cols, int bRow, int bCol);
void print2ArrayTarget(int **array1, int **array2, int *rows, int *cols, int bRow, int bCol);
void print3ArrayTarget(int **array1, int **array2, int **array3, int *rows, int *cols, int bRow, int bCol);
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
    const char *filename = ".\\TestingGrids\\testing8.lt4";
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
    memset(deplacementsHypothese, -1, deplacementsSize);
    memset(deplacementsHypotheseMH, -1, deplacementsSize);

    *curseurDeplacementsHypothese = 0;
    *curseurDeplacementsRetenu = 0;
    *curseurDeplacementsMH = 0;
    *objectiveFunctionMH = 0;
    *currentTankDirection = gridWorked[tankPosition[0][0]][tankPosition[0][1]];

    printf("curserH %d ; curserR %d\n", *curseurDeplacementsHypothese, *curseurDeplacementsRetenu);
    ///////////////////////////
    // MHeuristique

    int *curseur = 0;
    int *testMove = getRandomMove();
    testMove = 0;

    // reset grid
    resetGridWorked(gridOrigin, gridWorked, numRows, numColumns);
    resetGridGround(gridOrigin, gridGround, numRows, numColumns);
    resetGridMovables(gridOrigin, gridMovables, gridGround, numRows, numColumns);
    resetGridWorked(gridOrigin, gridWorkedCopy, numRows, numColumns);
    resetGridGround(gridOrigin, gridGroundCopy, numRows, numColumns);
    resetGridMovables(gridOrigin, gridMovablesCopy, gridGround, numRows, numColumns);

    // stop if tank == base
    // while
    // while (!(tankPosition[0][0] == basesPosition[0][0] &&
    //          tankPosition[0][1] == basesPosition[0][1]))
    // {
    // rnd 1-n
    // chose combi 0, 10,20,30,40,11,22,33,44
    // fnd frst : false combi+1/total
    // fnd sc : true whrite : false cmbi+1
    // write : curseur+2
    // si dans le vide ou all combi other
    // fn du diable
    //
    //
    printf("bigin\n");
    // if (tankAction(gridOrigin, gridWorked, gridMovables,
    //                gridGround, gridWorkedCopy, gridGroundCopy, gridMovablesCopy,
    //                numRows, numColumns,
    //                tankPosition, basesPosition, firePosition,
    //                currentTankDirection, currentFireDirection,
    //                curseurDeplacementsHypothese, curseurDeplacementsRetenu, curseurDeplacementsMH,
    //                objectiveFunctionMH,
    //                testMove, curseur))
    // {
    if (tankAction(gridOrigin, gridWorked, gridMovables, gridGround,
                   gridWorkedCopy, gridGroundCopy, gridMovablesCopy,
                   numRows, numColumns, tankPosition, basesPosition,
                   firePosition, &currentTankDirection, &currentFireDirection,
                   curseurDeplacementsHypothese, curseurDeplacementsRetenu, curseurDeplacementsMH,
                   &objectiveFunctionMH, &testMove, &curseur))
    {
        printf("ok|||||||\n");
        mirror3Grids(gridWorked, gridWorkedCopy, gridMovables, gridMovablesCopy, gridGroundCopy, gridGround, numRows, numColumns);
        mirrorPosition(tankPosition, 0, 1);
        mirrorPosition(tankPosition, 0, 2);
    }
    else
    {
        printf("ko\n");
        mirror3Grids(gridWorkedCopy, gridWorked, gridMovablesCopy, gridMovables, gridGroundCopy, gridGround, numRows, numColumns);
        mirrorPosition(tankPosition, 2, 0);
        mirrorPosition(tankPosition, 2, 1);
    }
    printf("curse cursor = %d\n", curseur);
    tankAction(gridOrigin, gridWorked, gridMovables, gridGround,
                   gridWorkedCopy, gridGroundCopy, gridMovablesCopy,
                   numRows, numColumns, tankPosition, basesPosition,
                   firePosition, &currentTankDirection, &currentFireDirection,
                   curseurDeplacementsHypothese, curseurDeplacementsRetenu, curseurDeplacementsMH,
                   &objectiveFunctionMH, &testMove, &curseur);
    printf("curse cursor = %d\n", curseur);

    mirror3Grids(gridWorked, gridWorkedCopy, gridMovables, gridMovablesCopy, gridGroundCopy, gridGround, numRows, numColumns);
    mirrorPosition(tankPosition, 2, 0);
    mirrorPosition(tankPosition, 2, 1);
    //     // compteur de tour
    // }

    // > 20 not fast at all, 30 -> 5 seconds
    // while (curseurDeplacementsMH > curseurDeplacementsHypothese);
    // bugtest:
    // *curseurDeplacementsMH = *curseur;
    // printf("curserHM %d\n", *curseurDeplacementsMH);

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
    printf("\n");
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
    printf("\n");
}

// Display a grid with |
void printArrayTarget(int **array, int *rows, int *cols, int bRow, int bCol)
{
    for (int i = 0; i < *rows; i++)
    {
        for (int j = 0; j < *cols; j++)
        {
            if (i == bRow && j == bCol)
            {
                printf("|%d| ", array[i][j]);
            }
            else
            {
                printf("%3d ", array[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
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

// Display 2 grids with targets
void print2ArrayTarget(int **array1, int **array2, int *rows, int *cols, int bRow, int bCol)
{
    printf("array1\n");
    printArrayTarget(array1, rows, cols, bRow, bCol);
    printf("array2\n");
    printArrayTarget(array2, rows, cols, bRow, bCol);
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

// Display 3 grids with tagets
void print3ArrayTarget(int **array1, int **array2, int **array3, int *rows, int *cols, int bRow, int bCol)
{
    printf("array1\n");
    printArrayTarget(array1, rows, cols, bRow, bCol);
    printf("array2\n");
    printArrayTarget(array2, rows, cols, bRow, bCol);
    printf("array3\n");
    printArrayTarget(array3, rows, cols, bRow, bCol);
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

// know next move legality for parametric array
bool isLegalMVMove(int movableX, int movableY, int moveID, int **arrayGrid, int *nbRows, int *nbColumns)
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
        if (movableX == 0)
        {
            return false;
        }
        else
        {
            if (nextMoovableFloor(movableX, movableY, moveID, arrayGrid))
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
        if (movableY == *nbColumns - 1)
        {
            return false;
        }
        else
        {
            if (nextMoovableFloor(movableX, movableY, moveID, arrayGrid))
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
        if (movableX == *nbRows - 1)
        {
            return false;
        }
        else
        {
            if (nextMoovableFloor(movableX, movableY, moveID, arrayGrid))
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
        if (movableY == 0)
        {
            return false;
        }
        else
        {
            if (nextMoovableFloor(movableX, movableY, moveID, arrayGrid))
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

bool nextMoovableFloor(int movableX, int movableY, int moveID, int **arrayGrid)
{
    switch (moveID)
    {
    case UP:
        // check if future ground is ok
        if (isMovableFloor(arrayGrid[movableX - 1][movableY]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case RIGHT:
        if (isMovableFloor(arrayGrid[movableX][movableY + 1]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case DOWN:
        if (isMovableFloor(arrayGrid[movableX + 1][movableY]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case LEFT:
        if (isMovableFloor(arrayGrid[movableX][movableY - 1]))
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
    case WATERFULL:
        return true;
    default:
        // printf("other floor %d\n", floorID);
        return false;
    }
    return false;
}

bool isMovableFloor(int elementID)
{
    switch (elementID)
    {
    case DIRT:
        return true;
    case BASE:
        return true;
    // adding
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

bool isTunnel(int elementID)
{
    switch (elementID)
    {
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
        // more simple
    case BRICKS:
        return true;
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
        // printf("isBrick\n");
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

// n : Ground -> Work ; n+1 : URDL ~if tank/mobale ?
bool moveMovable(int movableX, int movableY, int testMoveID, int **gridWorked, int **gridMovables, int **gridGround)
{ // gridmov !!!!!!
    int movableID = 0;
    switch (testMoveID)
    {
    case UP:
        movableID = gridWorked[movableX][movableY];
        gridMovables[movableX][movableY] = NOTHING;
        if (gridGround[movableX][movableY] == THINICE)
        {
            gridWorked[movableX][movableY] = WATER;
            gridGround[movableX][movableY] = WATER;
        }
        else if (gridGround[movableX][movableY] == WATER)
        {
            gridWorked[movableX][movableY] = gridGround[movableX][movableY];
            return false;
        }
        else
        {
            gridWorked[movableX][movableY] = gridGround[movableX][movableY];
            gridMovables[movableX][movableY] = NOTHING;
        }
        movableX = movableX - 1;
        gridWorked[movableX][movableY] = movableID;
        gridMovables[movableX][movableY] = movableID;
        return true;
    case RIGHT:
        movableID = gridWorked[movableX][movableY];
        gridMovables[movableX][movableY] = NOTHING;
        if (gridGround[movableX][movableY] == THINICE)
        {
            gridWorked[movableX][movableY] = WATER;
            gridGround[movableX][movableY] = WATER;
        }
        else if (gridGround[movableX][movableY] == WATER)
        {
            gridWorked[movableX][movableY] = gridGround[movableX][movableY];
            return false;
        }
        else
        {
            gridWorked[movableX][movableY] = gridGround[movableX][movableY];
            gridMovables[movableX][movableY] = NOTHING;
        }
        movableY = movableY + 1;
        gridWorked[movableX][movableY] = movableID;
        gridMovables[movableX][movableY] = movableID;
        return true;
    case DOWN:
        movableID = gridWorked[movableX][movableY];
        gridMovables[movableX][movableY] = NOTHING;
        if (gridGround[movableX][movableY] == THINICE)
        {
            gridWorked[movableX][movableY] = WATER;
            gridGround[movableX][movableY] = WATER;
        }
        else if (gridGround[movableX][movableY] == WATER)
        {
            gridWorked[movableX][movableY] = gridGround[movableX][movableY];
            return false;
        }
        else
        {
            gridWorked[movableX][movableY] = gridGround[movableX][movableY];
            gridMovables[movableX][movableY] = NOTHING;
        }
        movableX = movableX + 1;
        gridWorked[movableX][movableY] = movableID;
        gridMovables[movableX][movableY] = movableID;
        return true;
    case LEFT:
        movableID = gridWorked[movableX][movableY];
        gridMovables[movableX][movableY] = NOTHING;
        if (gridGround[movableX][movableY] == THINICE)
        {
            gridWorked[movableX][movableY] = WATER;
            gridGround[movableX][movableY] = WATER;
        }
        else if (gridGround[movableX][movableY] == WATER)
        {
            gridWorked[movableX][movableY] = gridGround[movableX][movableY];
            return false;
        }
        else
        {
            gridWorked[movableX][movableY] = gridGround[movableX][movableY];
            gridMovables[movableX][movableY] = NOTHING;
        }
        movableY = movableY - 1;
        gridWorked[movableX][movableY] = movableID;
        gridMovables[movableX][movableY] = movableID;
        return true;
    }
    return false;
}

// copy from -> paste to
void mirrorGrid(int **gridFrom, int **gridTo, int *numRows, int *numColumns)
{
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numColumns; j++)
        {
            gridTo[i][j] = gridFrom[i][j];
        }
    }
}

// copy from -> paste to
void mirror3Grids(int **gridFrom1, int **gridTo1, int **gridFrom2, int **gridTo2, int **gridFrom3, int **gridTo3, int *numRows, int *numColumns)
{
    mirrorGrid(gridFrom1, gridTo1, numRows, numColumns);
    mirrorGrid(gridFrom2, gridTo2, numRows, numColumns);
    mirrorGrid(gridFrom3, gridTo3, numRows, numColumns);
}

// copy from gridOrigin
void resetGridWorked(int **gridOrigin, int **gridWorked, int *numRows, int *numColumns)
{
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numColumns; j++)
        {
            gridWorked[i][j] = gridOrigin[i][j];
        }
    }
}

// copy from gridOrigin -> paste to gridGround
void resetGridGround(int **gridOrigin, int **gridGround, int *numRows, int *numColumns)
{
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numColumns; j++)
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
void resetGridMovables(int **gridOrigin, int **gridMovables, int **gridGround, int *numRows, int *numColumns)
{
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numColumns; j++)
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
    printf("o00 %d, o01 %d, nr %d, nc %d\n ", objectPosition[objectCoo][0], objectPosition[objectCoo][1], *numRows, *numColumns);
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
        // printArray(gridWorked, numRows, numColumns);
        // printf("\n");
        // printArray(gridGround, numRows, numColumns);
        // printf("\n");
        // printf("fp00 %d, fp01 %d bricks\n", firePosition[0][0], firePosition[0][1]);
        gridGround[firePosition[0][0]][firePosition[0][1]] = DIRT;
        gridWorked[firePosition[0][0]][firePosition[0][1]] = DIRT;
        // printArray(gridWorked, numRows, numColumns);
        // printf("\n");
        // printArray(gridGround, numRows, numColumns);
        // printf("findbricks\n");
        break;
        // only 2 kill
    case ANTITANKUP:
        if (*currentFireDirection == DOWN)
        {
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKUPDEAD;
            gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKUPDEAD;
            gridGround[firePosition[0][0]][firePosition[0][1]] = ANTITANKUPDEAD; // to be sure
            print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
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
            print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
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
            print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
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
            print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
            break;
        }
        else
        {
            printf("atl not kill ?\n");
        }
        break;
    default:
        printf("!! bang ft= %d fp00 %d fp01 %d\n", firedTileID, firePosition[0][0], firePosition[0][1]);
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

// Bm+mirror+AT : water case ok ~missing way and ice // can return false and continue
bool movableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns)
{
    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
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
                    // replace element in new location
                    gridGround[firePosition[0][0]][firePosition[0][1]] = WATERFULL;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = WATERFULL;
                    // erase movable block
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    // copy ground at before location
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                // isMovable always false 4 mv
                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                { // ok because water always treated
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    // return directly false if no ICE/THINICE
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MOVABLEBLOC;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("findmovable cfd %d\n", *currentFireDirection);
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
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORUPRIGHT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("findmovable cfd %d\n", *currentFireDirection);
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORRIGHTDOWN;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("findmovable cfd %d\n", *currentFireDirection);
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORDOWNLEFT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("findmovable cfd %d\n", *currentFireDirection);
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = MIRRORLEFTUP;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("findmovable cfd %d\n", *currentFireDirection);
        return false;
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKUP;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("findmovable cfd %d\n", *currentFireDirection);
        return false;
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKRIGHT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("findmovable cfd %d\n", *currentFireDirection);
        return false;
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKDOWN;
                    gridMovables[firePosition[0][0]][firePosition[0][1] + 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] + 1] = gridGround[firePosition[0][0]][firePosition[0][1] + 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
                    return true;
                }
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("findmovable cfd %d\n", *currentFireDirection);
        return false;
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridMovables[firePosition[0][0] + 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] + 1][firePosition[0][1]] = gridGround[firePosition[0][0] + 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridMovables[firePosition[0][0]][firePosition[0][1] - 1] = NOTHING;
                    gridWorked[firePosition[0][0]][firePosition[0][1] - 1] = gridGround[firePosition[0][0]][firePosition[0][1] - 1];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    return true;
                }
                if (isTunnel(gridGround[firePosition[0][0]][firePosition[0][1]]))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!tunnelTPMovables(firePosition[0][0], firePosition[0][1], gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if (isFloor(gridGround[firePosition[0][0]][firePosition[0][1]]) && !isMovable(gridMovables[firePosition[0][0]][firePosition[0][1]], currentFireDirection))
                {
                    gridMovables[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridWorked[firePosition[0][0]][firePosition[0][1]] = ANTITANKLEFT;
                    gridMovables[firePosition[0][0] - 1][firePosition[0][1]] = NOTHING;
                    gridWorked[firePosition[0][0] - 1][firePosition[0][1]] = gridGround[firePosition[0][0] - 1][firePosition[0][1]];
                    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                    if (!movableOnIce(firePosition[0][0], firePosition[0][1], *currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("onIce ko \n");
                    }
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
        print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("findmovable cfd %d\n", *currentFireDirection);
        return false;
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
    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
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
                print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                return true;
            }
            return false;
        case LEFT:
            *currentFireDirection = UP;
            firePosition[0][0] = firePosition[0][0] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                return true;
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
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
                print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                return true;
            }
            return false;
        case UP:
            *currentFireDirection = RIGHT;
            firePosition[0][1] = firePosition[0][1] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                return true;
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
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
                print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                return true;
            }
            return false;
        case RIGHT:
            *currentFireDirection = DOWN;
            firePosition[0][0] = firePosition[0][0] + 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                return true;
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
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
                print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                return true;
            }
            return false;
        case DOWN:
            *currentFireDirection = LEFT;
            firePosition[0][1] = firePosition[0][1] - 1;
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
            {
                print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
                return true;
            }
            return false;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("finddeflectable\n");
        return false;
    default:
        // printf("bangid = %d tn %d\n", firedTileID, turnNumber);
        return false;
    }
}

bool turnableAction(int firedTileID, int **firePosition, int *currentFireDirection, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns)
{
    print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
    switch (firedTileID)
    {
    case ROTATIVEMIRRORDOWNLEFT:

        switch (*currentFireDirection)
        {
        case DOWN:
        case LEFT:
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ROTATIVEMIRRORLEFTUP;
            print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
            return true;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("finddeflectable\n");
        return false;
    case ROTATIVEMIRRORLEFTUP:

        switch (*currentFireDirection)
        {
        case LEFT:
        case UP:
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ROTATIVEMIRRORUPRIGHT;
            print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
            return true;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("finddeflectable\n");
        return false;
    case ROTATIVEMIRRORUPRIGHT:
        switch (*currentFireDirection)
        {
        case UP:
        case RIGHT:
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ROTATIVEMIRRORRIGHTDOWN;
            print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
            return true;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
        printf("finddeflectable\n");
        return false;
    case ROTATIVEMIRRORRIGHTDOWN:

        switch (*currentFireDirection)
        {
        case RIGHT:
        case DOWN:
            gridWorked[firePosition[0][0]][firePosition[0][1]] = ROTATIVEMIRRORDOWNLEFT;
            print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
            return true;
        default:
            printf("X%d ", *currentFireDirection);
            return false;
        }
        print2ArrayTarget(gridWorked, gridMovables, numRows, numColumns, firePosition[0][0], firePosition[0][1]);
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

// tank exclusif + recursive
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

// only mv + recursive : false = no need roll back but copy
bool movableOnIce(int movableX, int movableY, int moveID, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns)
{
    // here on way
    printf("start ice %d\n", gridWorked[movableX][movableY]);
    // follow n+1 tile where u are
    switch (gridGround[movableX][movableY])
    {
    case ICE:
    case THINICE:
        // you r on n+1
        printf("onfirsICE %d\n", moveID);
        print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, movableX, movableY);
        if (isLegalMVMove(movableX, movableY, moveID, gridWorked, numRows, numColumns))
        { // n+2 cheked
            printf("mvx %d, mvy %d, mID %d\n", movableX, movableY, moveID);
            printf("legalICEmove\n");
            // moveMovable handle ICE/THINCE/WATER cases
            if (moveMovable(movableX, movableY, moveID, gridWorked, gridMovables, gridGround))
            {
                int *ptx = &movableX;
                int *pty = &movableY;
                printf("mvx %d, mvy %d, mID %d, ptx %d, pty %d", movableX, movableY, moveID, *ptx, *pty);
                // update xy wip
                advanceMovableCoo(ptx, pty, moveID);
                printf("mvx %d, mvy %d, mID %d, ptx %d, pty %d", movableX, movableY, moveID, *ptx, *pty);
                // check n+2
                if (isIce(gridGround[movableX][movableY]))
                {
                    if (!movableOnIce(movableX, movableY, moveID, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("%d %d %d\n", gridGround[movableX][movableY], movableX, movableY);
                        printf("return ice false\n");
                        print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, movableX, movableY);
                        return false;
                    }
                }
                else if (isTunnel(gridGround[movableX][movableY]))
                {
                    print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, movableX, movableY);
                    if (!tunnelTPMovables(movableX, movableY, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("ice mv tp not ok\n");
                    }
                    else
                    {
                        printf("ice mv tp ok\n");
                    }
                    print3ArrayTarget(gridWorked, gridMovables, gridGround, numRows, numColumns, movableX, movableY);
                    return false;
                    // bug log
                }
                else if (isFloor(gridGround[movableX][movableY]))
                {
                    return true;
                }
            }
            else
            {
                // if error or water
                printf("illegalICEmove\n");
                printf("%d %d %d\n", gridGround[movableX][movableY], movableX, movableY);
                return false;
            }
        }
        return false;
    default:
        printf("stop ice m %d\n", gridWorked[movableX][movableY]);
        return false;
    }
    printf("end of ice m\n");
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// only water ~change name ?
bool isDeathDestination(int curentTile)
{
    switch (curentTile)
    {
    case WATER:
        return true;
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
        printf("not highway id:%d\n", elementID);
        return false;
    }
    return false;
}

// True if Ice
bool isIce(int elementID)
{
    switch (elementID)
    {
    case ICE:
        return true;
    case THINICE:
        return true;
    default:
        printf("not ice id:%d\n", elementID);
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

// if brue -> thnik 0..n-1 + n+1..+00 ?
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

// every fire or move
bool antiTankAction(int **tankPosition, int tankCoo, int **gridWorked, int *numRows, int *numColumns)
{
    int rangeAT[1][2];
    // printf("ATA !!\n");
    // attention if case block but not dead so change case not return

    for (int i = UP; i <= LEFT; i++)
    {
        // printf("ATA !! %d\n", i);
        rangeAT[0][0] = tankPosition[tankCoo][0];
        rangeAT[0][1] = tankPosition[tankCoo][1];
        switch (i)
        {
        case UP:
            printf("ATA up %d tp %d\n", i, tankPosition[tankCoo][0]);
            if (tankPosition[tankCoo][0] > 0)
            {
                for (int j = 1; j <= tankPosition[tankCoo][0]; j++)
                {
                    rangeAT[0][0] = tankPosition[tankCoo][0] - j;
                    printf("uat %d %d\n", rangeAT[0][0], rangeAT[0][1]);
                    // printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                    if (gridWorked[rangeAT[0][0]][rangeAT[0][1]] == ANTITANKDOWN)
                    {
                        printf("at up killer ok\n");
                        printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                        return true;
                    }
                    if (!isFireTrought(gridWorked[rangeAT[0][0]][rangeAT[0][1]]))
                    {
                        // printf("no upped at bc bolcked\n");
                        // printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                        goto nextCross;
                    }
                    // printf("for up\n");
                }
            }
            else
            {
                // printf("no upped at\n");
                // printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                // printf("no upped at\n");
                goto nextCross;
            }
            goto nextCross;

        case RIGHT:
            printf("ATA r %d tp %d\n", i, tankPosition[tankCoo][1]);
            if (tankPosition[tankCoo][1] < *numColumns - 1)
            {
                for (int j = 1; j <= *numColumns - 1 - tankPosition[tankCoo][1]; j++)
                {
                    rangeAT[0][1] = tankPosition[tankCoo][1] + j;
                    printf("rat %d %d\n", rangeAT[0][0], rangeAT[0][1]);
                    if (gridWorked[rangeAT[0][0]][rangeAT[0][1]] == ANTITANKLEFT)
                    {
                        printf("at r killer ok\n");
                        printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                        return true;
                    }
                    if (!isFireTrought(gridWorked[rangeAT[0][0]][rangeAT[0][1]]))
                    {
                        printf("no righted at bc bolcked\n");
                        printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                        goto nextCross;
                    }
                }
            }
            else
            {
                printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                printf("no righted at\n");
                goto nextCross;
            }
            goto nextCross;

        case DOWN:
            printf("ATA d %d tp %d\n", i, tankPosition[tankCoo][0]);

            if (tankPosition[tankCoo][0] > *numRows - 1)
            {
                for (int j = 1; j <= *numRows - 1 - tankPosition[tankCoo][0]; j++)
                {
                    rangeAT[0][0] = tankPosition[tankCoo][0] + j;
                    printf("dat %d %d\n", rangeAT[0][0], rangeAT[0][1]);
                    if (gridWorked[rangeAT[0][0]][rangeAT[0][1]] == ANTITANKUP)
                    {
                        printf("at d killer ok\n");
                        printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                        return true;
                    }
                    if (!isFireTrought(gridWorked[rangeAT[0][0]][rangeAT[0][1]]))
                    {
                        // printf("no downed at bc bolcked\n");
                        // printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                        goto nextCross;
                    }
                }
            }
            else
            {
                // printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                // printf("no downed at\n");
                goto nextCross;
            }
            goto nextCross;
        case LEFT:
            printf("ATA l %d tp %d\n", i, tankPosition[tankCoo][1]);
            if (tankPosition[tankCoo][1] > 0)
            {
                for (int j = 1; j <= tankPosition[tankCoo][1]; j++)
                {
                    rangeAT[0][1] = tankPosition[tankCoo][1] - j;
                    printf("lat %d %d\n", rangeAT[0][0], rangeAT[0][1]);
                    if (gridWorked[rangeAT[0][0]][rangeAT[0][1]] == ANTITANKRIGHT)
                    {
                        printf("at l killer ok\n");
                        printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                        return true;
                    }
                    if (!isFireTrought(gridWorked[rangeAT[0][0]][rangeAT[0][1]]))
                    {
                        printf("no lefted at bc bolcked\n");
                        printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                        goto nextCross;
                    }
                }
            }
            else
            {
                // printArrayTarget(gridWorked, numRows, numColumns, rangeAT[0][0], rangeAT[0][1]);
                // printf("no lefted at\n");
                goto nextCross;
            }
            goto nextCross;
        default:
            printf("err move atAction %d\n", i);
            return false;
        }
    nextCross:
    }
    printf("no AT action\n");
    return false;
}

// only 4 tank
bool tunnelTPTank(int **tankPosition, int tankCoo, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns)
{
    // get tunnelNumber
    int tunnelColor = gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]];
    int tunnelNumber = 0;
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numColumns; j++)
        {
            if (gridGround[i][j] == tunnelColor)
            {
                tunnelNumber = tunnelNumber + 1;
            }
        }
    }
    if (tunnelNumber == 1)
    {
        printf("tp kill !!!!\n");
        return false;
    }

    printf("tunnel nmb %d colo %d\n", tunnelNumber, tunnelColor);

    // get tunnelTankCoo + fill arrayTunnel
    int arrayTunnel[tunnelNumber][2];
    tunnelNumber = 0;
    int tunnelTankCoo = 0;
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numColumns; j++)
        {
            if (gridGround[i][j] == tunnelColor)
            {
                if (tankPosition[tankCoo][0] == i && tankPosition[tankCoo][1] == j)
                {
                    tunnelTankCoo = tunnelNumber;
                }
                printf("ggij %d\n", gridGround[i][j]);
                arrayTunnel[tunnelNumber][0] = i;
                arrayTunnel[tunnelNumber][1] = j;
                tunnelNumber = tunnelNumber + 1;
            }
        }
    }

    for (int i = 0; i < tunnelNumber; i++)
    {
        printf("array Tunnel : %d ; %d\n", arrayTunnel[i][0], arrayTunnel[i][1]);
    }

    // tp to right tunnel
    for (int k = 0; k < tunnelNumber; k++)
    {
        if (k != tunnelTankCoo)
        {
            // if next tunnel empty
            if (gridWorked[arrayTunnel[k][0]][arrayTunnel[k][1]] == gridGround[arrayTunnel[k][0]][arrayTunnel[k][1]])
            {
                printf("begin tp !\n");
                printArrayBraket(gridWorked, numRows, numColumns, tankPosition[tankCoo][0], tankPosition[tankCoo][1]);

                printf("tp0 %d tp1 %d !\n", tankPosition[tankCoo][0], tankPosition[tankCoo][1]);
                printf("at0 %d at1 %d !\n", arrayTunnel[k][0], arrayTunnel[k][1]);
                gridWorked[arrayTunnel[k][0]][arrayTunnel[k][1]] = gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]];
                printArrayBraket(gridWorked, numRows, numColumns, tankPosition[tankCoo][0], tankPosition[tankCoo][1]);

                printf("gw tp %d ; gd tp %d !\n", gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]], gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]]);
                gridWorked[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]] = gridGround[tankPosition[tankCoo][0]][tankPosition[tankCoo][1]];
                printArrayBraket(gridWorked, numRows, numColumns, tankPosition[tankCoo][0], tankPosition[tankCoo][1]);

                tankPosition[tankCoo][0] = arrayTunnel[k][0];
                tankPosition[tankCoo][1] = arrayTunnel[k][1];
                printArrayBraket(gridWorked, numRows, numColumns, tankPosition[tankCoo][0], tankPosition[tankCoo][1]);
                printf("end tp !\n");
                return true;
            }
        }
    }
    printArrayBraket(gridWorked, numRows, numColumns, tankPosition[tankCoo][0], tankPosition[tankCoo][1]);
    printf("no tp...\n");
    return true;
}

// only 4 mv
bool tunnelTPMovables(int movableX, int movableY, int **gridWorked, int **gridMovables, int **gridGround, int *numRows, int *numColumns)
{
    // get tunnelNumber
    int tunnelColor = gridGround[movableX][movableY];
    int tunnelNumber = 0;
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numColumns; j++)
        {
            if (gridGround[i][j] == tunnelColor)
            {
                tunnelNumber = tunnelNumber + 1;
            }
        }
    }

    if (tunnelNumber == 0)
    {
        printf("tp m ERROR!!\n");
        return false;
    }

    if (tunnelNumber == 1)
    {
        gridWorked[movableX][movableY] = gridGround[movableX][movableY];
        gridMovables[movableX][movableY] = NOTHING;
        printf("tp eraze m!!\n");
        return true;
    }

    printf("tunnel nmb %d colo %dm\n", tunnelNumber, tunnelColor);

    // get tunnelMVCoo + fill arrayTunnel
    int arrayTunnel[tunnelNumber][2];
    tunnelNumber = 0;
    int tunnelMVCoo = 0;
    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numColumns; j++)
        {
            if (gridGround[i][j] == tunnelColor)
            {
                if (movableX == i && movableY == j)
                {
                    tunnelMVCoo = tunnelNumber;
                }
                printf("ggij m%d\n", gridGround[i][j]);
                arrayTunnel[tunnelNumber][0] = i;
                arrayTunnel[tunnelNumber][1] = j;
                tunnelNumber = tunnelNumber + 1;
            }
        }
    }

    for (int i = 0; i < tunnelNumber; i++)
    {
        printf("array Tunnel m : %d ; %d\n", arrayTunnel[i][0], arrayTunnel[i][1]);
    }

    // tp to right tunnel
    for (int k = 0; k < tunnelNumber; k++)
    {
        if (k != tunnelMVCoo)
        {
            // if next tunnel empty
            if (gridWorked[arrayTunnel[k][0]][arrayTunnel[k][1]] == gridGround[arrayTunnel[k][0]][arrayTunnel[k][1]])
            {
                printf("begin tp m!\n");
                printArrayBraket(gridWorked, numRows, numColumns, movableX, movableY);

                printf("tp0 %d tp1 %d m!\n", movableX, movableY);
                printf("at0 %d at1 %d m!\n", arrayTunnel[k][0], arrayTunnel[k][1]);
                gridWorked[arrayTunnel[k][0]][arrayTunnel[k][1]] = gridWorked[movableX][movableY];
                gridMovables[arrayTunnel[k][0]][arrayTunnel[k][1]] = gridWorked[movableX][movableY];
                printArrayBraket(gridWorked, numRows, numColumns, movableX, movableY);

                printf("gw tp %d ; gd tp %d m!\n", gridWorked[movableX][movableY], gridGround[movableX][movableY]);
                gridWorked[movableX][movableY] = gridGround[movableX][movableY];
                gridMovables[movableX][movableY] = 0;
                printArrayBraket(gridWorked, numRows, numColumns, movableX, movableY);
                printf("end tp m!\n");
                return true;
            }
        }
    }
    printArrayBraket(gridWorked, numRows, numColumns, movableX, movableY);
    printf("no tp...m\n");
    return true;
}

void advanceMovableCoo(int *movableX, int *movableY, int moveID)
{
    switch (moveID)
    {
    case UP:
        *movableX = *movableX - 1;
        break;
    case RIGHT:
        *movableY = *movableY + 1;
        break;
    case DOWN:
        *movableX = *movableX + 1;
        break;
    case LEFT:
        *movableY = *movableY - 1;
        break;
    default:
        printf("errorMove mvx %d mvy %d\n", movableX, movableY);
        break;
    }
}

// movables :
// tp . + ice only
// other +1
// destroy mv

// paral : pour chaque base

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
    printf("ok1\n");
    printf("le num est %d\n", *testMove);
    if (*testMove == FIRE)
    {
        printf("fire cursor = %d\n", *curseur);
        *curseur = *curseur + 1;
        printf("fp00 %d tp00 %d fp01 %d tp01 %d fp10 %d tp10 %d fp11 %d tp11 %d\n", firePosition[0][0], tankPosition[0][0],
               firePosition[0][1], tankPosition[0][1],
               firePosition[1][0], tankPosition[0][0],
               firePosition[1][1], tankPosition[0][1]);
        // reset position at origin
        firePosition[0][0] = tankPosition[0][0];
        firePosition[0][1] = tankPosition[0][1];
        firePosition[1][0] = tankPosition[0][0];
        firePosition[1][1] = tankPosition[0][1];
        printf("fp00 %d tp00 %d fp01 %d tp01 %d fp10 %d tp10 %d fp11 %d tp11 %d\n", firePosition[0][0], tankPosition[0][0],
               firePosition[0][1], tankPosition[0][1],
               firePosition[1][0], tankPosition[0][0],
               firePosition[1][1], tankPosition[0][1]);
        printf("ctd %d gw0 %d cfd %d\n",
               *currentTankDirection, gridWorked[tankPosition[0][0]][tankPosition[0][1]], *currentFireDirection);
        // set aim direction
        *currentTankDirection = gridWorked[tankPosition[0][0]][tankPosition[0][1]];
        *currentFireDirection = *currentTankDirection;
        printf("ctd %d gw0 %d cfd %d\n",
               *currentTankDirection, gridWorked[tankPosition[0][0]][tankPosition[0][1]], *currentFireDirection);

        // position + 1
        getFirstShootNextCoo(tankPosition, firePosition, currentTankDirection);
        // print2ArrayBraket(gridWorked, gridGround, numRows, numColumns, tankPosition[0][0],tankPosition[0][1]);
        printf("2fp00 %d tp00 %d fp01 %d tp01 %d fp10 %d tp10 %d fp11 %d tp11 %d\n", firePosition[0][0], tankPosition[0][0],
               firePosition[0][1], tankPosition[0][1],
               firePosition[1][0], tankPosition[0][0],
               firePosition[1][1], tankPosition[0][1]);
        fireDead = true;
        printf("inspect border nr%d nc%d\n", *numRows, *numColumns);
        if (!isOutOfBorder(firePosition, 0, numRows, numColumns))
        {
            firedTileID = gridWorked[firePosition[0][0]][firePosition[0][1]];
            printf("ft %d\n", firedTileID);
            fireDead = false;
        }
        else
        {
            return false;
        }

        while (!(isOutOfBorder(firePosition, 0, numRows, numColumns) || isFireStop(firedTileID) || fireDead))
        {
            printf("on fire\n");
            fireDead = false;

            if (firePosition[0][0] == tankPosition[0][0] && firePosition[0][1] == tankPosition[0][1])
            {
                return false;
            }
            else if (isFireTrought(firedTileID))
            {
                printf("throught \n");
                getFirstShootNextCoo(firePosition, firePosition, currentFireDirection);
                // print2ArrayBraket(gridWorked, gridGround, numRows, numColumns, tankPosition[0][0], tankPosition[0][1]);
                fireDead = false;
                goto nextFirePosition;
            }
            // or explode
            else if (isShootable(firedTileID, currentFireDirection))
            {
                printf("bang , cursor = %d\n", *curseur);
                shotableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns);
                // when brick explode
                if (antiTankAction(tankPosition, 0, gridWorked, numRows, numColumns))
                {
                    mirror3Grids(gridWorkedCopy, gridWorked, gridMovablesCopy, gridMovables, gridGroundCopy, gridGround, numRows, numColumns);
                    fireDead = false;
                    return false;
                }
                fireDead = true;
                return true;
            }
            // or move
            else if (isMovable(firedTileID, currentFireDirection) || firedTileID == MOVABLEBLOC)
            {
                // tunnel case in movableAction
                // save 0 tp and grids
                // if at false : save n+1
                switch (firedTileID)
                {
                case MOVABLEBLOC:
                case MIRRORUPRIGHT:
                case MIRRORRIGHTDOWN:
                case MIRRORDOWNLEFT:
                case MIRRORLEFTUP:
                case ANTITANKUP:
                case ANTITANKRIGHT:
                case ANTITANKDOWN:
                case ANTITANKLEFT:
                    printf("move cursor = %d\n", *curseur);
                    // printf("f00 = %d, f01 %d, forientation = %d\n", firePosition[0][0], firePosition[0][1], *currentFireDirection);
                    if (movableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        printf("moved ok \n");
                        return true;
                    }
                    else
                    {
                        printf("moved NO \n");
                        return false;
                    }
                    break;
                default:
                    printf("!!! ismov firID %d\n", firedTileID);
                    return false;
                    break;
                }
                if (antiTankAction(tankPosition, 0, gridWorked, numRows, numColumns))
                {
                    printf("ata DEADt\n");
                    mirror3Grids(gridWorkedCopy, gridWorked, gridMovablesCopy, gridMovables, gridGroundCopy, gridGround, numRows, numColumns);
                    fireDead = false;
                    return false;
                }
                fireDead = true;
                return true;
            }
            // or deflect
            else if (isFireDeflect(firedTileID, currentFireDirection))
            {
                printf("deflected, cursor = %d\n", *curseur);
                if (deflectableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                {
                    printf("deflected ok \n");
                    goto nextFirePosition;
                }
                else
                {
                    printf("deflected NO \n");
                    return false;
                }
            }
            // or turn
            else if (isTurnable(firedTileID, currentFireDirection))
            {
                // printf("turned, cursor = %d\n", *curseur);
                if (turnableAction(firedTileID, firePosition, currentFireDirection, gridWorked, gridMovables, gridGround, numRows, numColumns))
                {
                    // printf("turned ok \n");
                    return true;
                }
                else
                {
                    // printf("turned NO \n");
                    return false;
                }
            }
            // or error...
            else
            {
                // printf("!!! elif fireTiled firedTileID=%d ; currentFireDirection=%d\n", firedTileID, *currentFireDirection);
                // printf("!!! elif fp00=%d ; fp01=%d\n", firePosition[0][0], firePosition[0][1]);
                resetGridWorked(gridOrigin, gridWorked, numRows, numColumns);
                resetGridWorked(gridOrigin, gridWorkedCopy, numRows, numColumns);
                // printArray(gridWorked, numRows, numColumns);
                fireDead = true;
                return false;
            }
        nextFirePosition:
            // printf("endFire \n");
            if (!isOutOfBorder(firePosition, 0, numRows, numColumns) && !fireDead)
            {
                // printf("NotOut fp00 = %d, fp01 = %d, cursor = %d\n", firePosition[0][0], firePosition[0][1], *curseur);
                firedTileID = gridWorked[firePosition[0][0]][firePosition[0][1]];
                fireDead = false;
            }
            else
            {
                // printf("\nyou're finished :\n");
                // printf("Out fp00 = %d, fp01 = %d, cursor = %d\n\n", firePosition[0][0], firePosition[0][1], *curseur);
                return false;
            }
        }
    }
    else if (gridWorked[tankPosition[0][0]][tankPosition[0][1]] != *testMove && *testMove != FIRE)
    {
        // printf("turn cursor = %d\n\n", *curseur);
        gridWorked[tankPosition[0][0]][tankPosition[0][1]] = *testMove;
        *currentTankDirection = gridWorked[tankPosition[0][0]][tankPosition[0][1]];
        return true;
    }
    else
    {
        // printf("move cursor = %d\n\n", *curseur);
        // here cases with ice, way, enemy, tunnel
        // if n+1 ok
        if (isLegalMove(tankPosition, 0, *testMove, gridWorked, numRows, numColumns))
        {
            // mirror to begin
            mirror3Grids(gridWorked, gridWorkedCopy, gridMovables, gridMovablesCopy, gridGroundCopy, gridGround, numRows, numColumns);
            mirrorPosition(tankPosition, 0, 1);
            // printf("test hiw\n");
            // check n+1 = H
            if (nextHighWay(tankPosition, 1, *testMove, gridWorked))
            {
                // ok so move on H
                if (moveTank(tankPosition, 1, *testMove, gridWorked, gridGround))
                {
                    // you r on way so what way ?
                    if (onFirstHighWay(tankPosition, *testMove, gridWorked, gridMovables, gridGround, numRows, numColumns))
                    {
                        if (isTunnel(gridGround[tankPosition[1][0]][tankPosition[1][1]]))
                        {
                            if (!tunnelTPTank(tankPosition, 1, gridWorked, gridMovables, gridGround, numRows, numColumns))
                            {
                                // printf("TP DEADt\n");
                                goto deadHighWay;
                            }
                        }
                        if (antiTankAction(tankPosition, 1, gridWorked, numRows, numColumns))
                        {
                            // printf("ata DEADt\n");
                            goto deadHighWay;
                        }
                        mirrorPosition(tankPosition, 1, 0);
                        // printf("testmove %d\n", *testMove);
                        // printMovingLettersWithoutPointor(deplacementsHypotheseMH, *curseur);
                        gridWorked[tankPosition[0][0]][tankPosition[0][1]] = *testMove;
                        *currentTankDirection = *testMove;
                        // printf("currentTankDirection %d\n", *currentTankDirection);
                        return true;
                    }
                    else
                    {
                    // forget all
                    deadHighWay:
                        mirror3Grids(gridWorkedCopy, gridWorked, gridMovablesCopy, gridMovables, gridGroundCopy, gridGround, numRows, numColumns);
                        mirrorPosition(tankPosition, 0, 1);
                        return false;
                    }
                }
            }
            else
            {
                if (moveTank(tankPosition, 0, *testMove, gridWorked, gridGround))
                {
                    // printArrayBraket(gridWorked, numRows, numColumns, tankPosition[0][0], tankPosition[0][1]);
                    // printf("bf tp? gd%d tp10 %d tp11 %d\n", gridGround[tankPosition[0][0]][tankPosition[0][1]], tankPosition[0][0], tankPosition[0][1]);
                    if (isTunnel(gridGround[tankPosition[0][0]][tankPosition[0][1]]))
                    {
                        // printf("it's tuinnel\n");
                        if (!tunnelTPTank(tankPosition, 0, gridWorked, gridMovables, gridGround, numRows, numColumns))
                        {
                            // printf("TP DEAD only mv\n");
                            mirror3Grids(gridWorkedCopy, gridWorked, gridMovablesCopy, gridMovables, gridGroundCopy, gridGround, numRows, numColumns);
                            mirrorPosition(tankPosition, 1, 0);
                            return false;
                        }
                    }
                    if (antiTankAction(tankPosition, 0, gridWorked, numRows, numColumns))
                    {
                        // printf("ata DEADt only mv\n");
                        mirror3Grids(gridWorkedCopy, gridWorked, gridMovablesCopy, gridMovables, gridGroundCopy, gridGround, numRows, numColumns);
                        mirrorPosition(tankPosition, 1, 0);
                        return false;
                    }
                    else
                    {
                        // printf("ok no at\n");
                        mirrorPosition(tankPosition, 0, 1);
                        return true;
                    }
                }
            }
        }
        else
        {
            // printf("nonlegal \n");
            return false;
        }
    }

    // printf("2curserMH %d ; 2curserH %d\n", *curseurDeplacementsMH, *curseurDeplacementsHypothese);

    ////////
    return true;
}