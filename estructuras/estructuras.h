#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

typedef struct sNodo
{
    void*dato;
    unsigned tamInfo;
    struct sNodo * sig;
}tNodo;



#endif // ESTRUCTURAS_H_INCLUDED
