
#ifndef BUFFER_H
#define BUFFER_H

//Incluímos semáforos para manejar las race condition
#include <semaphore.h>

//Definimos la estructura del buffer
typedef struct {
    int* buff;      //  Puntero al array del búfer
    int capacity;   // Capacidad máxima del buffer
    int in;         // Entero que indica en qué posición agregar peticiones
    int out;        // Entero que indica en qué posición sacar peticiones
    
    sem_t mutex;    // Semáforo para exclusión mutua
    sem_t empty_slots;  // Cuenta los espacios vacíos
    sem_t full_slots;   // Cuenta los espacios llenos
} buffer_t;

extern buffer_t request_buffer;

void buffer_init(buffer_t *b, int capacity);
void buffer_destroy(buffer_t *b);
void buffer_insert(buffer_t *b, int item);
int buffer_remove(buffer_t *b);

#endif

