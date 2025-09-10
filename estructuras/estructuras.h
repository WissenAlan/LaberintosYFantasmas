#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#define TODO_OK 0
#define COLA_LLENA 1
#define COLA_VACIA 2
#define PILA_LLENA 1
#define PILA_VACIA 2
#define SIN_MEM 3
#define MIN(X,Y) ((X)>(Y)?(X):(Y))

typedef struct sNodo
{
    void*dato;
    unsigned tamInfo;
    struct sNodo * sig;
}tNodo;



#endif // ESTRUCTURAS_H_INCLUDED
