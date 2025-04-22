#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "functions.h"

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
            snprintf(bufferInfo + strlen(bufferInfo), sizeof(bufferInfo) - strlen(bufferInfo), "%s%s", "Nombre del proceso: ", buffer+6);
        } else if (strncmp("State:", buffer, 6) == 0) {
            snprintf(bufferInfo + strlen(bufferInfo), sizeof(bufferInfo) - strlen(bufferInfo), "%s%s", "Estado del proceso: ", buffer+7);
        } else if (strncmp("VmSize:", buffer, 7) == 0) {
            snprintf(bufferInfo + strlen(bufferInfo), sizeof(bufferInfo) - strlen(bufferInfo), "%s%s", "Tamaño de la imagen de memoria: ", buffer+8);
        } else if (strncmp("VmExe:", buffer, 6) == 0) {
            snprintf(bufferInfo + strlen(bufferInfo), sizeof(bufferInfo) - strlen(bufferInfo), "%s%s", "Tamaño de la memoria TEXT: ", buffer+8);
        } else if (strncmp("VmData:", buffer, 7) == 0) {
            snprintf(bufferInfo + strlen(bufferInfo), sizeof(bufferInfo) - strlen(bufferInfo), "%s%s", "Tamaño de la memoria DATA: ", buffer+8);
        } else if (strncmp("VmStk:", buffer, 6) == 0) {
            snprintf(bufferInfo + strlen(bufferInfo), sizeof(bufferInfo) - strlen(bufferInfo), "%s%s", "Tamaño de la memoria STACK: ", buffer+7);
        } else if (strncmp("voluntary_ctxt_switches:", buffer, 24) == 0) {
            snprintf(bufferInfo + strlen(bufferInfo), sizeof(bufferInfo) - strlen(bufferInfo), "%s%s", "# de cambios de contexto Voluntarios: ", buffer+25);
        } else if (strncmp("nonvoluntary_ctxt_switches:", buffer, 27) == 0) {
            snprintf(bufferInfo + strlen(bufferInfo), sizeof(bufferInfo) - strlen(bufferInfo), "%s%s", "# de cambios de contexto No Voluntarios: ", buffer+28);
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


int processPIDs(int argc, char* argv[], struct Node** queue, int *hasR, int *hasL) {
    //Si hay menos de 2 argumentos, solo se ejecutó el comando y no se pasaron pids ni flags
    if (argc < 2) {
        printf("\033[1;33m ------------------- Información de PSINFO --------------------\n\033[0m");
        printf("Uso: psinfo [ -l pid1 pid2 ... ] [ -r ]\n");
        printf("\nPara ejemplos más detallados del comando, visita la man page del programa ejecutando \"man psinfo\"\n\n");
        return -1;
    }
    //Variable contadora para las validaciones de parámetros
    int i = 1;

    // Si el primer argumento es la flag -l entonces se pasa a hasL 1. Se aumenta el contador para validar los otros argumentos. 
    //Opción donde se tiene -l entre los argumentos
    if (strcmp(argv[i], "-l") == 0) {
        *hasL = 1;
        i++;
        

        // Si i siendo 2 es mayor o igual que la cantidad de argumentos, significa que solo se pasó -l y no se pasaron pids. 
        if (i >= argc || strcmp(argv[i], "-r") == 0) {
            printf("\033[1;31m ERROR. PARÁMETROS INCORRECTOS \033[0m\n");
            printf("Después de la flag -l debe ir mínimo un pid. Ejemplo: psinfo -l pid1 pid2 ...\n\n");
            printf("\033[1;33m Para más información acceda a la man page del comando ejecutando: man psinfo\033[0m\n");
            return -1;
        }
       
        //Si llegó a este punto significa que hay más de 2 argumentos
        //Si hay más de 2 argumentos se valida cada uno de ellos, esperando que todos excepto -r si existe, sean numéricos. Si alguno no lo es, retorna un error. 
        while (i < argc && strcmp(argv[i], "-r") != 0) {
            if (!isNumeric(argv[i])) {
                printf("\033[1;31m ERROR. PARÁMETROS INCORRECTOS \033[0m\n");
                printf("Todos los process ids deben ser numéricos. El parámetro que causa el error es: %s ...\n\n", argv[i]);
                printf("\033[1;33m Para más información acceda a la man page del comando ejecutando: man psinfo\033[0m\n");
                return -1;
            }

            //Si no ha retornado error hasta el momento, entonces se guarda la info del proceso en la cola.
            storeProcessInfo(argv[i], queue);
            //Se aumenta en uno para ir iterando sobre los argumentos. 
            i++;
        }
        
        // Si hay más argumentos, presumiblemente es la flag -r. Por lo tanto se verifica, si lo es, se pone hasR en 1
        // Si resulta que en el ciclo anterior se salió sin terminar porque encontró la flag -r, entonces i estará antes de argc-1. Por lo tanto, se 
        // lanza el error y se informa al usuario. 
        if (i < argc && strcmp(argv[i], "-r") == 0) {
            *hasR = 1;
            if (i != argc - 1) {
                printf("\033[1;31m ERROR. PARÁMETROS INCORRECTOS \033[0m\n");
                printf("La bandera -r debe estar al final del comando.\n");
                printf("\033[1;33m Para más información acceda a la man page del comando ejecutando: man psinfo\033[0m\n");

                return -1;
            }
        }
    // Si el programa no tiene la flag -l
    } else {
        // Verifica que el pid sea numérico, si no lo es, salta un error. 
        if (!isNumeric(argv[i])) {
            printf("\033[1;31m ERROR. PARÁMETROS INCORRECTOS \033[0m\n");
            printf("PID '%s' no es válido. Debe ser numérico.\n", argv[i]);
            printf("\033[1;33m Para más información acceda a la man page del comando ejecutando: man psinfo\033[0m\n");
            return -1;
        }
        // Si no saltó el error, significa que el parámetro es correcto y se guarda la información del mismo en la cola. 
        storeProcessInfo(argv[i], queue);
        //Aumentamos el contador para verificar si hay otros argumentos adicionales además del pid pero sin incluir -l
        i++;
        // Si hay más argumentos pero no se pasó -l entonces hay 2 opciones, o es la flag -r, caso en el cual no debe lanzar error
        // o son otros process ids, con lo cual si debe lanzar error. 
        if (i < argc) {
            //Si el argumento coincide con la flag -r se pone hasR en 1
            if (strcmp(argv[i], "-r") == 0) {
                *hasR = 1;
                //Si no es el último argumento, significa que -r no está al final y debe lanzar un error. 
                if (i != argc - 1) {
                    printf("Error: La bandera -r debe estar al final del comando.\n");
                    return -1;
                }
                //Si está de último, el código continúa normalmente.
            //Si no es la flag -r significa que el usuario está tratando de pasar otros pids sin la flag -l, por lo que debe saltar un error. 
            } else {
                printf("\033[1;31m ERROR. PARÁMETROS INCORRECTOS \033[0m\n");
                printf("Si quiere pasar múltiples process id, debe poner la flag -l. Ejemplo: psinfo -l pid1 pid2 ...\n\n");
                printf("\033[1;33m Para más información acceda a la man page del comando ejecutando: man psinfo\033[0m\n");
                return -1;
            }
        }
    }

    return 0;
}

// Función para encontrar la primera posición del argumento que corresponde a un process id
int firstProcessPosition(int argc, char* argv[]){
    //Iteramos sobre todos los argumentos
    for (int i = 1; i<argc; i++){
        // Valida si cada argumento es igual a -l o a -r. Si no es así, entonces retorna la posición, el i. 
        if(!((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "-r") == 0))){
            return i;
        }
    }
    // Si no retornó nada, entonces retorna un error, ya que no habría procesos en los argumentos
    return -1;
}

//Función para escribir un reporte en un archivo .info
void writeReport(int argc, char* argv[], struct Node* head, int firstProcessPosition){
    //Declara el nombre del archivo como report. El cual es un apuntador a un tipo FILE de C. 
    FILE *report;
    //Inicializamos el arreglo de caracteres como psinfo-report- que es el prefijo de todos los archivos .info y donde concatenaremos los pids.
    char nombre_archivo[128] = "psinfo-report-";
    // Creamos un nodo temporal que apunta a la cabeza de la cola para tener la info del primer pid. 
    struct Node* temp = head;

    //Iteramos los argumentos del comando desde la posición del primer argumento que corresponde a un process id. 
    for (int i = firstProcessPosition; i < argc; i++) {
        // Si el argumento corresponde a -l o -r entonces los ignoramos y continuamos. 
        if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "-l") == 0) {
            continue;
        }

        strcat(nombre_archivo, argv[i]);
        // Solo añadir '-' si no es el último PID (verificamos si hay más PIDs adelante)
        // Tomamos la j como el argumento siguiente al que estamos verificando
        int j = i + 1;
        // Mientras el j no sea el último argumento y igual a -l o -r entonces que continúe, aumentamos el contador. 
        while (j < argc && (strcmp(argv[j], "-r") == 0 || strcmp(argv[j], "-l") == 0)){
            j++;
        }
        // Si el j no es todavía el último argumento, que debería ser -r, entonces se agrega el -
        if (j < argc) {
            strcat(nombre_archivo, "-");
        // Si es ya el último, entonces es -r, no agregamos nada sino que le ponemos la extensión .info y salimos del for. 
        } else {
            strcat(nombre_archivo, ".info");
            break;
        }
    }
    
    // Abrimos el archivo con fopen en modo escritura
    report = fopen(nombre_archivo, "w");
    // Mientras el nodo temporal tenga datos, osea, no sea nulo, (que no haya llegado al final de la cola) entonces que con fprintf ponga la info 
    // del proceso actual en el archivo y avance hacia el nodo siguiente. 
    while (temp != NULL){
        fprintf(report, "%s\n", temp->data);
        temp = temp->next;
    }

}

