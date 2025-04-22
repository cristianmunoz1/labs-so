#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Campo{
  const char* clave;
  const char* valor;
};

struct Proceso{
  int pid;
  char valores[8][128];
};

void mostrarUso(){
  printf("Forma de uso incorrecto. \n");
  printf("Uso:\n");
  printf(" ./psinfo <PID>\n");
  printf(" ./psinfo -l <PID1> <PID2> ...\n");
  printf(" ./psinfo -r <PID1> <PID2> ...\n");
};

int esNumero(const char* str){
  for(int i = 0; str[i] != 0; i++){
    if(!isdigit(str[i]))return 0;
  }
  return 1;
};

int main(int argc, char* argv[]){

    if (argc < 2) {
      mostrarUso();
      return 1;
    }
    
    struct Campo campos[] = {
      {"Name:", "Nombre del proceso"},
      {"State:", "Estado"},
      {"VmSize:", "Tamaño total de la imagen de memoria"},
      {"VmExe:", "Tamaño de la memoria TEXT"},
      {"VmData:", "Tamaño de la memoria DATA"},
      {"VmStk:", "Tamaño de la STACK"},
      {"voluntary_ctxt_switches:", "Número de cambios de contexto voluntarios"},
      {"nonvoluntary_ctxt_switches:", "Número de cambios de contexto no voluntarios"}
    };
    
    const int numCampos = sizeof(campos) / sizeof(campos[0]);
    
    int cantidad = 0;
    int esLista = 0;
    int esReporte = 0;
    char** pids = NULL;
    
    if(strcmp(argv[1], "-l")==0){
      if(argc < 3){
        mostrarUso();
        return 1;
      }
      esLista = 1;
      cantidad = argc - 2;
      pids = &argv[2];
    }
    else if(strcmp(argv[1], "-r")==0){
      if(argc < 3){
        mostrarUso();
        return 1;
      }
      esReporte = 1;
      cantidad = argc - 2;
      pids = &argv[2];
    }
    else if(argc == 2 && esNumero(argv[1])){
      cantidad = 1;
      pids = &argv[1];
    }
    else{
      mostrarUso();
      return 1;
    }
    
    for(int i=0; i < cantidad; i++){
      if(!esNumero(pids[i])){
        printf("PID infálido: %s\n", pids[i]);
        mostrarUso();
        return 1;
      }
    }
    
    struct Proceso procesos[cantidad];
    
    for(int p=0; p < cantidad;p++){
      procesos[p].pid= atoi(pids[p]);
      char ruta[256];
      sprintf(ruta, "cat /proc/%d/status 2>&1", procesos[p].pid); 
      
      FILE* comando = popen(ruta, "r");
      if (comando == NULL){
        procesos[p].pid = -1;
        snprintf(procesos[p].valores[0], sizeof(procesos[p].valores[0]), "No se encontró el proceso con PID %s.", pids[p]);
        continue;
      }
      
      for(int i = 0; i < numCampos; i++){
        procesos[p].valores[i][0] = '\0';
      }
      
      char linea[256];
      char contenido[2048]= "";
      while(fgets(linea, sizeof(linea), comando)){
        strcat(contenido, linea);
      }
      pclose(comando);
      
      if(strstr(contenido, "No such file or directory") || strlen(contenido) == 0){
        snprintf(procesos[p].valores[0], sizeof(procesos[p].valores[0]), "No se encontró el proceso con PID %s.", pids[p]);
        continue;
      }
      
      char* lineaPtr = strtok(contenido, "\n");
      while (lineaPtr){
        for(int i = 0; i < numCampos; i++){
          if(strncmp(lineaPtr, campos[i].clave, strlen(campos[i].clave))==0){
            char* dato = strchr(lineaPtr, '\t');
            if (dato){
              while(isspace(*dato)) dato++;
              strncpy(procesos[p].valores[i], dato, sizeof(procesos[p].valores[i]));
              procesos[p].valores[i][strcspn(procesos[p].valores[i], "\n")] = '\0';
            }
          }
        }
        lineaPtr = strtok(NULL, "\n");
      }
    }
    
    if(esReporte){
      char nombreArchivo[256] = "psinfo-report";
      for(int p = 0; p < cantidad; p++){
        char pidStr[16];
        sprintf(pidStr, "-%d", procesos[p].pid);
        strcat(nombreArchivo, pidStr);
      }
      strcat(nombreArchivo, ".info");
      
      FILE* archivo = fopen(nombreArchivo, "w");
      if(archivo == NULL){
        printf("Error al crear el archivo de salida.\n");
        return 1;
      }
      
      for(int p=0; p < cantidad; p++){
        if(strncmp(procesos[p].valores[0], "No se encontró el proceso", 26) == 0){
          fprintf(archivo, "\n%s\n", procesos[p].valores[0]);
          continue;
        }
        
        fprintf(archivo, "\n ---- Información del proceso con PID %d ----\n", procesos[p].pid);
        for(int i = 0; i < numCampos; i++){
          if(procesos[p].valores[i][0] != '\0'){
            fprintf(archivo, "%s: %s\n", campos[i].valor, procesos[p].valores[i]);
          }
        }
      }
      
      fclose(archivo);
      printf("Archivo de salida generado: %s\n", nombreArchivo);
    }
    
    else{
     for(int p = 0; p < cantidad; p++){
        if(strncmp(procesos[p].valores[0], "No se encontró el proceso", 26) == 0){
          printf("\n%s\n", procesos[p].valores[0]);
          continue;
        }
        
        printf("\n---- Información del proceso con PID %d ----\n", procesos[p].pid);
        for(int i=0; i < numCampos; i++){
          if(procesos[p].valores[i][0] != '\0'){
            printf("%s: %s\n", campos[i].valor, procesos[p].valores[i]);
          }
        }
      } 
    }
    
    return 0;
}
