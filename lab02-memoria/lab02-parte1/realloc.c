#include <stdlib.h>
#include <stdio.h>

typedef struct{
    int* data;
    size_t size;
} Vector;


void init(Vector* v) {
    v->data = NULL;
    v->size = 0;
}

void push(Vector *v, int value){
    v->data = realloc(v->data, (v->size+1) * sizeof(int));
    if(v->data == NULL){
        fprintf(stderr, "Error al asignar memoria\n");
        exit(1);
    }
    v->data[v->size] = value;
    v->size++;
}

void print(Vector *v) {
    for (size_t i = 0; i < v->size; ++i)
        printf("%d ", v->data[i]);
    printf("\n");
}

void free_vector(Vector *v) {
    free(v->data);
    v->data = NULL;
    v->size = 0;
}

int main(){
    Vector v;
    init(&v);
    
    for(int i = 0; i<100; i++){
        push(&v, i);
    }

    print(&v);

    free_vector(&v);
    return 0;
    
}
