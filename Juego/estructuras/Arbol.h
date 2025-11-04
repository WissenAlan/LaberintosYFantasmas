#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"

#define CLAVE_ENCONTRADA 1
#define CLAVE_NO_ENCONTRADA 0
#define ARCHIVO_NO_INDEXADO 0
#define ARCHIVO_INDEXADO 1
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
int insertarnodorecursivo(tArbol *arbol, void *dato, unsigned cantBytes, int (*cmp)(const void*a, const void*b));

int cmp_clave(const void *a, const void *b);

int compararRanking(const void *a, const void *b);
void eliminarArbol(tArbol *pa);
//Funciones de indice.
int buscarEnArchivoConIndice(FILE *fp, const tArbol *pa, tJugadorDatos *datos);
int buscarEnArbol(const tArbol* pa, void *d, unsigned tam,int (*cmp)(const void *, const void *));
int indexarArchivoPersonas(const char *nombreArchivoPersonas, tArbol *p);
int guardarIndiceEnArchivo(const tArbol *p, const char *nombreArchivoIndice);
int cargarIndiceDesdeArchivo(tArbol *pa, const char *nombreArchivoIndice);
void recorrerArbolPreOrder(const tArbol *pa, unsigned n, void *params, void (*accion)(void *, unsigned, unsigned, void *));
void guardarNodoEnArchivo(void *info, unsigned tam, unsigned n, void *params);
void cargarArchivoOrdenadoEnIndiceBalanceado(tArbol *pa, FILE *fp,int inicio,int fin);
int indexarArchivoPersonasOrdenado(const char *nombreArchivoPersonas,tArbol *p);

#endif // ARBOL_H_INCLUDED
