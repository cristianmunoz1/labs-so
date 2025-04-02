#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
    char inicio_comando[50] = "cat /proc/";
    strcat(inicio_comando, argv[1]);
    strcat(inicio_comando, "/status");
    printf("%s\n", inicio_comando);
}

