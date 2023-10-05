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
    {L"Tu", 1}, {L"Tr", 2}, {L"Td", 3}, {L"Tl", 4}, {L"D", 5}, {L"b", 6}, {L"w", 7}, {L"Bs", 8}, {L"Bm", 9}, {L"B", 10}, {L"Au", 11}, {L"Ar", 12}, {L"Ad", 13}, {L"Al", 14}, {L"Mur", 15}, {L"Mdr", 16}, {L"Mdl", 17}, {L"Mlu", 18}, {L"Wu", 19}, {L"Wr", 20}, {L"Wd", 21}, {L"Wl", 22}, {L"C", 23}, {L"Rur", 24}, {L"Rrd", 25}, {L"Rdl", 26}, {L"Rlu", 27}, {L"I", 28}, {L"i", 29}, {L"Tr", 30}, {L"Tg", 31}, {L"Tb", 32}, {L"Tc", 33}, {L"Ty", 34}, {L"Tp", 35}, {L"Tw", 36}, {L"Td", 37}};

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

    for (int i = 0; i < 5; i++)
    {
        fgetws(header, sizeof(header) / sizeof(header[0]), file);
    }

    //wchar_t token[256];
    wchar_t *token;
    //header[0] = L'\0'; // Vide la chaîne header

    // Lire les données du fichier et les stocker dans le tableau
    for (int i = 0; i < *numRows; i++)
    {
        fgetws(header, sizeof(header) / sizeof(header[0]), file);
        for (int j = 0; j < *numColumns; j++)
        {
            //fgetws(header, sizeof(header) / sizeof(header[0]), file);
            
            wprintf(L"En-tête complet : %ls\n", header);
            token = wcstok(header, L" ");
            //wprintf(L"Token complet : %ls\n", token);   

            // Recherche de la correspondance dans la table de conversion
            for (int k = 0; k < sizeof(table) / sizeof(table[0]); k++)
            {
                //printf("tok = %ls\n", token);
                //printf("letter = %ls\n", table[k].lettre);
                
                if (wcscmp(token, table[k].lettre) == 0)
                {
                    loadedFile[i][j] = table[k].valeur;
                    //printf("YESYES %d\n", loadedFile[i][j]);
                    //printf("val = %d\n", table[k].valeur);
                    //wprintf(L"mot : %ls\n", table[k].lettre);
                    break;
                }
            }
        }
    }

    fclose(file);
    return loadedFile;
}


// wcsncpy
// wcsstr 

// wcsncpy(motExtrait, chaineSource + debut, longueur);
// const wchar_t *position = wcsstr(chaine, sousChaine);

//check chaine lentgh ?
//trouver position1
//change in int and copy
// erase position1 + 1