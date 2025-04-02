#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    // Variable para el inicio del comando
    char inicio_comando[50] = "cat /proc/";
    // Concatenamos el inicio del comando con el primer argumento, que sería el pid
    strcat(inicio_comando, argv[1]);
    // Concatenamos con /status para que el programa imprima la información de estado del proceso
    // El comando queda como: cat /proc/123/status
    // Siendo 123 un ejemplo de pid
    strcat(inicio_comando, "/status");

    // Declaramos un buffer para leer cada línea del archivo que retorne la función popen. 128 porque es lo máximo que ocupa una línea. 
    char buffer[128];

    //Abrimos un flujo de archivo para abrir el pipe en modo lectura en el cual guardaremos la información de la salida del comando. 
    FILE *fp = popen(inicio_comando, "r");

    //Si fp es nulo, osea que no se pudo ejecutar el comando o que el comando no retornó nada
    if (fp == NULL) {
        perror("Comando popen falló, intente de nuevo");
        // Retorna 1 para salir
        return 1;
    }
    
    // Fgets ayuda a abrir un pipe del archivo fp y guardar línea por línea en el buffer que definimos anteriormente
    while(fgets(buffer, sizeof(buffer), fp) != NULL){
        printf("%s", buffer);
    }
    
    // Se cierra el pipe para ahorrar recursos. 
    pclose(fp);
    return 0;

}

