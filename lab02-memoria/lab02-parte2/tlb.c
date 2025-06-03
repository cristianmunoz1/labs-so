#include <stdio.h>  // printf, fprintf
#include <stdlib.h> // exit, calloc, free
#include <time.h>   // clock_gettime
#include <unistd.h> // sysconf

// Función utilitaria para manejar errores
#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s pages trials\n", argv[0]);
    exit(EXIT_FAILURE);
  }
    
  long PAGESIZE = sysconf(_SC_PAGESIZE); // Tamaño de la página
  long jump = PAGESIZE / sizeof(int); // Cuál debe ser el salto para asegurar 
  				 // que se acceda una página diferente cada iteración? 
  int pages = atoi(argv[1]); // Este valor es el que varía para observar el cambio 
  				 // en el tiempo de ejecución del programa
  int trials = atoi(argv[2]);// El experimento se debe repetir para obtener un 
  				 //valor confiable en la medición del tiempo
  				 
  //Inicializar jump, pages y trials...
  
  // Se crea e inicializa el vector
  int *a = calloc(pages, PAGESIZE);
  
  struct timespec start, end;
  
  //Punto inicial de dedida de tiempo
  if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start) == -1)
    handle_error("clock_gettime");

  //Experimento
  for (int j = 0; j < trials; j++) {
    for (int i = 0; i < pages * jump; i += jump)
      a[i] += 1;
  }

  //Punto final de medida de tiempo
  if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end) == -1)
    handle_error("clock_gettime");

  // Imprime tiempo transcurrido del experimento en nanosegundos
  printf("%f\n",
         ((end.tv_sec - start.tv_sec) * 1E9 + end.tv_nsec - start.tv_nsec) /
             (trials * pages));
  free(a);
  return 0;
}
