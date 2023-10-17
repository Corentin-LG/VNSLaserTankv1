#include <stdio.h>
#include <stdlib.h>

int main() {
    int numRows = 15;
    int numCols = 26;

    //int myArray[numRows][numCols];

    int **myArray = (int **)malloc(numRows * sizeof(int *));
    for (int i = 0; i < numRows; i++) {
        myArray[i] = (int *)malloc(numCols * sizeof(int));
    }

    int rowLenght = sizeof(myArray) / sizeof(myArray[0]);
    int colLenght = sizeof(myArray[0]) / sizeof(myArray[0][0]);


    printf("Nombre de lignes : %d\n", rowLenght);
    printf("Nombre de colonnes : %d\n", colLenght);





    for (int i = 0; i < numRows; i++) {
        free(myArray[i]);
    }
    free(myArray);





    // int ro, col;
    // int **arrayGrid = (int **)malloc((numRows + 1) * sizeof(int *));
    // for (int i = 0; i < numRows; i++) {
    //     arrayGrid[i] = (int *)malloc((numCols + 1) * sizeof(int));
    // }

    // Obtenez le nombre de lignes (dimension 1) et le nombre de colonnes (dimension 2).
    // ro = sizeof(arrayGrid) / sizeof(arrayGrid[0]);
    // col = sizeof(arrayGrid[0]) / sizeof(arrayGrid[0][0]);

    // for (int i = 0; i < myArray:: ; i++) {
    //     for (int j = 0; j < 4; j++) {
    //         printf("myArray[%d][%d] = %d\n", i, j, myArray[i][j]);
    //     }
    // }

    // Affichez le nombre de lignes et de colonnes.
    
   
    // printf("Nombre de lignes : %d\n", ro);
    // printf("Nombre de colonnes : %d\n", col);

    // Vous pouvez maintenant utiliser ro et col comme nécessaire.

    // Libérez la mémoire allouée pour le tableau.
    // for (int i = 0; i < numRows; i++) {
    //     free(arrayGrid[i]);
    // }
    // free(arrayGrid);

    return 0;
}
