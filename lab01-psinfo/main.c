#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int printProcessInfo(char pid[]){
    char inicio_comando[50] = "cat /proc/";
    char buffer[1024];
    int i;
    
    strcat(inicio_comando, pid);
    strcat(inicio_comando, "/status");
    printf("\n");
    //printf("El comando a ejecutar es: %s\n", inicio_comando);
    FILE *fp = popen(inicio_comando, "r");

    while(fgets(buffer, sizeof(buffer), fp) != NULL){
       
        if(strncmp("Name:", buffer, 5) == 0){
            printf("Nombre del proceso: %s", buffer + 6);
        } else if (strncmp("State:", buffer, 6) == 0){
            printf("Estado del proceso: %s", buffer + 7);
        } else if (strncmp("VmSize:", buffer, 7) == 0){
            printf("Tamaño de la imagen de memoria: %s", buffer + 8);
        } else if (strncmp("VmExe:", buffer, 6) == 0){
            printf("Tamaño de la memoria TEXT: %s", buffer + 7);
        } else if (strncmp("VmData:", buffer, 7) == 0){
            printf("Tamaño de la memoria DATA: %s", buffer + 8);
        } else if (strncmp("VmStk:", buffer, 6) == 0){
            printf("Tamaño de la memoria STACK: %s", buffer + 7);
        } else if (strncmp("voluntary_ctxt_switches:", buffer, 24) == 0){
            printf("# de cambios de contexto voluntarios: %s", buffer + 25);
        } else if (strncmp("nonvoluntary_ctxt_switches:", buffer, 27) == 0){
            printf("# de cambios de contexto no voluntarios: %s", buffer + 28);
        }
    }
    printf("\n--------------------------------------\n");
    fclose(fp);
    return 0;
 
}

int main(int argc, char* argv[]){
    int i; //Iterador para el for
    printf("\n");
    printf("INFORMACIÓN DE LOS PROCESOS: \n");
    
    for (i=1; i<argc; i++){
        printProcessInfo(argv[i]);
    }
    return 0;
}
