#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdbool.h>

int **loadFile(const char *filename, int *numRows, int *numColumns);
void LLloadFile(const char *filename, int *numRows, int *numColumns);

int main()
{
    // Inputs obligatiores
    const char *filename = "Beginner-I.lt4";
    int **tableau;
    int numRows, numColumns;
    // remplissage du tableau
    //tableau = loadFile(filename, &numRows, &numColumns);
    LLloadFile(filename, &numRows, &numColumns);
    return 0;
}

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
    
    printf("lignes : %d et colonnes : %d", numRows, numColumns);

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
    outputArray[0][0] = *numColumns;
    outputArray[0][1] = *numLines;
    printf("lignes : %d et colonnes : %d", outputArray[0][0], outputArray[0][1]);
    return outputArray;
}

void LLloadFile(const char *filename, int *numRows, int *numColumns)
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
    
    printf("lignes : %d et colonnes : %d", numRows, numColumns);
    return NULL;
}