//Inclusión de las librerías estándar
#include <stdio.h>
#include <stdlib.h>
//Inclusión de librería para métodos de comparación de strings strncmp strcmp etc.
#include <string.h>


// Definición de la función principal
int main(int argc, char *argv[]){
    //Creamos en el stack un arreglo de caracteres para guardar la ruta del archivo
    char path[30];
    //Concatenamos toda la ruta del archivo status con su respectivo process id (pid)
    snprintf(path, sizeof(path), "/proc/%s/status", argv[1]);
    //Creamos el puntero a archivo donde se abrirá el archivo status del pid correspondiente
    FILE *status;
    //Creamos la variable de tipo size_t para pasarle a la función getline
    size_t linesize = 128;
    //Creamos una variable de tipo size_t para detener el loop while cuando se acabe el archivo, cuando retorne -1 significa que ya acabó
    size_t nread = 0;
    //Abrimos el archivo en modo lectura "r" con la función fopen
    status = fopen(path, "r");
    //Creamos un espacio en el heap del tamaño de 120 chars para almacenar línea por línea del archivo
    char* buffer = malloc(linesize * sizeof(char));
    
    //Verificamos si hay error al abrir el archivo
    if(status == NULL){
        printf("Error al abrir el archivo %s\n", path);
        exit(-1);
    }
     
    while((nread = getline(&buffer, &linesize, status)) != -1){
        if (strncmp("Name:", buffer, 5) == 0) {
            printf("%-35s %s", "Nombre del proceso:", buffer + 5);
        } else if (strncmp("State:", buffer, 6) == 0) {
            printf("%-35s %s", "Estado del proceso:", buffer + 6);
        } else if (strncmp("VmSize:", buffer, 7) == 0) {
            printf("%-35s %s", "Tamaño total de la imágen de memoria:", buffer + 8);
        } else if (strncmp("VmData:", buffer, 7) == 0) {
            printf("%-35s %s", "Tamaño de la memoria DATA:", buffer + 7);
        } else if (strncmp("VmStk:", buffer, 6) == 0) {
            printf("%-35s %s", "Tamaño de la memoria STACK:", buffer + 6);
        } else if (strncmp("VmExe:", buffer, 6) == 0) {
            printf("%-35s %s", "Tamaño de la memoria TEXT:", buffer + 6);
        } else if (strncmp("voluntary_ctxt_switches:", buffer, 24) == 0) {
            printf("%-35s %s", "Context Switches voluntarios:", buffer + 24);
        } else if (strncmp("nonvoluntary_ctxt_switches:", buffer, 27) == 0) {
            printf("%-35s %s", "Context Switches involuntarios:", buffer + 27);
        }
    }

}
