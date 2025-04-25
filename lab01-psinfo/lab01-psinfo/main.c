//Librería que contiene el método para validar si un caracter es dígito o no
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "functions.h"

int main(int argc, char* argv[]) {
    int hasL = 0;
    int hasR = 0;
    struct Node* queue = NULL;

    if (processPIDs(argc, argv, &queue, &hasR, &hasL) != 0) {
        return -1;
    }

    if (hasR) {
        int pos = firstProcessPosition(argc, argv);
        writeReport(argc, argv, queue, pos);
    }

    printProcessQueue(queue);
    return 0;
}

