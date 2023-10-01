#include <stdio.h>
#include <stdlib.h>

int main() {
    int **deuxDArray;

    deuxDArray = (int **)malloc(sizeof(int *) * 2);
    for (int i = 0; i < 2; i++) {
        deuxDArray[i] = (int *)malloc(sizeof(int) * 3);
    }
    deuxDArray[1][2] = 5;

    printf("%d", deuxDArray[1][2]);

    for (int i = 0; i < 2; i++) {
        free(deuxDArray[i]);
    }
    free(deuxDArray);


    const char *filename = "Beginner-I.lt4";
    printf("%s", filename);

    return 0;
}
