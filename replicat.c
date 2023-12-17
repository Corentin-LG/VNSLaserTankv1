#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    // Nom de fichier initial
    const char *nomFichierInitial = "nomDuFichier.txt";

    // Nouveau nom de fichier
    const char *nouveauNom = "Soluce";

    // Trouver l'index du dernier point dans le nom du fichier
    const char *dernierPoint = strrchr(nomFichierInitial, '.');
    if (dernierPoint == NULL) {
        // Gérer le cas où il n'y a pas d'extension
        fprintf(stderr, "Erreur : Pas d'extension de fichier trouvé.\n");
        return 1;
    }

    // Calculer la longueur totale du nouveau nom de fichier
    size_t longueurNouveauNomFichier = strlen(nouveauNom) + strlen(nomFichierInitial) + 1;

    // Allouer de la mémoire pour le nouveau nom de fichier
    char *nouveauNomFichier = malloc(longueurNouveauNomFichier);

    // Construire le nouveau nom de fichier avec "Soluce" suivi de l'ancien nom
    strcpy(nouveauNomFichier, nouveauNom);
    strcat(nouveauNomFichier, nomFichierInitial);

    // Afficher le nouveau nom de fichier
    printf("Nouveau nom de fichier : %s\n", nouveauNomFichier);

    // Libérer la mémoire allouée
    free(nouveauNomFichier);

    return 0;
}
