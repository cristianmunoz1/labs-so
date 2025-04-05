#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void writeReport(int argc, char* argv[], struct Node* head, int firstProcessPosition);
int firstProcessPosition(int argc, char* argv[]);
int storeProcessInfo(char pid[], struct Node** queue){
    // Variable para el inicio del comando
    char inicio_comando[50] = "cat /proc/";
    // Concatenamos el inicio del comando con el primer argumento, que sería el pid
    strcat(inicio_comando, pid);
    // Concatenamos con /status para que el programa imprima la información de estado del proceso
    // El comando queda como: cat /proc/123/status
    // Siendo 123 un ejemplo de pid
    strcat(inicio_comando, "/status");
    // Declaramos un buffer para leer cada línea del archivo que retorne la función popen. 128 porque es lo máximo que ocupa una línea. 
    char buffer[128];
    //Buffer que almacenará toda la información necesaria de un proceso y la pasará a la data de un nuevo nodo
    char bufferInfo[2048] = "";

    //Abrimos un flujo de archivo para abrir el pipe en modo lectura en el cual guardaremos la información de la salida del comando. 
    FILE *fp = popen(inicio_comando, "r");

    //Si fp es nulo, osea que no se pudo ejecutar el comando o que el comando no retornó nada
    if (fp == NULL) {
        perror("Comando popen falló, intente de nuevo");
        // Retorna 1 para salir
        return 1;
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

    insertAtEnd(queue, bufferInfo);
    pclose(fp);
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
int processPIDs(int argc, char* argv[], struct Node** queue, int *hasR, int *hasL){
      
    for(int i = 1; i<argc; i++){
        if(strcmp(argv[i], "-l") == 0){
            *hasL = 1;
        } else if(strcmp(argv[i], "-r") == 0) {
            *hasR = 1;
        }
    }
    //Si pasa solo un argumento significa que no pasó ningún pid, por lo que se le advierte al usuario
    if (argc < 2){
        printf("Por favor, ingresa al menos un Process id. \n");
    }

    // si el primer argumento coincide con -l entonces continuamos con la lectura de argumentos
    if (strcmp(argv[1], "-l") == 0) {
        // Si el nro de argumentos es igual a 2, significa que después de -l no ingresó ningún pid
        if (argc == 2) {
            printf("Por favor, ingresa al menos un PID después de -l.\n");
            return -1;
        }

        // Si hasta este punto no hemos salido del programa, entonces significa que los argumentos están correctamente escritos
        // Procesamos todos los PIDs que vienen después de -l
        // Iniciamos en 2 ya que el arg[0] es el mismo comando, arg[1] es -l 
        for (int i = 2; i < argc; i++) {
            //Guardamos la información del proceso en la cola queue
            storeProcessInfo(argv[i], queue);
        }
    } else {
        // Si no se pasa -l pero hay varios pids, mostramos un mensaje
        if (argc > 2) {
            printf("Si desea imprimir la información de varios PIDs, agrega el parámetro -l.\n");
            printf("Ejemplo: psinfo -l 123 456\n");
            return -1;
        }

        // Si no se pasa -l procesamos un solo pid
        storeProcessInfo(argv[1], queue);
    }

    if (*hasR == 1){
        writeReport(argc, argv, *queue, firstProcessPosition(argc, argv));
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

    for (int i = firstProcessPosition; i<argc; i++){
        if (i!=argc-1){
            strcat(nombre_archivo, argv[i]);
            strcat(nombre_archivo, "-");
        } else {
            strcat(nombre_archivo, argv[i]);
            strcat(nombre_archivo, ".info");
        }
    }

    report = fopen(nombre_archivo, "w");
    while (temp != NULL){
        fprintf(report, "%s\n", temp->data);
        temp = temp->next;
    }

}

int main(int argc, char* argv[]){
    //Declaración de variables necesarias
    //Son 1 si en los argumentos está la flag r o l
    int hasL = 0;
    int hasR = 0;
    //Cola donde se va a almacenar la info de los procesos.  
    struct Node* queue = NULL;
    if (processPIDs(argc, argv, &queue, &hasR, &hasL) != 0){
        // Si no retorna 0, es porque hay un error en el código
        return -1;
    }
    printf("hasR %d", hasR);
    printf("hasL %d", hasL);
    printProcessQueue(queue);
    return 0;
}

