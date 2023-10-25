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
// grid
bool isFloor(int elementID);
bool isOutOfBorder(int **objectPosition, int numRows, int numColumns);
// tank
bool isLegalMove(int **arrayTankCell, int moveID, int **arrayGrid, int nbRows, int nbColumns);
// elements
bool isMovable(int elementID, int positionID);
bool isMovableAtBeginning(int elementID);
bool isShootable(int elementID, int positionID);
bool isTurnable(int elementID, int positionID);
bool isUnMovable(int elementID); // dep
                                 // fire
bool isFireTrought(int elementID);
bool isFireDeflect(int elementID, int positionID);
bool isFireStop(int elementID);

// Ground
bool nextFloor(int **arrayTankCell, int moveID, int **arrayGrid);

// Tank
bool moveTank(int **tankPosition, int testMoveID, int **gridWorked, int **gridGround);

// Grids
void resetGridWorked(int **gridOrigin, int **gridWorked, int numRows, int numCols);
void resetGridGround(int **gridOrigin, int **gridGround, int numRows, int numCols);
void resetGridMovables(int **gridOrigin, int **gridMovables, int numRows, int numCols);

// depreci
void moveOnGrid(int **arrayTankCell, int moveID, int **arrayGrid);
bool legalFire(int **arrayTankCell, int positionID, int **arrayGrid);
bool autoKill(int **arrayTankCell, int positionID, int **arrayGrid);
//////////////////////////////////////////////////////////////////
// Reading Functions //
void printArray(int **array, int rows, int cols);
void printMovingLetters(int *array, int curseur);
int replayDeplacements(char deplacementLetter);

//////////////////////////////////////////////////////////////////
// Structs //
struct ConversionTable
{
    const wchar_t *lettre;
    int valeur;
};

struct ConversionTable tableConversionSimple[] = {
    {L"Tu", 1}, {L"Tright", 2}, {L"Tdown", 3}, {L"Tleft", 4}, {L"D", 5}, {L"b", 6}, {L"w", 7}, {L"Bs", 8}, {L"Bm", 9}, {L"B", 10}, {L"Au", 11}, {L"Ar", 12}, {L"Ad", 13}, {L"Al", 14}, {L"Mur", 15}, {L"Mdr", 16}, {L"Mdl", 17}, {L"Mul", 18}, {L"Wu", 19}, {L"Wr", 20}, {L"Wd", 21}, {L"Wl", 22}, {L"C", 23}, {L"Rur", 24}, {L"Rdr", 25}, {L"Rdl", 26}, {L"Rul", 27}, {L"I", 28}, {L"i", 29}, {L"Tr", 30}, {L"Tg", 31}, {L"Tb", 32}, {L"Tc", 33}, {L"Ty", 34}, {L"Tp", 35}, {L"Tw", 36}, {L"Td", 37}, {L"Aud", 38}, {L"Ard", 39}, {L"Add", 40}, {L"Ald", 41}};

struct ConversionTable tableConversionBis[] = {
    {L"Tu\n", 1}, {L"Tright\n", 2}, {L"Tdown\n", 3}, {L"Tleft\n", 4}, {L"D\n", 5}, {L"b\n", 6}, {L"w\n", 7}, {L"Bs\n", 8}, {L"Bm\n", 9}, {L"B\n", 10}, {L"Au\n", 11}, {L"Ar\n", 12}, {L"Ad\n", 13}, {L"Al\n", 14}, {L"Mur\n", 15}, {L"Mdr\n", 16}, {L"Mdl\n", 17}, {L"Mul\n", 18}, {L"Wu\n", 19}, {L"Wr\n", 20}, {L"Wd\n", 21}, {L"Wl\n", 22}, {L"C\n", 23}, {L"Rur\n", 24}, {L"Rdr\n", 25}, {L"Rdl\n", 26}, {L"Rul\n", 27}, {L"I\n", 28}, {L"i\n", 29}, {L"Tr\n", 30}, {L"Tg\n", 31}, {L"Tb\n", 32}, {L"Tc\n", 33}, {L"Ty\n", 34}, {L"Tp\n", 35}, {L"Tw\n", 36}, {L"Td\n", 37}, {L"Aud\n", 38}, {L"Ard\n", 39}, {L"Add\n", 40}, {L"Ald\n", 41}};

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
    ANTITANKLEFTDEAD
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
    const char *filename = "testing.lt4";
    const int CYCLES = 5;

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

    int **firePosition = (int **)malloc((2) * sizeof(int *));
    for (int i = 0; i < 2; i++)
    {
        firePosition[i] = (int *)malloc((2) * sizeof(int));
    }

    int *deplacementsHypothese = (int *)malloc((1000000) * sizeof(int));
    int *deplacementsHypotheseMH = (int *)malloc((1000000) * sizeof(int));
    int *deplacementsRetenu = (int *)malloc((1000000) * sizeof(int));

    int curseurDeplacementsHypothese = 0;
    int curseurDeplacementsMH = 0;
    int curseurDeplacementsRetenu = 0;

    int objectiveFunctionHypothese = 0;
    int objectiveFunctionMH = 0;
    int objectiveFunctionRetenu = 0;

    size_t deplacementsSize = sizeof(int) * 1000000;
    wchar_t header[1000];
    //////////////////////////////////////////////////////////////////
    // Annexe Var //
    int numRows = 0;
    int numColumns = 0;
    int numBases = 0;
    int currentTankDirection = 0;
    int currentFireDirection = 0;
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

        for (int j = 0; j < numColumns; j++)
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
                        tankPosition[1][0] = i;
                        tankPosition[0][1] = j;
                        tankPosition[1][1] = j;
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
                    if (isMovableAtBeginning(tableConversionSimple[k].valeur))
                    {
                        gridMovables[i][j] = tableConversionSimple[k].valeur;
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

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // First "Solution" //
    printf("test\n");

    int curseur = 0;
    int turnNumber = 0;

    // here, if blanc it's cool
    for (int w = 0; w < curseur; w++)
    {
        printf("%d|", deplacementsHypothese[w]);
    }
    printf("\n");
    memset(deplacementsHypothese, -1, deplacementsSize);
    for (int w = 0; w < curseur; w++)
    {
        printf("%d|", deplacementsHypothese[w]);
    }
    printf("\n");

    while (!(tankPosition[0][0] == basesPosition[0][0] &&
             tankPosition[0][1] == basesPosition[0][1]))
    {
        int testMove = getRandomMove();

        if (testMove == FIRE)
        {
            deplacementsHypothese[curseur] = testMove;
            deplacementsRetenu[curseur] = testMove;
            turnNumber--;
            curseur++;
            turnNumber++;
            objectiveFunctionHypothese = objectiveFunctionHypothese - 2;
            objectiveFunctionRetenu = objectiveFunctionRetenu - 2;
        }
        else if (gridWorked[tankPosition[0][0]][tankPosition[0][1]] != testMove)
        {
            gridWorked[tankPosition[0][0]][tankPosition[0][1]] = testMove;
            deplacementsHypothese[curseur] = testMove;
            deplacementsRetenu[curseur] = testMove;
            turnNumber--;
            curseur++;
            turnNumber++;
        }
        else
        {
            if (isLegalMove(tankPosition, testMove, gridWorked, numRows, numColumns))
            {
                if (moveTank(tankPosition, testMove, gridWorked, gridGround))
                {
                    deplacementsHypothese[curseur] = testMove;
                    deplacementsRetenu[curseur] = testMove;
                    curseur++;
                    turnNumber++;
                    objectiveFunctionHypothese--;
                    objectiveFunctionRetenu--;
                }
            }
            else
            {
                // printf("nonlegal\n");
            }
        }
    }

    curseurDeplacementsHypothese = curseur;
    curseurDeplacementsRetenu = curseur;
    printf("curserH %d ; curserR %d\n", curseurDeplacementsHypothese, curseurDeplacementsRetenu);

    //////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // wip

    bool fireDead = false;
    int intermEphemere = 0;
    int firedTileID = 0;

    do
    {
        // reset grid
        resetGridWorked(gridOrigin, gridWorked, numRows, numColumns);
        resetGridGround(gridOrigin, gridGround, numRows, numColumns);
        resetGridMovables(gridOrigin, gridMovables, numRows, numColumns);
        // reset cmpt
        turnNumber = 0;
        curseur = 0;
        curseurDeplacementsMH = 0;
        objectiveFunctionMH = 0;
        // reset HP
        memset(deplacementsHypotheseMH, -1, deplacementsSize);
        // reset Tank
        intermEphemere = tankPosition[1][0];
        tankPosition[0][0] = intermEphemere;
        intermEphemere = tankPosition[1][1];
        tankPosition[0][1] = intermEphemere;
        // stop if tank == base
        while (!(tankPosition[0][0] == basesPosition[0][0] &&
                 tankPosition[0][1] == basesPosition[0][1]) ||
               turnNumber < 50)
        {
            int testMove = getRandomMove();

            if (testMove == FIRE)
            {
                // reset position at origin
                intermEphemere = 0;
                intermEphemere = tankPosition[0][0];
                firePosition[0][0] = intermEphemere;
                intermEphemere = tankPosition[0][1];
                firePosition[0][1] = intermEphemere;
                intermEphemere = tankPosition[0][0];
                firePosition[1][0] = intermEphemere;
                intermEphemere = tankPosition[0][1];
                firePosition[1][1] = intermEphemere;

                // set aim direction
                currentTankDirection = gridWorked[tankPosition[0][0]][tankPosition[0][1]];
                currentFireDirection = currentTankDirection;
                printArray(gridWorked, numRows, numColumns);
                printf("ctd %d, cfd %d, tp00 %d, tp 01 %d, gwtt %d\n", currentTankDirection, currentFireDirection, tankPosition[0][0], tankPosition[0][0], gridWorked[tankPosition[0][0]][tankPosition[0][1]]);

                // position + 1
                switch (currentTankDirection)
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
                    printf("errorMov %d\n");
                    break;
                }
                printf("fp00 %d, fp01 %d\n", firePosition[0][0], firePosition[0][1]);
                // bug out of borns

                /////////////////////////////////////////////////////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////////////////////////////////////////////////////
                // (isOutOfBorder(firePosition, numRows, numColumns) || isFireStop(firedTileID)

                // printf("\n ctp = %d, cfp = %d\n", currentTankDirection, currentFireDirection);
                // printf("tp00 %d ,tp01%d\n", tankPosition[0][0], tankPosition[0][1]);
                // printf("test outb%d, fp00 %d ,fp01%d\n", isOutOfBorder(firePosition, numRows, numColumns), firePosition[0][0], firePosition[0][1]);
                // printf("isFireStop(firedTileID) = %d, test %d\n", isFireStop(firedTileID), !(isOutOfBorder(firePosition, numRows, numColumns) || isFireStop(firedTileID) || fireDead));
                fireDead = true;
                if (!isOutOfBorder(firePosition, numRows, numColumns))
                {
                    firedTileID = gridWorked[firePosition[0][0]][firePosition[0][1]];
                    printf("ft %d tn %d\n", firedTileID, turnNumber);
                    fireDead = false;
                }
                // while // wip weloop
                while (!(isOutOfBorder(firePosition, numRows, numColumns) || isFireStop(firedTileID) || fireDead) && turnNumber < 50)
                {
                    // printf("im here\n");
                    // printf("outB = %d; fp00 = %d; fp01 = %d; nr = %d, nc = %d\n", isOutOfBorder(firePosition, numRows, numColumns), firePosition[0][0], firePosition[0][1], numRows, numColumns);
                    // printf("ifs = %d; ft = %d\n", isFireStop(firedTileID), firedTileID);
                    // either just pass
                    // printf("brick10 = %d\n", gridGround[firePosition[0][0]][firePosition[0][1]]);
                    // printArray(gridGround, numRows, numColumns);
                    // if (firePosition[0][0]>=numRows || firePosition[0][0]<0 || firePosition[0][1]>=numColumns|| firePosition[0][1]<0){
                    //    printf("fp00 %d, fp01 %d \n",firePosition[0][0], firePosition[0][1]);
                    //    printf("r %d, c %d \n",numRows, numColumns);
                    // }
                    // printf("fp00 %d, fp01 %d \n",firePosition[0][0], firePosition[0][1]);
                    printf("bf\n");
                    firedTileID = gridWorked[firePosition[0][0]][firePosition[0][1]];
                    printf("tile %d, ift = %d; is = %d\n", firedTileID, isFireTrought(firedTileID), isShootable(firedTileID, currentFireDirection));
                    printf("af\n");
                    fireDead = false;
                    if (isFireTrought(firedTileID))
                    {
                        printf("throught\n");
                        switch (currentFireDirection)
                        {
                        case UP:
                            firePosition[0][0] = firePosition[0][0] - 1;
                            break;
                        case RIGHT:
                            firePosition[0][1] = firePosition[0][1] + 1;
                            break;
                        case DOWN:
                            firePosition[0][0] = firePosition[0][0] + 1;
                            break;
                        case LEFT:
                            firePosition[0][1] = firePosition[0][1] - 1;
                            break;
                        default:
                            break;
                        }
                        fireDead = false;
                        goto nextFirePosition;
                    }
                    // or explode
                    else if (isShootable(firedTileID, currentFireDirection))
                    {
                        printf("bang %d\n", turnNumber);
                        switch (firedTileID)
                        {
                        case BRICKS:
                            printArray(gridWorked, numRows, numColumns);
                            printf("\n");
                            printArray(gridGround, numRows, numColumns);
                            printf("\n");
                            printf("fp00 %d, fp01 %d bricks\n", firePosition[0][0], firePosition[0][1]);
                            gridGround[firePosition[0][0]][firePosition[0][1]] = DIRT;
                            gridWorked[firePosition[0][0]][firePosition[0][1]] = DIRT;
                            printArray(gridWorked, numRows, numColumns);
                            printf("\n");
                            printArray(gridGround, numRows, numColumns);
                            printf("findbricks\n");
                            break;

                        default:
                            printf("bangid = %d tn %d\n", firedTileID, turnNumber);
                            break;
                        }
                        fireDead = true;
                        goto nextFirePosition;
                    }
                    // // or move
                    // else if (isMovable(firedTileID, currentFireDirection) || gridWorked[firePosition[0][0]][firePosition[0][1]] == MOVABLEBLOC)
                    // {
                    //     fireDead = true;
                    // }
                    // // or deflect
                    // else if (isFireDeflect(firedTileID, currentFireDirection))
                    // {
                    //     printf("deflected\n");
                    //     fireDead = true; // only 4 test :-> erase
                    // }
                    // // or turn
                    // else if (isTurnable(firedTileID, currentFireDirection))
                    // {
                    //     printf("turned\n");
                    //     fireDead = true;
                    // }
                    // // or error...
                    else
                    {
                        printf("elif fireTiled firedTileID=%d ; currentFireDirection=%d\n", firedTileID, currentFireDirection);
                        fireDead = true;
                    }
                nextFirePosition:
                    printf("endFire %d\n", turnNumber);
                }
                printf("endFireCmpt %d\n", turnNumber);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////////////////////////////////////////////////////
                // what appended for this new position?
                // check out of borns and stopped
                printf("es %d\n", turnNumber);
                deplacementsHypotheseMH[curseur] = testMove;
                curseur++;
                turnNumber++;
                objectiveFunctionMH = objectiveFunctionMH - 2;
                printf("es2 %d\n", turnNumber);
            }
            else if (gridWorked[tankPosition[0][0]][tankPosition[0][1]] != testMove && testMove != FIRE)
            {
                printf("rote1 %d\n", turnNumber);
                gridWorked[tankPosition[0][0]][tankPosition[0][1]] = testMove;
                deplacementsHypotheseMH[curseur] = testMove;
                turnNumber--;
                curseur++;
                turnNumber++;
                printf("rote %d\n", turnNumber);
            }
            else
            {
                if (isLegalMove(tankPosition, testMove, gridWorked, numRows, numColumns))
                {
                    if (moveTank(tankPosition, testMove, gridWorked, gridGround))
                    {
                        printf("mv1 %d\n", turnNumber);
                        deplacementsHypotheseMH[curseur] = testMove;
                        curseur++;
                        turnNumber++;
                        objectiveFunctionMH--;
                        printf("mv %d\n", turnNumber);
                    }
                    printf("legal %d\n", turnNumber);
                }
                else
                {
                    printf("nonlegal %d\n", turnNumber);
                }
                printf("endmv %d\n", turnNumber);
            }
            printf("endAction %d\n", turnNumber);
        }
        curseurDeplacementsMH = curseur;
        printf("2curserMH %d ; 2curserH %d\n", curseurDeplacementsMH, curseurDeplacementsHypothese);

    } while (curseurDeplacementsMH > 50 && turnNumber < 50);
    // > 20 not fast at all, 30 -> 5 seconds
    // while (curseurDeplacementsMH > curseurDeplacementsHypothese);

    curseurDeplacementsMH = curseur;
    printf("curserHM %d\n", curseurDeplacementsMH);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Write Output //

    // //////////////////////////////////////////////////////////////////
    // // Replay //

    // // need further modif
    // tankPosition[0][0] = tankPosition[1][0];
    // tankPosition[0][1] = tankPosition[1][1];
    // resetGridWorked(gridOrigin, gridWorked, numRows, numColumns);

    // // prendre la méta heur pour l'instant et son curseur
    // // faire la boucle de déplacement et afficher

    // for (int i = 0; i < curseurDeplacementsMH; i++)
    // {
    //     printf("Move N° %d;\n", i);
    //     if (deplacementsHypotheseMH[i] == FIRE)
    //     {
    //         printArray(gridWorked, numRows, numColumns);
    //     }
    //     else if (gridWorked[tankPosition[0][0]][tankPosition[0][1]] != deplacementsHypotheseMH[i])
    //     {
    //         gridWorked[tankPosition[0][0]][tankPosition[0][1]] = deplacementsHypotheseMH[i];
    //         printArray(gridWorked, numRows, numColumns);
    //     }
    //     else
    //     {
    //         if (isLegalMove(tankPosition, deplacementsHypotheseMH[i], gridWorked, numRows, numColumns))
    //         {
    //             if (moveTank(tankPosition, deplacementsHypotheseMH[i], gridWorked, gridGround))
    //             {
    //                 printArray(gridWorked, numRows, numColumns);
    //             }
    //         }
    //         else
    //         {
    //             // printf("nonlegal\n");
    //         }
    //     }
    // }

    //////////////////////////////////////////////////////////////////
    // Display //
    printf("gridOrigin = lignes : %d et colonnes : %d\n", numRows, numColumns);
    printArray(gridOrigin, numRows, numColumns);
    printf("gridWorked = lignes : %d et colonnes : %d\n", numRows, numColumns);
    printArray(gridWorked, numRows, numColumns);
    printf("gridGround = lignes : %d et colonnes : %d\n", numRows, numColumns);
    printArray(gridGround, numRows, numColumns);
    printf("gridMovables = lignes : %d et colonnes : %d\n", numRows, numColumns);
    printArray(gridMovables, numRows, numColumns);
    printf("tankp\n");
    printArray(tankPosition, 2, 2);
    printf("basesp\n");
    printArray(basesPosition, numBases, 2);
    printf("meta\n");
    printMovingLetters(deplacementsHypotheseMH, curseurDeplacementsMH);
    printf("hyp\n");
    printMovingLetters(deplacementsHypothese, curseurDeplacementsHypothese);
    printf("retenu\n");
    printMovingLetters(deplacementsRetenu, curseurDeplacementsRetenu);

    //////////////////////////////////////////////////////////////////
    // Free Memory //
    if (gridOrigin != NULL)
    {
        for (int i = 0; i < numRows; i++)
        {
            free(gridOrigin[i]);
        }
        free(gridOrigin);
    }

    if (gridWorked != NULL)
    {
        for (int i = 0; i < numRows; i++)
        {
            free(gridWorked[i]);
        }
        free(gridWorked);
    }

    if (gridGround != NULL)
    {
        for (int i = 0; i < numRows; i++)
        {
            free(gridGround[i]);
        }
        free(gridGround);
    }

    if (gridMovables != NULL)
    {
        for (int i = 0; i < numRows; i++)
        {
            free(gridMovables[i]);
        }
        free(gridMovables);
    }

    if (tankPosition != NULL)
    {
        for (int i = 0; i < 2; i++)
        {
            free(tankPosition[i]);
        }
        free(tankPosition);
    }

    if (firePosition != NULL)
    {
        for (int i = 0; i < 2; i++)
        {
            free(firePosition[i]);
        }
        free(firePosition);
    }

    if (basesPosition != NULL)
    {
        for (int i = 0; i < 5; i++)
        {
            free(basesPosition[i]);
        }
        free(basesPosition);
    }

    free(deplacementsHypotheseMH);
    free(deplacementsHypothese);
    free(deplacementsRetenu);

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

void printMovingLetters(int *array, int curseur)
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

//////////////////////////////////////////////////////////////////
// Array Functions //

// dep
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
    int randomNumber = rand() % 5;
    return randomNumber;
}

//////////////////////////////////////////////////////////////////
// Elements Functions //
bool isLegalMove(int **arrayTankCell, int moveID, int **arrayGrid, int nbRows, int nbColumns)
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

bool legalFire(int **arrayTankCell, int positionID, int **arrayGrid)
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
    switch (moveID)
    {
    case FIRE:
        // printf("F");
        return true;
    case UP:
        // check if future ground is ok
        if (isFloor(arrayGrid[arrayTankCell[0][0] - 1][arrayTankCell[0][1]]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case RIGHT:
        if (isFloor(arrayGrid[arrayTankCell[0][0]][arrayTankCell[0][1] + 1]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case DOWN:
        if (isFloor(arrayGrid[arrayTankCell[0][0] + 1][arrayTankCell[0][1]]))
        {
            return true;
        }
        else
        {
            return false;
        }
    case LEFT:
        if (isFloor(arrayGrid[arrayTankCell[0][0]][arrayTankCell[0][1] - 1]))
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
    default:
        // printf("other floor %d\n", floorID);
        return false;
    }
    return false;
}

bool isMovable(int elementID, int positionID)
{
    switch (elementID)
    {
    // case MOVABLEBLOC:
    //     return true;
    // need n+1
    case ANTITANKUP:
        // positionID point to distination
        if (positionID == DOWN)
        {
            // hear, this is killed
            return false;
        }
        // hear, this is moved
        return true;
    case ANTITANKRIGHT:
        if (positionID == LEFT)
        {
            return false;
        }
        return true;
    case ANTITANKDOWN:
        if (positionID == UP)
        {
            return false;
        }
        return true;
    case ANTITANKLEFT:
        if (positionID == RIGHT)
        {
            return false;
        }
        return true;
    case MIRRORUPRIGHT:
        // positionID point to distination
        if (positionID == DOWN || positionID == LEFT)
        {
            // is deflected
            return false;
        }
        // isMoved
        return true;
    case MIRRORRIGHTDOWN:
        if (positionID == LEFT || positionID == UP)
        {
            return false;
        }
        return true;
    case MIRRORDOWNLEFT:
        if (positionID == UP || positionID == RIGHT)
        {
            return false;
        }
        return true;
    case MIRRORLEFTUP:
        if (positionID == RIGHT || positionID == DOWN)
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

bool isShootable(int elementID, int positionID)
{
    switch (elementID)
    {
    case BRICKS:
        printf("isBrick\n");
        return true;
    // positionID point to distination
    case ANTITANKUP:
        if (positionID == DOWN)
        {
            // hear, this is killed
            return true;
        }
        return false;
    case ANTITANKRIGHT:
        if (positionID == LEFT)
        {
            return true;
        }
        return false;
    case ANTITANKDOWN:
        if (positionID == UP)
        {
            return true;
        }
        return false;
    case ANTITANKLEFT:
        if (positionID == RIGHT)
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

// dep
bool isUnMovable(int elementID)
{
    switch (elementID)
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
    case ANTITANKUPDEAD:
        return true;
    case ANTITANKRIGHTDEAD:
        return true;
    case ANTITANKDOWNDEAD:
        return true;
    case ANTITANKLEFTDEAD:
        return true;
    default:
        // printf("other movable %d\n", unMovableID);
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

bool isFireDeflect(int elementID, int positionID)
{
    switch (elementID)
    {
    case MIRRORUPRIGHT:
        // positionID point to distination
        if (positionID == UP || positionID == RIGHT)
        {
            // is moved
            return false;
        }
        // isDeflect
        return true;
    case MIRRORRIGHTDOWN:
        if (positionID == RIGHT || positionID == DOWN)
        {
            return false;
        }
        return true;
    case MIRRORDOWNLEFT:
        if (positionID == DOWN || positionID == LEFT)
        {
            return false;
        }
        return true;
    case MIRRORLEFTUP:
        if (positionID == LEFT || positionID == UP)
        {
            return false;
        }
        return true;
    case ROTATIVEMIRRORUPRIGHT:
        // positionID point to distination
        if (positionID == UP || positionID == RIGHT)
        {
            // is turned
            return false;
        }
        // isDeflect
        return true;
    case ROTATIVEMIRRORRIGHTDOWN:
        if (positionID == RIGHT || positionID == DOWN)
        {
            return false;
        }
        return true;
    case ROTATIVEMIRRORDOWNLEFT:
        if (positionID == DOWN || positionID == LEFT)
        {
            return false;
        }
        return true;
    case ROTATIVEMIRRORLEFTUP:
        if (positionID == LEFT || positionID == UP)
        {
            return false;
        }
        return true;
    default:
        printf("other deflect %d\n", positionID);
        return false;
    }
    return false;
}

bool isTurnable(int elementID, int positionID)
{
    switch (elementID)
    {
    case ROTATIVEMIRRORUPRIGHT:
        // positionID point to distination
        if (positionID == UP || positionID == RIGHT)
        {
            // isTurned
            return true;
        }
        // is deflected
        return false;
    case ROTATIVEMIRRORRIGHTDOWN:
        if (positionID == RIGHT || positionID == DOWN)
        {
            return true;
        }
        return false;
    case ROTATIVEMIRRORDOWNLEFT:
        if (positionID == DOWN || positionID == LEFT)
        {
            return true;
        }
        return false;
    case ROTATIVEMIRRORLEFTUP:
        if (positionID == LEFT || positionID == UP)
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
            if (isMovableAtBeginning(gridOrigin[i][j]))
            {
                gridMovables[i][j] = gridOrigin[i][j];
            }
        }
    }
}

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

bool isOutOfBorder(int **objectPosition, int numRows, int numColumns)
{
    if (objectPosition[0][0] < 0 || objectPosition[0][0] >= numRows || objectPosition[0][1] < 0 || objectPosition[0][0] >= numColumns)
    {
        return true;
    }
    return false;
}