#include "colaDin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void crearCola(tCola*cola) {
    cola->pri = cola->ult = NULL;
}

int colaInsertar(tCola*p, const void*d, unsigned cantBytes) {
    tNodo *nue = (tNodo *) malloc(sizeof(tNodo));
    if (!nue)
        return SIN_MEM;
    nue->dato = malloc(cantBytes);
    if(!nue->dato)
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->dato, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = NULL;
    if (!p->pri)
        p->pri= nue;
    else
        p->ult->sig = nue;
    p->ult = nue;
    return TODO_OK;
}

int sacarDeCola(tCola*cola, void*dato, unsigned tam) {
    tNodo*elim = cola->pri;
    if (!elim)
        return COLA_VACIA;
    memcpy(dato, elim->dato, MIN(tam, elim->tamInfo));
    cola->pri = elim->sig;
//    if(!cola->pri)
//        cola->ult = NULL;
    free(elim->dato);
    free(elim);
    return TODO_OK;
}

void vaciarCola(tCola*cola) {
    tNodo*elim;
    while (cola->pri) {
        elim = cola->pri;
        cola->pri = elim->sig;
        free(elim->dato);
        free(elim);
    }
}

int verPrimero(tCola*cola, void*dato, unsigned tam) {
    tNodo*elim = cola->pri;
    if (!elim)
        return COLA_VACIA;
    memcpy(dato, elim->dato, MIN(tam, elim->tamInfo));
    return TODO_OK;
}
int colaVacia(tCola *cola) {
    if (cola->pri == NULL)
        return COLA_VACIA;
    return TODO_OK;
}
