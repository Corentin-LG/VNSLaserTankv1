#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdbool.h>

int **loadFile(const char *filename, int *numRows, int *numColumns);
// void afficherTableau(int **tableau, int numRows, int numColumns);

int main()
{
    // Inputs obligatiores
    const char *filename = "Beginner-I.lt4";
    int **tableau;
    int numRows, numColumns;
    // remplissage du tableau
    tableau = loadFile(filename, &numRows, &numColumns);

    // if (tableau != NULL)
    // {
    //     afficherTableau(tableau, numRows, numColumns);

    //     // Libérez la mémoire lorsque vous avez fini
    //     for (int i = 0; i < numRows; i++)
    //     {
    //         free(tableau[i]);
    //     }
    //     free(tableau);
    // }
    
    return 0;
}

// Table de conversion
// struct ConversionTable
// {
//     wchar_t lettre;
//     int valeur;
// };

// struct ConversionTable table[] = {
//     {L'Tu', 1}, {L'Tr', 2}, {L'Td', 3}, {L'Tl', 4}, {L'D', 5}, {L'b', 6}, {L'w', 7}, {L'Bs', 8}, {L'Bm', 9}, {L'B', 10}, {L'Au', 11}, {L'Ar', 12}, {L'Ad', 13}, {L'Al', 14}, {L'Mur', 15}, {L'Mrd', 16}, {L'Mdl', 17}, {L'Mlu', 18}, {L'Wu', 19}, {L'Wr', 20}, {L'Wd', 21}, {L'Wl', 22}, {L'C', 23}, {L'Rur', 24}, {L'Rrd', 25}, {L'Rdl', 26}, {L'Rlu', 27}, {L'I', 28}, {L'i', 29}, {L'Tr', 30}, {L'Tg', 31}, {L'Tb', 32}, {L'Tc', 33}, {L'Ty', 34}, {L'Tp', 35}, {L'Tw', 36}, {L'Td', 37}
// };

int **loadFile(const char *filename, int *numRows, int *numColumns)
{
    // Chargement du fichier
    setlocale(LC_ALL, "");
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    // Recherche du nombre de lignes
    wchar_t header[256];
    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Rows: %d", *numRows) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de lignes\n");
        fclose(file);
        return NULL;
    }

    // Recherche du nombre de colonnes
    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Cols: %d", *numColumns) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de colonnes\n");
        fclose(file);
        return NULL;
    }
    
    printf("lignes : %d et colonnes : %d", *numRows, *numColumns);

    int **outputArray = (int **)malloc((*numRows) * sizeof(int *));
    for (int i = 0; i < (*numRows); i++)
    {
        outputArray[i] = (int *)malloc((*numColumns) * sizeof(int));
    }

    int *numLines = 0;
    wchar_t line[7 + *numRows];

    while (fgetws(line, sizeof(line) / sizeof(line[0]), file) != NULL) {
        (*numLines)++;
    }

    fseek(file, 0, SEEK_SET);
// //wip
// //convertir : outparr[i][j] = conv(inarr[i,j], )
//     // Parcourez les lignes du fichier, convertissez-les et stockez-les dans le tableau 2D
//     for (int i = 7; i < numLines; i++)
//     {
//         convertir(linesArray[i], outputArray[i], *numColumns);
//     }

//     // Libérez la mémoire pour linesArray
//     for (int i = 0; i < numLines; i++)
//     {
//         free(linesArray[i]);
//     }
//     free(linesArray);

    return outputArray;
}


// void convertir(const wchar_t *entree, int *sortie, int numColumns)
// {
//     int i = 0;
//     while (entree[i] != '\0')
//     {
//         wchar_t lettre[3];
//         lettre[0] = entree[i];
//         lettre[1] = entree[i + 1];
//         lettre[2] = '\0'; // Assurez-vous que la lettre est terminée par un caractère nul

//         int valeur = 0;
//         for (int j = 0; j < sizeof(table) / sizeof(table[0]); j++)
//         {
//             if (wcscmp(table[j].lettre, lettre) == 0)
//             {
//                 valeur = table[j].valeur;
//                 break;
//             }
//         }

//         sortie[i / numColumns][i % numColumns] = valeur;
//         i += 2; // Passe à la lettre suivante (2 caractères par lettre)
//     }
// }

// void afficherTableau(int **tableau, int numRows, int numColumns)
// {
//     for (int i = 0; i < numRows; i++)
//     {
//         for (int j = 0; j < numColumns; j++)
//         {
//             printf("%d ", tableau[i][j]);
//         }
//         printf("\n"); // Nouvelle ligne pour chaque ligne du tableau
//     }
// }