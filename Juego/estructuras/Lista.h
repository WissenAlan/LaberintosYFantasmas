#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"
#define ERROR -1
typedef struct sNodoL
{
    void *dato;
    unsigned tam;
    struct sNodoL *sig;

} tNodoL;

typedef tNodoL *tLista;

void crearLista(tLista *lista);
int insertarPrimero(tLista *lista, const void *dato, unsigned cantBytes);
int insertarFinal(tLista *lista, const void *dato, unsigned cantBytes);
void recorrer(tLista *lista, void accion(void*a));
void mostrar(void *dato);
void vaciarLista(tLista *lista);
int listaVacia(const tLista *lista);
int sacarPrimero(tLista *lista, void *dato, unsigned cantBytes);
int sacarUltimo(tLista *lista, void *dato, unsigned cantBytes);
void ordenarLista(tLista *lista, int (*cmp)(const void *a, const void *b));
tNodoL **buscarMenor(tLista *lista, int (*cmp)(const void *a, const void *b));
int insertarOrdenado(tLista *lista, const void *dato, unsigned cantBytes, int (*cmp)(const void *a, const void*b));
int cmp_int(const void *a, const void *b);

int buscar_cla_n(tLista *lista, const void *clave, int (*cmp)(const void *, const void *), int n, void *dato, unsigned cantBytes);
int buscar_cla_n_recursivo(tLista *lista, const void *clave, int (*cmp)(const void *a, const void *b), int n, void *dato, unsigned cantBytes);
int contar_cla(tLista *lista, const void *clave, int (*cmp)(const void *a, const void *b), int arg);
int contar_cla_recursivo(tLista *lista, const void *clave, int (*cmp)(const void *a, const void *b), int arg);
int buscar_cla(tLista *lista, const void *clave, void *dato, unsigned cantBytes, int (*cmp)(const void *a, const void *b), int arg);
int buscar_cla_recursivo(tLista *lista, const void *clave, void *dato, unsigned cantBytes, int (*cmp)(const void *a, const void *b), int arg);
void recorrer_recursivo(tLista *lista, void accion(void*a));
void recorrer_inverso(tLista *lista, void accion(void*a));
int cmp_jugador_puntos_desc(const void *a, const void *b);

#endif // LISTA_H_INCLUDED
