#include "colaDin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void crearCola(tCola*cola)
{
    cola->pri = NULL;
}

int colaInsertar(tCola*cola,const void*dato,unsigned tam)
{
    tNodo*nue;
    nue = (tNodo*)malloc(sizeof(tNodo));
    if(!nue)
        return SIN_MEM;
    nue->dato = malloc(tam);
    if(!nue->dato)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->dato,dato,tam);
    nue->tamInfo = tam;
    nue->sig = NULL;

    if(!cola->pri)
        cola->pri = nue;
    else
        cola->ult->sig = nue;
    cola->ult = nue;

    return TODO_OK;
}

int sacarDeCola(tCola*cola,void*dato,unsigned tam)
{
    tNodo*elim = cola->pri;
    if(!elim)
        return COLA_VACIA;
    memcpy(dato,elim->dato,MIN(tam,elim->tamInfo));
    cola->pri = elim->sig;
    free(elim->dato);
    free(elim);
    return TODO_OK;
}

void vaciarCola(tCola*cola)
{
    tNodo*elim;
    while(cola->pri)
    {
        elim = cola->pri;
        cola->pri = elim->sig;
        free(elim->dato);
        free(elim);
    }
}

int verPrimero(tCola*cola,void*dato,unsigned tam)
{
    tNodo*elim = cola->pri;
    if(!elim)
        return COLA_VACIA;
    memcpy(dato,elim->dato,MIN(tam,elim->tamInfo));
    return TODO_OK;
}
