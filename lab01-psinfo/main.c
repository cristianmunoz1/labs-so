#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


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

int main(int argc, char* argv[]){
    struct Node* queue = NULL;
    for(int i=1; i<argc; i++){
        storeProcessInfo(argv[i], &queue);
    }

    printProcessQueue(queue);
    return 0;
}

