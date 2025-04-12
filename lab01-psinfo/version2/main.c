//Inclusión de las librerías estándar
#include <stdio.h>
#include <stdlib.h>
//Inclusión de librería para métodos de comparación de strings strncmp strcmp etc.
#include <string.h>
#include "functions.h"

// Definición de la función principal
int main(int argc, char *argv[]){
    if(argc == 2){
        if(strcmp(argv[1], "-l")==0){
            printf("Después de -l debes escribir al menos un pid");
        }else{
            printProcessInfo(argv[1]);
        }
    } else if (argc > 2){
        if(strcmp(argv[1], "-l") == 0){
            printf("\n --------------------------------------------------- \n");
            for(int i = 2; i < argc; i++){
                printProcessInfo(argv[i]);
                printf("\n --------------------------------------------------- \n");
            }
        }else{
            printf("Si quieres poner varios pids, debes poner la flag -l\n");
        }
    }
}


