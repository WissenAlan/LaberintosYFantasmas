#ifndef PILADIN_H_INCLUDED
#define PILADIN_H_INCLUDED
#include "estructuras.h"

typedef tNodo* tPila;

void crearPila(tPila*pila);
int pilaVacia(const tPila*pila);
int apilar(tPila*pila, const void* dato, unsigned tamDato);
int desapilar(tPila*pila, void* dato, unsigned tamDato);
int verTope(tPila*pila, void* dato, unsigned tamDato);
void vaciarPila(tPila*pila);


#endif // PILADIN_H_INCLUDED
