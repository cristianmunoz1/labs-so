// Condicional que evita que un archivo header se incluya varias veces al momento de compilar
#ifndef LIST_H
#define LIST_H

// Definición de la estructura del nodo
struct Node {
    char* data;
    struct Node* next;
};

// Declaración de las funciones de la lista
struct Node* createNode(const char* data);
void insertAtEnd(struct Node** head, const char* data);
void deleteFromFirst(struct Node** head);
void print(struct Node* head);

#endif // LIST_H

