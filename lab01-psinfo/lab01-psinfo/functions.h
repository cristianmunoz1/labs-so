#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "list.h"

//Función que escribirá el reporte de los procesos en un archivo
void writeReport(int argc, char* argv[], struct Node* head, int firstProcessPosition);
//Función que encontrará la posición del primer argumento que sea un proceso dentro de un conjunto de argumentos
int firstProcessPosition(int argc, char* argv[]);
//Función que indica si un valor es numérico o no
int isNumeric(const char* str);
//Función que guardará toda la información de un proceso dado su process id en la cola que se le pase por referencia
int storeProcessInfo(char pid[], struct Node** queue);
//Función que imprimirá toda la información de los procesos que hay almacenados en una cola
void printProcessQueue(struct Node* head);
//Función que procesa parcialmente los argumentos que se pasen al comando psinfo, encontrando el primero y las flags. Hace manejo de errores
int processPIDs(int argc, char* argv[], struct Node** queue, int *hasR, int *hasL);

#endif // FUNCTIONS_H
