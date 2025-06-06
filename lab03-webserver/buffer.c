#include <stdlib.h>
#include <stdio.h>
#include "buffer.h"
buffer_t request_buffer;
void buffer_init(buffer_t *buffer, int capacity) {
    buffer->buf = malloc(sizeof(int) * capacity);
    buffer->capacity = capacity;
    buffer->in = 0;
    buffer->out = 0;
    buffer->count = 0;
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->not_full, NULL);
    pthread_cond_init(&buffer->not_empty, NULL);
}

void buffer_destroy(buffer_t *buffer) {
    free(buffer->buf);
    pthread_mutex_destroy(&buffer->mutex);
    pthread_cond_destroy(&buffer->not_full);
    pthread_cond_destroy(&buffer->not_empty);
}

void buffer_insert(buffer_t *buffer, int item) {
    pthread_mutex_lock(&buffer->mutex);
    while (buffer->count == buffer->capacity)
        pthread_cond_wait(&buffer->not_full, &buffer->mutex);

    buffer->buf[buffer->in] = item;
    buffer->in = (buffer->in + 1) % buffer->capacity;
    buffer->count++;

    pthread_cond_signal(&buffer->not_empty);
    pthread_mutex_unlock(&buffer->mutex);
}

int buffer_remove(buffer_t *buffer) {
    pthread_mutex_lock(&buffer->mutex);
    while (buffer->count == 0)
        pthread_cond_wait(&buffer->not_empty, &buffer->mutex);

    int item = buffer->buf[buffer->out];
    buffer->out = (buffer->out + 1) % buffer->capacity;
    buffer->count--;

    pthread_cond_signal(&buffer->not_full);
    pthread_mutex_unlock(&buffer->mutex);
    return item;
}

