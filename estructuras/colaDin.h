#ifndef COLADIN_H_INCLUDED
#define COLADIN_H_INCLUDED
#include "estructuras.h"
#define TODO_OK 0
#define COLA_LLENA 1
#define COLA_VACIA 2
#define SIN_MEM 3
#define MIN(X,Y) ((X)>(Y)?(X):(Y))

typedef struct
{
    tNodo*pri;
    tNodo*ult;
}tCola;

void crearCola(tCola*cola);
int colaInsertar(tCola*cola,const void*dato,unsigned tam);
int sacarDeCola(tCola*cola,void*dato,unsigned tam);
void vaciarCola(tCola*cola);
int verPrimero(tCola*cola,void*dato,unsigned tam);

#endif // COLADIN_H_INCLUDED
