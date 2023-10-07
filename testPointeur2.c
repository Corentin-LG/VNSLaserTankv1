#include <stdio.h>

void change(int *pointeur);

int main() {
    int np;
    int *ptr1;
    int *ptr2;
    int *ptr3;
    int *ptr4;

    np = 5;
    ptr1 = &np;
    ptr2 = &ptr1;
    printf("l1 np = %d, ptr1 = %p, ptr2 = %p\n", np, (void *)ptr1, (void *)ptr2);
    printf("l2 np = %p, ptr1 = %p, ptr2 = %p\n", (void *)&np, (void *)&ptr1, (void *)&ptr2);
    printf("l3 ptr1 = %d, *ptr2 = %p\n", *ptr1, (void *)*ptr2);

    change(ptr1);
    printf("l1 np = %d, ptr1 = %p, ptr2 = %p\n", np, (void *)ptr1, (void *)ptr2);
    printf("l2 np = %p, ptr1 = %p, ptr2 = %p\n", (void *)&np, (void *)&ptr1, (void *)&ptr2);
    printf("l3 ptr1 = %d, *ptr2 = %p\n", *ptr1, (void *)*ptr2);

    return 0;
}

void change(int *pointeur) {
    //printf("*pointeur = %d, pointeur = %p, &pointeur = %p,\n", *pointeur, (void *)pointeur, (void *)&pointeur);
    *pointeur = 10; // Modifier la valeur pointée par le pointeur
}
