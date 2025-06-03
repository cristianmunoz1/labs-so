#include <stdio.h>
#include <stdlib.h>

int main(){
    int* data = malloc(100*sizeof(int));
    int* funny = &data[51];
    //Incluimos datos en el arreglo
    for(int i = 0; i<100; i++){
        data[i] = i;
    }
    //Liberamos el arreglo
    free(funny);

    printf("El valor data[50] es %d\n", data[50]);
}

