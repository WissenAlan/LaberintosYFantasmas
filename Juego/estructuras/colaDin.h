#ifndef COLADIN_H_INCLUDED
#define COLADIN_H_INCLUDED
#include "estructuras.h"


typedef struct
{
    tNodo*pri;
    tNodo*ult;
} tCola;

void crearCola(tCola*cola);
int colaInsertar(tCola*cola, const void*dato, unsigned tam);
int sacarDeCola(tCola*cola, void*dato, unsigned tam);
void vaciarCola(tCola*cola);
int verPrimero(tCola*cola, void*dato, unsigned tam);
int colaVacia(tCola *cola);
#endif // COLADIN_H_INCLUDED
