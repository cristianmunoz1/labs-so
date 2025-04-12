//Librería que contiene el método para validar si un caracter es dígito o no
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void writeReport(int argc, char* argv[], struct Node* head, int firstProcessPosition);
int firstProcessPosition(int argc, char* argv[]);

//Definición de función para saber si un dato es numérico, con esta se validarán que los pids sean correctos
int isNumeric(const char *str){
    for(int i = 0; str[i]; i++){
        if(!isdigit(str[i])){
            return 0;
        }
    }
    return 1;
}

int storeProcessInfo(char pid[], struct Node** queue){
    char path[64];
    snprintf(path, sizeof(path), "/proc/%s/status", pid);
    // Declaramos un buffer para leer cada línea del archivo que retorne la función popen. 128 porque es lo máximo que ocupa una línea. 
    char buffer[128];
    //Buffer que almacenará toda la información necesaria de un proceso y la pasará a la data de un nuevo nodo
    char bufferInfo[2048] = "";

    //Abrimos un flujo de archivo para abrir el pipe en modo lectura en el cual guardaremos la información de la salida del comando. 
    FILE *fp = fopen(path, "r"); 

    //Si fp es nulo, osea que no se pudo ejecutar el comando o que el comando no retornó nada
    if (fp == NULL) {
        snprintf(bufferInfo, sizeof(bufferInfo), "El proceso con pid %s no existe o fue terminado\n", pid);
        insertAtEnd(queue, bufferInfo);
        return -1;
    }
    // Fgets ayuda a abrir un pipe del archivo fp y guardar línea por línea en el buffer que definimos anteriormente
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {        
        if (strncmp("Name:", buffer, 5) == 0) {
            strcat(bufferInfo, "Nombre del proceso: ");
            strcat(bufferInfo, buffer + 6);
        } else if (strncmp("State:", buffer, 6) == 0) {
            strcat(bufferInfo, "Estado del proceso: ");
            strcat(bufferInfo, buffer + 7);
        } else if (strncmp("VmSize:", buffer, 7) == 0) {
            strcat(bufferInfo, "Tamaño de la imagen de memoria: ");
            strcat(bufferInfo, buffer + 8);
        } else if (strncmp("VmExe:", buffer, 6) == 0) {
            strcat(bufferInfo, "Tamaño de la memoria TEXT: ");
            strcat(bufferInfo, buffer + 7);
        } else if (strncmp("VmData:", buffer, 7) == 0) {
            strcat(bufferInfo, "Tamaño de la memoria DATA: ");
            strcat(bufferInfo, buffer + 8);
        } else if (strncmp("VmStk:", buffer, 6) == 0) {
            strcat(bufferInfo, "Tamaño de la memoria STACK: ");
            strcat(bufferInfo, buffer + 7);
        } else if (strncmp("voluntary_ctxt_switches:", buffer, 24) == 0) {
            strcat(bufferInfo, "# de cambios de contexto voluntarios: ");
            strcat(bufferInfo, buffer + 25);
        } else if (strncmp("nonvoluntary_ctxt_switches:", buffer, 27) == 0) {
            strcat(bufferInfo, "# de cambios de contexto no voluntarios: ");
            strcat(bufferInfo, buffer + 28);
        } 
    }
    
    fclose(fp);
    insertAtEnd(queue, bufferInfo);
    return 0;
}

void printProcessQueue(struct Node* head){
    //creamos un nodo temporal que apunte a head
    struct Node* temp = head;
    printf("\nINFORMACIÓN DE LOS PROCESOS: \n");
    
    printf("------------------------------------------------------------------------\n");
    //mientras no sea null entonces vamos a imprimir la data del nodo temp
    while (temp != NULL) {
        printf("%s\n", temp->data);  
        //Ahora temp debe apuntar al siguiente
        temp = temp->next;

        printf("------------------------------------------------------------------------\n");
    }
}

// Función para validar que se pase el -l como parámetro cuando se ingresen varios pids
int processPIDs(int argc, char* argv[], struct Node** queue, int *hasR, int *hasL) {
    if (argc < 2) {
        printf("Uso: psinfo [ -l pid1 pid2 ... ] [ -r ]\n");
        return -1;
    }

    int i = 1;
    if (strcmp(argv[i], "-l") == 0) {
        *hasL = 1;
        i++;

        if (i >= argc || strcmp(argv[i], "-r") == 0) {
            printf("Error: Debe ingresar al menos un PID después de -l.\n");
            return -1;
        }

        // Leer PIDs múltiples
        while (i < argc && strcmp(argv[i], "-r") != 0) {
            if (!isNumeric(argv[i])) {
                printf("Error: PID '%s' no es válido. Debe ser numérico.\n", argv[i]);
                return -1;
            }
            storeProcessInfo(argv[i], queue);
            i++;
        }

        if (i < argc && strcmp(argv[i], "-r") == 0) {
            *hasR = 1;
            if (i != argc - 1) {
                printf("Error: La bandera -r debe estar al final del comando.\n");
                return -1;
            }
        }

    } else {
        // Modo sin -l (1 solo PID)
        if (!isNumeric(argv[i])) {
            printf("Error: PID '%s' no es válido. Debe ser numérico.\n", argv[i]);
            return -1;
        }

        storeProcessInfo(argv[i], queue);
        i++;

        if (i < argc) {
            if (strcmp(argv[i], "-r") == 0) {
                *hasR = 1;
                if (i != argc - 1) {
                    printf("Error: La bandera -r debe estar al final del comando.\n");
                    return -1;
                }
            } else {
                printf("Error: Argumento no reconocido: %s\n", argv[i]);
                return -1;
            }
        }
    }

    return 0;
}

int firstProcessPosition(int argc, char* argv[]){
    for (int i = 1; i<argc; i++){
        if(!((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "-r") == 0))){
            printf("La posición del primer argumento proceso es: %d\n", i);
            return i;
        }
    }

    return -1;
}


void writeReport(int argc, char* argv[], struct Node* head, int firstProcessPosition){
    FILE *report;
    char nombre_archivo[128] = "psinfo-report-";
    struct Node* temp = head;

    for (int i = firstProcessPosition; i < argc; i++) {
    // Ignorar banderas como -l o -r
    if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "-l") == 0) {
        continue;
    }

    strcat(nombre_archivo, argv[i]);

    // Solo añadir '-' si no es el último PID (verificamos si hay más PIDs adelante)
    int j = i + 1;
    while (j < argc && (strcmp(argv[j], "-r") == 0 || strcmp(argv[j], "-l") == 0)){
        j++;
    }

    if (j < argc) {
        strcat(nombre_archivo, "-");
    } else {
        strcat(nombre_archivo, ".info");
        break;
    }
}

    report = fopen(nombre_archivo, "w");
    while (temp != NULL){
        fprintf(report, "%s\n", temp->data);
        temp = temp->next;
    }

}

int main(int argc, char* argv[]) {
    int hasL = 0;
    int hasR = 0;
    struct Node* queue = NULL;

    if (processPIDs(argc, argv, &queue, &hasR, &hasL) != 0) {
        return -1;
    }

    if (hasR) {
        int pos = firstProcessPosition(argc, argv);
        writeReport(argc, argv, queue, pos);
    }

    printProcessQueue(queue);
    return 0;
}

