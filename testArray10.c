#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>

void changeRC(int ligne, int colonne);
void changeRC2(int *ligne, int *colonne);
void changeRC3(int **ligne, int **colonne);

int main(){
    int **tabInt;
    int *rows;
    int *cols;

    rows = (int *)malloc(sizeof(int));
    cols = (int *)malloc(sizeof(int));

    changeRC(rows, cols);
    
    printf("rows = %d et cols = %d\n", *rows, *cols);

    changeRC2(&rows, &cols);

    printf("rows2 = %d et cols2 = %d\n", *rows, *cols);


    changeRC3(&rows, &cols);

    printf("rows3 = %d et cols3 = %d\n", *rows, *cols);


    // N'oubliez pas de libérer la mémoire allouée
    free(rows);
    free(cols);

    return 0;
}

void changeRC(int ligne, int colonne){
    ligne = 5;
    colonne = 10;
    printf("ligne = %d et colonne = %d\n", ligne, colonne);
}

void changeRC2(int *ligne, int *colonne){
    *ligne = 5;
    *colonne = 10;
    printf("ligne2 = %d et colonne2 = %d\n", ligne, colonne);
}

void changeRC3(int **ligne, int **colonne) {
    **ligne = 5;
    **colonne = 10;
    printf("ligne3 = %d et colonne3 = %d\n", **ligne, **colonne);
}