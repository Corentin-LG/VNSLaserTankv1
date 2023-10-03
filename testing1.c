#include <stdio.h>
#include <locale.h> // Ajoutez l'inclusion de locale.h
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <wchar.h> // Ajoutez l'inclusion de wchar.h

void printArray(int **array, int rows, int cols);
int **LoadFile(const char *filename, int *numRows, int *numColumns);

int main()
{
    srand(time(NULL));
    const char *filename = "testing.lt4";
    int **tableau;
    int numRows;
    int numColumns;

    tableau = LoadFile(filename, &numRows, &numColumns);

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

    return 0;
}

// Table de conversion
struct ConversionTable
{
    const wchar_t *lettre;
    int valeur;
};


struct ConversionTable table[] = {
    {L"Tu", 1}, {L"Tr", 2}, {L"Td", 3}, {L"Tl", 4},{L"D", 5}, {L"b", 6}, {L"w", 7}, {L"Mdr", 8}, {L"Wu", 9}, {L"Wr", 10}};


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

int **LoadFile(const char *filename, int *numRows, int *numColumns)
{
    // Chargement du fichier
    setlocale(LC_ALL, "");
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        return NULL; // Retourne NULL en cas d'erreur
    }

    // Recherche du nombre de lignes
    wchar_t header[256];
    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Rows: %d", numRows) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de lignes\n");
        fclose(file);
        return NULL; // Retourne NULL en cas d'erreur
    }

    // Recherche du nombre de colonnes
    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Cols: %d", numColumns) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de colonnes\n");
        fclose(file);
        return NULL; // Retourne NULL en cas d'erreur
    }

    // Créez un tableau 2D pour stocker les valeurs converties
    int **loadedFile = (int **)malloc((*numRows) * sizeof(int *));
    for (int i = 0; i < (*numRows); i++)
    {
        loadedFile[i] = (int *)malloc((*numColumns) * sizeof(int));
    }

    for (int i = 0; i < *numRows; i++)
    {
        for (int j = 0; j < *numColumns; j++)
        {
            loadedFile[i][j] = rand() % 100; // Remplit avec des nombres aléatoires de 0 à 99
        }
    }
    fclose(file);
    return loadedFile;
}
