#include "pilaDin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void crearPila(tPila*pila)
{
    *pila = NULL;
}

int pilaVacia(const tPila*pila)
{
    if(*pila == NULL)
        return PILA_VACIA;
    return TODO_OK;
}

int apilar(tPila*pila,const void* dato,unsigned tamDato)
{
    tNodo*nue;
    nue = (tNodo*)malloc(sizeof(tNodo));
    if(!nue)
        return SIN_MEM;
    nue->dato = malloc(tamDato);
    if(!nue->dato)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->dato,dato,tamDato);
    nue->tamInfo = tamDato;
    nue->sig = *pila;
    *pila = nue;
    return TODO_OK;
}

int desapilar(tPila*pila,void* dato,unsigned tamDato)
{
    tNodo*elim = *pila;
    if(!elim)
        return PILA_VACIA;
    memcpy(dato,elim->dato,MIN(tamDato,elim->tamInfo));
    *pila = elim->sig;
    free(elim->dato);
    free(elim);
    return TODO_OK;
}

int verTope(tPila*pila,void* dato,unsigned tamDato)
{
    tNodo*elim = *pila;
    if(!elim)
        return PILA_VACIA;
    memcpy(dato,elim->dato,MIN(tamDato,elim->tamInfo));
    return TODO_OK;
}

void vaciarPila(tPila*pila)
{
    tNodo*elim;
    while(*pila)
    {
        elim = *pila;
        *pila = elim->sig;
        free(elim->dato);
        free(elim);
    }
}
