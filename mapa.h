#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED
#define VISITADO '.'
#define CAMINO '.'
#define ENTRADA 'E'
#define SALIDA 'S'
#define JUGADOR 'P'
#include "jugador.h"
typedef struct {
    int fil;
    int col;
}Celdas;

struct mapaL
{
    char **mat;
    int posyS;
    int posxS;
    bool exit;
};
void llenarMat(char**mat,int fil, int col);
void mostrarMat(char**mat,int fil, int col);
char**crearMatriz(int filas, int columnas);
void eliminarMatriz(char**mat,int filas,int columnas);
int crearLaberinto(struct mapaL *m, int fil, int col,struct player *p);
int buscarVecinos(char**mat,int fil, int col,Celdas*act,Celdas*vecinos);
void checkend(struct mapaL *m,struct player *p);
#endif // FUNCIONES_H_INCLUDED
