#ifndef PILADIN_H_INCLUDED
#define PILADIN_H_INCLUDED
#include "estructuras.h"

#define TODO_OK 0
#define PILA_LLENA 1
#define PILA_VACIA 2
#define SIN_MEM 3
#define MIN(X,Y) ((X)>(Y)?(X):(Y))

typedef tNodo* tPila;

void crearPila(tPila*pila);
int pilaVacia(const tPila*pila);
int apilar(tPila*pila,const void* dato,unsigned tamDato);
int desapilar(tPila*pila,void* dato,unsigned tamDato);
int verTope(tPila*pila,void* dato,unsigned tamDato);
void vaciarPila(tPila*pila);


#endif // PILADIN_H_INCLUDED
