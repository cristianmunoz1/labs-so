
#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>

typedef struct {
    int *buf;
    int capacity;
    int in;
    int out;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} buffer_t;

void buffer_init(buffer_t *buffer, int capacity);
void buffer_destroy(buffer_t *buffer);
void buffer_insert(buffer_t *buffer, int item);
int buffer_remove(buffer_t *buffer);

extern buffer_t request_buffer;

#endif

