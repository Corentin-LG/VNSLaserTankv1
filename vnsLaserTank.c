#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdbool.h>

bool loadFile(const char *filename, wchar_t ***linesArray, int *numLines) {
    setlocale(LC_ALL, "");
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        return false;
    }

    *numLines = 0;
    wchar_t line[256];

    while (fgetws(line, sizeof(line) / sizeof(line[0]), file) != NULL) {
        (*numLines)++;
    }

    // Réinitialiser la position du fichier au début
    fseek(file, 0, SEEK_SET);

    // Allouer de la mémoire pour le tableau de lignes
    *linesArray = (wchar_t **)malloc((*numLines) * sizeof(wchar_t *));
    if (*linesArray == NULL) {
        fprintf(stderr, "Échec de l'allocation de mémoire\n");
        fclose(file);
        return false;
    }

    // Lire chaque ligne du fichier et la stocker dans le tableau
    int i = 0;
    while (fgetws(line, sizeof(line) / sizeof(line[0]), file) != NULL) {
        // Allouer de la mémoire pour la ligne actuelle
        (*linesArray)[i] = (wchar_t *)malloc((wcslen(line) + 1) * sizeof(wchar_t));
        if ((*linesArray)[i] == NULL) {
            fprintf(stderr, "Échec de l'allocation de mémoire\n");
            fclose(file);
            return false;
        }

        // Copier la ligne dans le tableau
        wcscpy((*linesArray)[i], line);
        i++;
    }

    fclose(file);

    return true;
}

int main() {
    const char *filename = "Beginner-I.lt4";
    wchar_t **linesArray;
    int numLines;

    if (loadFile(filename, &linesArray, &numLines)) {
        for (int i = 0; i < numLines; i++) {
            wprintf(L"Ligne %d : %ls", i + 1, linesArray[i]);
        }
        for (int i = 0; i < numLines; i++) {
            free(linesArray[i]);
        }
        free(linesArray);
    }

    return 0;
}
