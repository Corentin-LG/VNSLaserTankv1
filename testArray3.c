#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdbool.h>

void LLloadFile(const char *filename, int *numRows, int *numColumns);

int main()
{
    // Inputs obligatoires
    const char *filename = "Beginner-I.lt4";
    int numRows, numColumns;
    // remplissage du tableau
    LLloadFile(filename, &numRows, &numColumns);
    return 0;
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
        return; // Pas besoin de retourner NULL ici
    }

    // Recherche du nombre de lignes
    wchar_t header[256];
    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Rows: %d", numRows) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de lignes\n");
        fclose(file);
        return; // Pas besoin de retourner NULL ici
    }

    // Recherche du nombre de colonnes
    fgetws(header, sizeof(header) / sizeof(header[0]), file);
    if (swscanf(header, L"Cols: %d", numColumns) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture du nombre de colonnes\n");
        fclose(file);
        return; // Pas besoin de retourner NULL ici
    }

    printf("lignes : %d et colonnes : %d\n", *numRows, *numColumns);
}
