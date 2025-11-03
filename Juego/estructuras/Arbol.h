#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include "estructuras.h"
typedef struct sNodoA
{
    unsigned tamInfo;
    void *dato;
    struct sNodoA *izq;
    struct sNodoA *der;
} tNodoA;

typedef tNodoA *tArbol;

void crearArbol(tArbol *);

int insertarnodoiterativo(tArbol *, void *dato, unsigned cantBytes, int (*cmp)(const void *a, const void*b));
int cmp_int(const void *a, const void *b);
int insertarnodorecursivo(tArbol *arbol, void *dato, unsigned cantBytes, int (*cmp)(const void*a, const void*b));

void Preorden(tArbol *arbol, void accion(void*));
void Inorden(tArbol *arbol, void accion(void*));
void Posorden(tArbol *arbol, void accion(void*));
void mostrar(void*);

void eliminarhojas(tArbol *arbol);
tArbol * mayorvalor(tArbol*);
tArbol *menorvalor(tArbol *arbol);
int altura_arbol(tArbol *);
int contar_hojas(tArbol *);
int contar_nodos(tArbol*);
void eliminarArbol(tArbol*);

void mostrar_clave(void *a);
int cmp_dni(const void *a, const void *b);
int cmp_id(const void *a, const void *b);

void insertarenarboldesdearch(tArbol *arbo, FILE *arch);

void _insertarEnArboldesdeArch(tArbol* arbol, FILE* arch, int li, int ls);

int escompleto(tArbol *arbol);

tArbol *mayor_rama(tArbol *arbol);
tArbol *buscarnodo(tArbol *arbol, const void *dato, int(*cmp)(const void *a, const void *b));
tArbol *nodoremplazo(tArbol *arbol);

void mostrar_jugador(void *a);
void mostrar_indice(void *a);

int Eliminarelem(tArbol *arbol, void *dato, int (*cmp)(const void*a, const void*b));
int Eliminarraiz(tArbol *arbol, void *dato, int (*cmp)(const void*a, const void*b));


#endif // ARBOL_H_INCLUDED
