#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <wchar.h>
#include <string.h>

void printArray(int **array, int rows, int cols);
bool firstTankPosition(int id);

struct ConversionTable
{
    const wchar_t *lettre;
    int valeur;
};

struct ConversionTable tableX[] = {
    {L"Tu", 1}, {L"Tr", 2}, {L"Td", 3}, {L"Tl", 4}, {L"D", 5}, {L"b", 6}, {L"w", 7}, {L"Bs", 8}, {L"Bm", 9}, {L"B", 10}, {L"Au", 11}, {L"Ar", 12}, {L"Ad", 13}, {L"Al", 14}, {L"Mur", 15}, {L"Mdr", 16}, {L"Mdl", 17}, {L"Mul", 18}, {L"Wu", 19}, {L"Wr", 20}, {L"Wd", 21}, {L"Wl", 22}, {L"C", 23}, {L"Rur", 24}, {L"Rrd", 25}, {L"Rdl", 26}, {L"Rlu", 27}, {L"I", 28}, {L"i", 29}, {L"Tr", 30}, {L"Tg", 31}, {L"Tb", 32}, {L"Tc", 33}, {L"Ty", 34}, {L"Tp", 35}, {L"Tw", 36}, {L"Td", 37}};

struct ConversionTable tableN[] = {
    {L"Tu\n", 1}, {L"Tr\n", 2}, {L"Td\n", 3}, {L"Tl\n", 4}, {L"D\n", 5}, {L"b\n", 6}, {L"w\n", 7}, {L"Bs\n", 8}, {L"Bm\n", 9}, {L"B\n", 10}, {L"Au\n", 11}, {L"Ar\n", 12}, {L"Ad\n", 13}, {L"Al\n", 14}, {L"Mur\n", 15}, {L"Mdr\n", 16}, {L"Mdl\n", 17}, {L"Mul\n", 18}, {L"Wu\n", 19}, {L"Wr\n", 20}, {L"Wd\n", 21}, {L"Wl\n", 22}, {L"C\n", 23}, {L"Rur\n", 24}, {L"Rrd\n", 25}, {L"Rdl\n", 26}, {L"Rlu\n", 27}, {L"I\n", 28}, {L"i\n", 29}, {L"Tr\n", 30}, {L"Tg\n", 31}, {L"Tb\n", 32}, {L"Tc\n", 33}, {L"Ty\n", 34}, {L"Tp\n", 35}, {L"Tw\n", 36}, {L"Td\n", 37}};

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
// enum gameMoving
// {
//     FIRE,
//     UP,
//     RIGHT,
//     DOWN,
//     LEFT
// };

int main()
{
    srand(time(NULL));
    const char *filename = "Beginner-I.lt4";
    // const char *filename = "testing.lt4";
    int numRows = 0;
    int numColumns = 0;
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
    // char deplacementsHypothese = (char *)malloc((1000) * sizeof(char));
    // char deplacementsRetenu = (char *)malloc((1000) * sizeof(char));
    // Chargement du fichier
    setlocale(LC_ALL, "");
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        return 1;
    }

    // Recherche du nombre de lignes
    wchar_t header[100000];
    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Rows: %d", &numRows) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de lignes\n");
        fclose(file);
        return 1;
    }

    // Recherche du nombre de colonnes
    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Cols: %d", &numColumns) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de colonnes\n");
        fclose(file);
        return 1;
    }

    // Créez un tableau 2D pour stocker les valeurs converties
    int **tableau = (int **)malloc((numRows + 1) * sizeof(int *));
    for (int i = 0; i < numRows; i++)
    {
        tableau[i] = (int *)malloc((numColumns + 1) * sizeof(int));
    }

    for (int i = 0; i < 5; i++)
    {
        fgetws(header, sizeof(header) / sizeof(header[0]), file);
    }

    wchar_t *token;
    wchar_t *splitBuffer;
    int k = 0;
    int l = 0;
    wchar_t *tokenInterm;
    wchar_t *tabxInterm;
    wchar_t *tabnInterm;

    // Lire les données du fichier et les stocker dans le tableau
    for (int i = 0; i < numRows; i++)
    {
        fgetws(header, sizeof(header) / sizeof(header[0]), file);

        token = wcstok(header, L" ");

        // wprintf(L"En-tête complet : %ls\n", header);
        for (int j = 0; j < numColumns; j++)
        {

            // wprintf(L"Token complet : %ls et plus si affinité\n", token);
            k = 0;

            // Recherche de la correspondance dans la table de conversion
            while (token != NULL)
            {
                // wprintf(L"j = %d, k = %d, token = %ls, tokenLenght = %d, lettre = %ls\n", j, k, token, strlen(token), tableX[k].lettre);
                tokenInterm = token;
                tabxInterm = tableX[k].lettre;
                tabnInterm = tableN[k].lettre;

                // wprintf(L"tokenInterm = %ls, tabxInterm = %ls, tokenIntermLenght = %d, tabxIntermLenght = %d,\n", tokenInterm, tabxInterm, strlen(&tokenInterm), strlen(&tabxInterm));
                if (wcscmp(tokenInterm, tabxInterm) == 0 || wcscmp(tokenInterm, tabnInterm) == 0)
                {
                    // printf("yes\n");
                    tableau[i][j] = tableX[k].valeur;
                    if (tableX[k].valeur == TANKUP || tableX[k].valeur == TANKRIGHT || tableX[k].valeur == TANKDOWN ||
                        tableX[k].valeur == TANKLEFT)
                    {
                        tankPosition[0][0] = i;
                        tankPosition[1][0] = i;
                        tankPosition[0][1] = j;
                        tankPosition[1][1] = j;
                        break;
                    }
                    // if (tableX[k].valeur == BASE)
                    // {
                    //     l = 0;
                    //     while (basesPosition[l][0])
                    //     {
                    //         l = l + 1;
                    //     }
                    //     basesPosition[l][0] = i;
                    //     basesPosition[l][1] = j;
                    //     break;
                    // }
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

    if (tableau != NULL)
    {
        printf("lignes : %d et colonnes : %d\n", numRows, numColumns);
        printArray(tableau, numRows, numColumns);

        // Libérer la mémoire du tableau
        for (int i = 0; i < numRows; i++)
        {
            free(tableau[i]);
        }
        free(tableau);
    }

    if (tankPosition != NULL)
    {
        printArray(tankPosition, 2, 2);

        for (int i = 0; i < 2; i++)
        {
            free(tankPosition[i]);
        }
        free(tankPosition);
    }

    if (basesPosition != NULL)
    {
        printArray(basesPosition, 5, 5);

        // Libérer la mémoire du tableau
        for (int i = 0; i < 5; i++)
        {
            free(basesPosition[i]);
        }
        free(basesPosition);
    }

    // free(deplacementsHypothese);
    // free(deplacementsRetenu);

    return 0;
}

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

void firstHeuristique(int **array)
{
}

bool firstTankPosition(int id)
{
    switch (id)
    {
    case TANKUP:
    case TANKRIGHT:
    case TANKDOWN:
    case TANKLEFT:
        return true;

    default:
        return false;
    }
}