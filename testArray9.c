#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>

void printArray(int **array, int rows, int cols);
int **LoadFile(const char *filename, int *numRows, int *numColumns);

int main()
{
    const char *filename = "Beginner-I.lt4";
    int **tableau;
    int *numRows;
    int *numColumns;
    numRows = (int *)malloc(sizeof(int));
    numColumns = (int *)malloc(sizeof(int));

    tableau = LoadFile(filename, numRows, numColumns);

    printf("lignes : %d et colonnes : %d\n", numRows, numColumns);

    printArray(tableau, numRows, numColumns);
    free(numRows);
    free(numColumns);
    if (tableau != NULL)
    {
        // Libérer la mémoire du tableau
        for (int i = 0; i < *numRows; i++)
        {
            free(tableau[i]);
        }
        free(tableau);
    }

    return 0;
}

// Table de conversion
struct ConversionTable
{
    const wchar_t *lettre;
    int valeur;
};

// so long maybe
// struct ConversionTable table[] = {
//     {L"Tu", 1}, {L"Tr", 2}, {L"Td", 3}, {L"Tl", 4}, {L"D", 5}, {L"b", 6}, {L"w", 7}, {L"Bs", 8}, {L"Bm", 9}, {L"B", 10}, {L"Au", 11}, {L"Ar", 12}, {L"Ad", 13}, {L"Al", 14}, {L"Mur", 15}, {L"Mrd", 16}, {L"Mdl", 17}, {L"Mlu", 18}, {L"Wu", 19}, {L"Wr", 20}, {L"Wd", 21}, {L"Wl", 22}, {L"C", 23}, {L"Rur", 24}, {L"Rrd", 25}, {L"Rdl", 26}, {L"Rlu", 27}, {L"I", 28}, {L"i", 29}, {L"Tr", 30}, {L"Tg", 31}, {L"Tb", 32}, {L"Tc", 33}, {L"Ty", 34}, {L"Tp", 35}, {L"Tw", 36}, {L"Td", 37}};

struct ConversionTable table[] = {
    {L"Tu", 1}, {L"Tr", 2}, {L"Td", 3}, {L"Tl", 4},{L"D", 5}, {L"b", 6}, {L"w", 7}, {L"Bs", 8}, {L"Bm", 9}, {L"B", 10}};


void printArray(int **array, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }
}

int **LoadFile(const char *filename, int *numRows, int *numColumns)
{
    // Chargement du fichier
    setlocale(LC_ALL, "");
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        return; // Pas besoin de retourner NULL ici
    }

    // Recherche du nombre de lignes
    wchar_t header[256];
    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Rows: %d", *numRows) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de lignes\n");
        fclose(file);
        return; // Pas besoin de retourner NULL ici
    }

    // Recherche du nombre de colonnes
    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Cols: %d", *numColumns) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de colonnes\n");
        fclose(file);
        return; // Pas besoin de retourner NULL ici
    }

    printf("lignes : %d et colonnes : %d\n", *numRows, *numColumns);

    // Créez un tableau 2D pour stocker les valeurs converties
    int **LoadFile = (int **)malloc((*numRows) * sizeof(int *));
    for (int i = 0; i < (*numRows); i++)
    {
        LoadFile[i] = (int *)malloc((*numColumns) * sizeof(int));
    }

    // reset head reader
    fseek(file, 0, SEEK_SET);

    wchar_t header2[250];

    for (int i = 0; i < 7; i++)
    {
        fgetws(header2, sizeof(header2) / sizeof(header2[0]), file);
    }

    header2[0] = L'\0'; // Vide la chaîne header

    int outputIndexRows = 0;
    int outputIndexColumns = 0;

    // Découper la chaîne en mots
    wchar_t *token;

    // Lire les lignes 8 à 23 et les concaténer dans header
    for (int i = 7; i < *numRows; i++)
    {
        fgetws(header2, sizeof(header2) / sizeof(header2[0]), file);

        // Découper la chaîne en mots
        token = wcstok(header2, L" ");

        while (token != NULL)
        {
            // Parcourir la table de conversion pour trouver la correspondance
            for (int j = 0; j < sizeof(table) / sizeof(table[0]); j++)
            {
                if (wcscmp(token, table[j].lettre) == 0)
                {
                    LoadFile[outputIndexRows][outputIndexColumns++] = table[j].valeur;
                    break;
                }
            }

            // Obtenir le mot suivant
            token = wcstok(NULL, L" ");

            // Zero Column
            outputIndexColumns = 0;

            // Next Line
            outputIndexRows++;
        }
    }

    return LoadFile;
}
