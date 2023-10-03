#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void changeRC(int *ligne, int *colonne);
void changeRC2(int **ligne, int **colonne); // Notez que nous utilisons des pointeurs vers des pointeurs ici
int **changeRC3(int *ligne, int *colonne);
void fillRandomNumbers(int **array, int rows, int cols);
void printArray(int **array, int rows, int cols);

int main() {
    int *rows;
    int *cols;

    int **intTab;

    srand(time(NULL));

    // Allouer de la mémoire pour rows et cols
    rows = (int *)malloc(sizeof(int));
    cols = (int *)malloc(sizeof(int));

    changeRC(rows, cols);
    
    printf("rows = %d et cols = %d\n", *rows, *cols);

    // changeRC2(&rows, &cols);

    // printf("rows2 = %d et cols2 = %d\n", *rows, *cols);

    intTab = changeRC3(&rows, &cols);

    printf("00 2= %d et 11 2= %d\n", intTab[0][0],  intTab[1][1]);

    fillRandomNumbers(intTab, rows, cols);
    printArray(intTab, rows, cols);
    // N'oubliez pas de libérer la mémoire allouée
    free(rows);
    free(cols);


// Libérer la mémoire allouée pour intTab
    for (int i = 0; i < *rows; i++) {
        free(intTab[i]);
    }
    free(intTab);


    return 0;
}

void changeRC(int *ligne, int *colonne) {
    *ligne = 5;
    *colonne = 10;
    printf("ligne = %d et colonne = %d\n", *ligne, *colonne);
}

void changeRC2(int **ligne, int **colonne) {
    **ligne = 5;
    **colonne = 10;
    printf("ligne2 = %d et colonne2 = %d\n", **ligne, **colonne);
}

int **changeRC3(int *ligne, int *colonne) {
    *ligne = 3;
    *colonne = 8;
    printf("ligne3 = %d et colonne3 = %d\n", *ligne, *colonne);

    int **changeRC3 = (int **)malloc((*ligne) * sizeof(int *));
    for (int i = 0; i < (*ligne); i++) {
        changeRC3[i] = (int *)malloc((*colonne) * sizeof(int));
    }
    changeRC3[0][0] = 7;
    changeRC3[1][1] = 7;
    printf("00 = %d et 11 = %d\n", changeRC3[0][0], changeRC3[1][1]);

    return changeRC3;
}

void fillRandomNumbers(int **array, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            array[i][j] = rand() % 100; // Remplit avec des nombres aléatoires de 0 à 99
        }
    }
}

void printArray(int **array, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }
}