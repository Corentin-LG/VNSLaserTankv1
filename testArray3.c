#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdbool.h>

void LLloadFile(const char *filename, int *numRows, int *numColumns);
void loadFile(const char *filename, int *numRows, int *numColumns, char ***table);

int main()
{
    // Inputs obligatoires
    const char *filename = "Beginner-I.lt4";
    int numRows, numColumns;
    char **table;
    loadFile(filename, &numRows, &numColumns, &table);

    // Vous pouvez maintenant utiliser le tableau 'table' qui contient les données.

    // Par exemple, pour accéder à une cellule (ligne 0, colonne 0), vous pouvez faire :
    printf("Contenu de la cellule (0, 0) : %s\n", table[0][0]);

    // Assurez-vous de libérer la mémoire lorsque vous avez fini avec le tableau.
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            
            free(table[i][j]);
        }
        free(table[i]);
    }
    free(table);

    return 0;
}

void loadFile(const char *filename, int *numRows, int *numColumns, char ***table)
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
    // ici ok
    // wip

    *table = (char **)malloc(*numRows * sizeof(char *));
    for (int i = 0; i < *numRows; i++)
    {
        (*table)[i] = (char *)malloc(*numColumns * sizeof(char)); // Notez le changement ici
    }

    // Lire les lignes et les stocker dans le tableau
    for (int i = 0; i < *numRows; i++)
    {
        fgetws(header, sizeof(header) / sizeof(header[0]), file);
        wchar_t *token = wcstok(header, L" ");
        int j = 0;
        while (token != NULL)
        {
            (*table)[i][j] = (char *)malloc((wcslen(token) + 1) * sizeof(char));
            wcstombs((*table)[i][j], token, wcslen(token) + 1);
            token = wcstok(NULL, L" ");
            j++;
        }
    }
    
    fclose(file);
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
