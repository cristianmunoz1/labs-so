#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// Crear un nuevo nodo con una cadena de texto dinámica
struct Node* createNode(const char* data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    // Copia dinámica de la cadena
    newNode->data = strdup(data);  
    newNode->next = NULL;
    return newNode;
}

// Insertar al final de la lista 
void insertAtEnd(struct Node** head, const char* data) {
    //Como insertamos al final, entonces creamos un nuevo nodo con la data que le pasamos como arg a la función
    struct Node* newNode = createNode(data);
    //Si no hay head, osea que la cola esté vacía, entonces head ahora debe apuntar a newNodo
    if (*head == NULL) {
        *head = newNode; 
        return;
    }
    //si la lista no está vacía, ejecuta lo siguiente
    // Crea un nodo temporal que apunta a head
    struct Node* temp = *head;
    //Mientras este nodo temporal tenga siguiente va a avanzar. Solo se detiene en el último nodo. Entonces temp ahora apunta al último,
    while (temp->next != NULL) {
        temp = temp->next;  
    }
    //A este último nodo, le asignamos como siguiente el nodo que acabamos de crear. 
    temp->next = newNode; 
}

// Eliminar el primer nodo de la lista (FIFO, sacar primero al primero que entra)
void deleteFromFirst(struct Node** head) {
    // si la lista está vacía que lo imprima
    if (*head == NULL) {
        printf("La lista enlazada está vacía\n");
        return;
    }
    // Se crea un nodo temporal que apunta a head
    struct Node* temp = *head;
    // Ahora cambiamos a head y lo ponemos a apuntar al siguiente del temporal. Que era el head antiguo. 
    *head = temp->next;
    // Se libera memoria
    free(temp);
}

// Imprimir los elementos de la lista
void print(struct Node* head) {
    // Se crea un nuevo nodo apuntando a head
    struct Node* temp = head;
    // Mientras este nodo que apuntaba a head sea diferente de nulo, osea que hay data, se imprime y se pasa al siguiente. 
    while (temp != NULL) {
        printf("%s -> ", temp->data);
        temp = temp->next;
    }
    //Imprimimos NULL Para saber que llegamos al final de la lista. 
    printf("NULL\n");
}

