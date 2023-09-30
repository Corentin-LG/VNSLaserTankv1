#include <stdio.h>
#include <locale.h>
#include <wchar.h>

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

    // Lire la première ligne du fichier .lt4
    wchar_t line[256]; // Vous pouvez ajuster la taille en fonction de vos besoins
    if (fgetws(line, sizeof(line) / sizeof(line[0]), file) != NULL) {
        wprintf(L"La première ligne du fichier %s est : %ls", filename, line);
    } else {
        fprintf(stderr, "Le fichier %s est vide\n", filename);
    }

    // Fermer le fichier
    fclose(file);

    return 0;
}
