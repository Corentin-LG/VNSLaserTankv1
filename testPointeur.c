#include <stdio.h>
#include <stdlib.h>
void changement1(int var1, int var2);
void changement2(int *var1, int var2);
void changement3(int **var1, int var2);
int main(){
    int *pointu;
    int paspointu;

    changement1(pointu, paspointu);
    printf("pointu = %d et paspointu = %d \n", pointu, paspointu);
    printf("&pointu = %d et paspointu = %d \n", &pointu, &paspointu);
    changement2(&pointu, paspointu);
    printf("v2 pointu = %d et paspointu = %d \n", pointu, paspointu);
    printf("&v2 pointu = %d et paspointu = %d \n", &pointu, &paspointu);
    changement3(&pointu, paspointu);
    printf("v3 pointu = %d et paspointu = %d \n", pointu, paspointu);
    printf("&v3 pointu = %d et paspointu = %d \n", &pointu, &paspointu);
    free(pointu);
    return 0;
}

void changement1(int var1, int var2){
    var1 = 5;
    var2 = 10;
    printf("cvar1 = %d et var2 = %d \n", var1, var2);
    printf("&cvar1 = %d et var2 = %d \n", &var1, &var2);
}

void changement2(int *var1, int var2){
    *var1 = 15;
    var2 = 20;
    printf("*cv2 var1 = %d et var2 = %d \n", *var1, var2);
    printf("&cv2 var1 = %d et var2 = %d \n", &var1, &var2);
}

void changement3(int **var1, int var2){
    *var1 = 35;
    var2 = 40;
    printf("*cv3 var1 = %d et var2 = %d \n", *var1, var2);
    printf("&cv3 var1 = %d et var2 = %d \n", &var1, &var2);
}