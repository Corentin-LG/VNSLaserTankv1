#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>

int main() {
    setlocale(LC_ALL, ""); // Définir la locale par défaut (UTF-8)

    FILE *file;
    const char *filename = "Beginner-I.lt4"; // Nom du fichier .lt4

    // Ouvrir le fichier .lt4 en lecture
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        return 1;
    }

    // Compter le nombre de lignes dans le fichier
    int numLines = 0;
    wchar_t line[256]; // Vous pouvez ajuster la taille en fonction de vos besoins

    while (fgetws(line, sizeof(line) / sizeof(line[0]), file) != NULL) {
        numLines++;
    }

    // Réinitialiser la position du fichier au début
    fseek(file, 0, SEEK_SET);

    // Allouer de la mémoire pour le tableau de lignes
    wchar_t **linesArray = (wchar_t **)malloc(numLines * sizeof(wchar_t *));
    if (linesArray == NULL) {
        fprintf(stderr, "Échec de l'allocation de mémoire\n");
        return 1;
    }

    // Lire chaque ligne du fichier et la stocker dans le tableau
    int i = 0;
    while (fgetws(line, sizeof(line) / sizeof(line[0]), file) != NULL) {
        // Allouer de la mémoire pour la ligne actuelle
        linesArray[i] = (wchar_t *)malloc((wcslen(line) + 1) * sizeof(wchar_t));
        if (linesArray[i] == NULL) {
            fprintf(stderr, "Échec de l'allocation de mémoire\n");
            return 1;
        }

        // Copier la ligne dans le tableau
        wcscpy(linesArray[i], line);
        i++;
    }

    // Fermer le fichier
    fclose(file);

    // Maintenant, vous avez un tableau de lignes dans linesArray
    // Vous pouvez accéder à chaque ligne comme linesArray[i]

    // Par exemple, pour afficher toutes les lignes
    for (i = 0; i < numLines; i++) {
        wprintf(L"Ligne %d : %ls", i + 1, linesArray[i]);
    }

    // N'oubliez pas de libérer la mémoire lorsque vous avez fini
    for (i = 0; i < numLines; i++) {
        free(linesArray[i]);
    }
    free(linesArray);

    return 0;
}
