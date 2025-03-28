#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h" 

// Función para almacenar la información de un solo proceso
int storeProcessInfo(char pid[], struct Node** queue) {
    char inicio_comando[50] = "cat /proc/";
    char buffer[1024];
    // concatenamos el inicio del comando con el número de la carpeta del proceso:
    // inicio_comando = cat /proc/123
    strcat(inicio_comando, pid);

    // concatenamos el inicio_comando, que ahora tiene el pid con /status
    // inicio_comando = cat /proc/123/status
    strcat(inicio_comando, "/status");
    
    //Abrimos un flujo de archivo para guardar la salida del comando y lo ejecutamos con popen en modo lectura "r" 
    FILE *fp = popen(inicio_comando, "r");
    
    //Si el archivo está vacío, entonces salimos
    if (fp == NULL) {
        //Mostrar mensaje informativo acerca de errores que pueden pasar al ejecutar popen
        perror("popen");
        return -1;
    }
    
    // Este buffer contendrá toda la información del proceso. De 2048 para asegurarnos de no quedarnos sin espacio
    char processInfo[2048] = ""; 
    
    //Mientras la función fgets en el archivo fp no sea null, vamos a seguir procesando la información de los procesos. 
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strncmp("Name:", buffer, 5) == 0) {
            strcat(processInfo, "Nombre del proceso: ");
            strcat(processInfo, buffer + 6);
        } else if (strncmp("State:", buffer, 6) == 0) {
            strcat(processInfo, "Estado del proceso: ");
            strcat(processInfo, buffer + 7);
        } else if (strncmp("VmSize:", buffer, 7) == 0) {
            strcat(processInfo, "Tamaño de la imagen de memoria: ");
            strcat(processInfo, buffer + 8);
        } else if (strncmp("VmExe:", buffer, 6) == 0) {
            strcat(processInfo, "Tamaño de la memoria TEXT: ");
            strcat(processInfo, buffer + 7);
        } else if (strncmp("VmData:", buffer, 7) == 0) {
            strcat(processInfo, "Tamaño de la memoria DATA: ");
            strcat(processInfo, buffer + 8);
        } else if (strncmp("VmStk:", buffer, 6) == 0) {
            strcat(processInfo, "Tamaño de la memoria STACK: ");
            strcat(processInfo, buffer + 7);
        } else if (strncmp("voluntary_ctxt_switches:", buffer, 24) == 0) {
            strcat(processInfo, "# de cambios de contexto voluntarios: ");
            strcat(processInfo, buffer + 25);
        } else if (strncmp("nonvoluntary_ctxt_switches:", buffer, 27) == 0) {
            strcat(processInfo, "# de cambios de contexto no voluntarios: ");
            strcat(processInfo, buffer + 28);
        }
    }
    
    // Insertamos la información completa en la cola
    insertAtEnd(queue, processInfo);
    
    //Cerramos el flujo de archivo para ahorrar recursos
    fclose(fp);
    return 0;
}

// Función para procesar los argumentos y llamar a la función de almacenamiento
int processPIDs(int argc, char* argv[], struct Node** queue) {

    //Si los argumentos son menos de 2, significa que el usuario no ingresó ningún pid
    if (argc < 2) {
        printf("Por favor, ingresa al menos un PID.\n");
        return -1;
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

    return 0;
}

// Función para imprimir la información de una lista ligada
void printProcessList(struct Node* head) {
    //creamos un nodo temporal que apunte a head
    struct Node* temp = head;
    printf("\nINFORMACIÓN DE LOS PROCESOS: \n");

    //mientras no sea null entonces vamos a imprimir la data del nodo temp que es la información del proceso
    while (temp != NULL) {
        printf("%s\n", temp->data);  
        //Ahora temp debe apuntar al siguiente
        temp = temp->next;
    }
}


int main(int argc, char* argv[]) {
    //inicializamos la cola
    struct Node* queue = NULL;
    
    // Procesar los argumentos y almacenar los pids en la cola
    if (processPIDs(argc, argv, &queue) != 0) {
        // Si hay algún error al procesar salimos 
        return -1;  
    }

    // Imprimir toda la cola después de almacenar la información
    printf("\nTodos los procesos almacenados en la cola:\n");
    // Imprimimos la lista enlazada
    printProcessList(queue);  

    return 0;
}

