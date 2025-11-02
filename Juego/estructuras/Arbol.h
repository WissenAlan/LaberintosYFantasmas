#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct sNodoA{
unsigned tamInfo;
void *dato;
struct sNodoA *izq;
struct sNodoA *der;
}tNodoA;

typedef tNodoA *tArbol;

typedef struct {
    char nombre[7];
    int puntaje;
} tRanking;

typedef struct{

unsigned clave;
unsigned numRegistro;

}tClave;

typedef struct{

int id;
char nombre[40];
int total_puntos;
int partidas_jugadas;

}tJugadorDatos;

typedef struct
{
    int id_partida;         // ID autoincremental
    int id_jugador;         // ID del jugador
    int puntaje;            // Puntos obtenidos
    int movimientos;        // Movimientos realizados
} tPartidaDatos;

typedef struct{
int clave;
long pos;

}tIndice;



void crearArbol(tArbol *);

int insertarnodoiterativo(tArbol *, void *dato, unsigned cantBytes, int (*cmp)(const void *a,const void*b));
int cmp_int(const void *a,const void *b);
int insertarnodorecursivo(tArbol *arbol,void *dato,unsigned cantBytes,int (*cmp)(const void*a,const void*b));

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
int cmp_dni(const void *a,const void *b);
int cmp_id(const void *a,const void *b);

void insertarenarboldesdearch(tArbol *arbo, FILE *arch);

void _insertarEnArboldesdeArch(tArbol* arbol, FILE* arch, int li, int ls);

int escompleto(tArbol *arbol);

tArbol *mayor_rama(tArbol *arbol);
tArbol *buscarnodo(tArbol *arbol,const void *dato,int(*cmp)(const void *a,const void *b));
tArbol *nodoremplazo (tArbol *arbol);

void mostrar_jugador(void *a);
void mostrar_indice(void *a);

int Eliminarelem(tArbol *arbol, void *dato,int (*cmp)(const void*a,const void*b));
int Eliminarraiz(tArbol *arbol, void *dato,int (*cmp)(const void*a,const void*b));


int cmp_clave(const void *a, const void *b);

int insertarArchaArbol(tArbol *arbol,const char *arch);
int buscarJugadorPorNombre(const char *nombreArchivo, const char *nombreBuscado, tJugadorDatos *jugadorEncontrado);
int crearIndice(tArbol *arbol, const char *arch);
void guardarInorden(tArbol *arbol, FILE *pf);
int buscarJugadorPorId(tArbol *arbol, const char *nombreArchivo, int idBuscado);
int compararRanking(const void *a, const void *b);

#endif // ARBOL_H_INCLUDED
