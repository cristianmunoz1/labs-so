
#include <stdlib.h>
#include <assert.h>
#include "buffer.h"

buffer_t request_buffer; 

void buffer_init(buffer_t *b, int capacity) {
    b->buff = (int *) malloc (capacity * sizeof(int));
    assert(b->buff != NULL);

    b->capacity = capacity;
    b->in = 0;
    b->out = 0;

    sem_init(&b->mutex, 0, 1);
    sem_init(&b->empty_slots, 0, capacity);
    sem_init(&b->full_slots, 0,0);
}

void buffer_destroy(buffer_t *b){
    free(b->buff);
    sem_destroy(&b->mutex);
    sem_destroy(&b->empty_slots);
    sem_destroy(&b->full_slots);
}

void buffer_insert(buffer_t *b, int item){
    sem_wait(&b->empty_slots);
    sem_wait(&b->mutex);

    b->buff[b->in] = item;
    b->in = (b->in + 1) % b->capacity;

    sem_post(&b->mutex);
    sem_post(&b->full_slots);
}

int buffer_remove(buffer_t *b){
    sem_wait(&b->full_slots);
    sem_wait(&b->mutex);

    int item = b->buff[b->out];
    b->out = (b->out + 1) % b->capacity;

    sem_post(&b->mutex);
    sem_post(&b->empty_slots);

    return item;
}

