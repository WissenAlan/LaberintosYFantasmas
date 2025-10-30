#include "header/mapa.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estructuras/pilaDin.h"
int crearMapa(tMapa *m) {
    m->exit = FALSE;
    m->posxS = m->posyS = m->colMapa = m->filMapa = 0;
    m->mat = NULL;
}
char**crearMatriz(int filas, int columnas) {
    char**mat = (char**)malloc(filas * sizeof(char*));
    if (!mat)
        return NULL;
    for (int i = 0; i < filas; i++) {
        mat[i] = (char*) malloc(columnas * sizeof(char));
        if (mat[i] == NULL) {
            for (int j = 0; j < i; j++)
                free(mat[j]);
            free(mat);
            return NULL;
        }
    }
    return mat;
}

void eliminarMatriz(char**mat, int filas, int columnas) {
    for (int i = 0; i < filas; i++)
        free(mat[i]);
    free(mat);
}

/*
(ERIK)
El siguiente algoritmo inicializa la matriz de la siguiente forma:
##############################
##  ##  ##  ##  ##  ##  ##  ##
##############################
##  ##  ##  ##  ##  ##  ##  ##
##############################
##  ##  ##  ##  ##  ##  ##  ##
##############################
##  ##  ##  ##  ##  ##  ##  ##
##############################
##  ##  ##  ##  ##  ##  ##  ##
##############################

El espacio vacio sirve como nodo para el algoritmo de formacion del mapa
*/

void llenarMat(char** mat, int fil, int col) {
    int x, y, i;
    for (x = 0; x < fil; x++) {
        memset(mat[x], PARED, col);  /// Llenar fila completa de una vez
    }
    for (x = ESPACIADO; x < fil - ESPACIADO; x += ESPACIADO + 1) {
        for (y = ESPACIADO; y < col - ESPACIADO; y += ESPACIADO + 1) {
            for (i = 0; i < ESPACIADO; i++)
                memset(&mat[x + i][y], CELDA, ESPACIADO);
        }
    }
}

void mostrarMat(char**mat, int fil, int col) {
    for (int x = 0; x < fil; x++)
        printf("     %s\n", mat[x]);
}

int crearLaberinto(tMapa *m, int filMod, int colMod, tJugador *pJug, int fant, int prem, int ext) {
    int cantVecinos;
    int r;
    int cantBoni = prem + ext;
    int cantFan = fant - 1;
    tPila p;
    pJug->posx = 1;
    Celdas act = {1, 1};
    Celdas vecinos[4];
    Celdas pared;
    srand(time(NULL));
    crearPila(&p);
    apilar(&p, &act, sizeof(Celdas));                   ///Apilo la primera celda
    while (pilaVacia(&p) == TODO_OK) {
        desapilar(&p, &act, sizeof(Celdas));            ///saco la ultima celda modificada
        m->mat[act.fil][act.col] = VISITADO;   ///<-----------Marco como visitado
        cantVecinos = buscarVecinos(m->mat, filMod, colMod, &act, vecinos); /// calculo los vecinos que hay disponibles y los guatdo en un vector
        if (cantVecinos) {
            apilar(&p, &act, sizeof(Celdas));           /// Como hay un vecino al que saltar lo apilo nuevamente
            r = rand() % cantVecinos;///<--------------- Selecciono un vecino random de lo que hay
            pared.fil = act.fil + vecinos[r].fil / 2;
            pared.col = act.col + vecinos[r].col / 2;
            m->mat[pared.fil][pared.col] = CAMINO;
            m->mat[pared.fil][pared.col] = CAMINO;
            vecinos[r].col += act.col;
            vecinos[r].fil += act.fil;
            apilar(&p, &vecinos[r], sizeof(Celdas));/// <----------------- Apilo la celda que se acaba de usar para que sea la ultima modificada
        } else {
            /// ATENCION!!!
            /// el sigiente algoritmo es para dererminar si se pone un fantasma o una bonificacion a traves de los numeros random
            /// fue hecho a las 3:55 am. luego de pelear con un error que se solucionaba poniendo un parentesis
            /// el que quiera hacerlo mejor, bienvenido sea ATTE: ERIK O.o
            /// LO VA A HACER ALAN ATTE:Haziel
            if (cantBoni && !(rand() % 10)) {
                m->mat[act.fil][act.col] = BONIFICACION;
                cantBoni --;
            } else if (cantFan > 0
                       && act.fil > filMod / 10
                       && act.col > colMod / 10
                       && rand() % 3 == 0) {
                m->mat[act.fil][act.col] = FANTASMA;
                //ubicar en vector de fantasmas!
                cantFan --;
            }
        }
    }
    for (int i = 0; i < filMod; i++) {
        for (int j = 0; j < colMod; j++) {
            if (m->mat[i][j] == VISITADO || m->mat[i][j] == CAMINO)
                m->mat[i][j] = CELDA;
        }
    }
//    act.col = 1;
//    act.fil = 1;
//    apilar(&p, &act, sizeof(Celdas));
//    while (pilaVacia(&p) == TODO_OK) {
//        desapilar(&p, &act, sizeof(Celdas));
//        m->mat[act.fil][act.col] = VISITADO;
//        cantVecinos = buscarVecinos(m->mat, filMod, colMod, &act, vecinos);
//        if (cantVecinos) {
//            apilar(&p, &act, sizeof(Celdas));
//            r = rand() % cantVecinos;
//            pared.fil = act.fil + vecinos[r].fil / 2;
//            pared.col = act.col + vecinos[r].col / 2;
//            m->mat[pared.fil][pared.col] = CAMINO;
//            m->mat[pared.fil][pared.col] = CAMINO;
//            vecinos[r].col += act.col;
//            vecinos[r].fil += act.fil;
//            apilar(&p, &vecinos[r], sizeof(Celdas));
//        } else {
//            if (cantBoni && !(rand() % 10)) {
//                m->mat[act.fil][act.col] = BONIFICACION;
//                cantBoni --;
//            } else if (cantFan
//                       && act.fil > filMod / 10
//                       && act.col > colMod / 10
//                       && rand() % 3 == 0) {
//                m->mat[act.fil][act.col] = FANTASMA;
//                cantFan --;
//            }
//        }
//    }
//    for (int i = 0; i < filMod; i++) {
//        for (int j = 0; j < colMod; j++) {
//            if (m->mat[i][j] == VISITADO || m->mat[i][j] == CAMINO)
//                m->mat[i][j] = CELDA;
//        }
//    }
    m->mat[act.fil - 1][act.col] = ENTRADA;
    m->mat[act.fil][act.col] = JUGADOR;
    pJug->posx = act.fil;
    pJug->posy = act.col;
    m->mat[filMod - 1][vecinos[r].col] = SALIDA ;
    m->mat[filMod - 2][vecinos[r].col] = FANTASMA ;
    m->posxS = filMod - 1;
    m->posyS = vecinos[r].col;
    m->jugadorMuerto= FALSE;
    m->exit= FALSE;
    vaciarPila(&p);
    return VERDADERO;
}

int buscarVecinos(char**mat, int fil, int col, Celdas*act, Celdas*vecinos) {
    int posx[] = { -2, 0, 2, 0 }; /// Posiciones validas que se pueden dar    ##############
    int posy[] = { 0, 2, 0, -2 }; /// solo se pueden ver considerar aquellas  ##v ##ac##  ##
                                  /// que no se encuentren visitadas y sean   ##############
                                  /// nodos validos                           ##  ##v ##  ##
                                  ///                                         ##############
                                  ///
                                  /// pd: el algoritmo esta modificado para que rompa mas de lo que
                                  /// deberia para poder generar mas espacio, de lo contrario solo habria
                                  /// un camino y seria mas complejo
    int cant = 0;
    for (int i = 0; i < 4; i++) { /// en esta condicion bastante larga revisamos:
        if ((act->fil + posx[i]) > 0 ///<------ que no se salga de la matriz
                && (act->fil + posx[i]) < fil
                && (act->col + posy[i]) > 0
                && (act->col + posy[i]) < col
                && mat[act->fil + posx[i]][act->col + posy[i]] != VISITADO ///<------ y que no este:visitado
                && mat[act->fil + posx[i]][act->col + posy[i]] != FANTASMA/// NO sea un fntasma
                && mat[act->fil + posx[i]][act->col + posy[i]] != BONIFICACION) {  /// NI sea una bonificacion
            vecinos[cant].fil = posx[i];
            vecinos[cant].col = posy[i];
            cant++;
        }
    }
    return cant;
}
void checkend(tMapa *m, tJugador *pJug) {
    if (pJug->posx == m->posxS && pJug->posy == m->posyS)
        m->exit = VERDADERO;
}
void checklifes(tMapa *m, tJugador *pJug)
{
    if(pJug->vidas == 0)
    {
        m->jugadorMuerto= VERDADERO;
    }
}
int llenarMapa(tJugador *p, tMapa* m, int fant, int prem, int ext) {
    int fil, col;
    int cont = 0;
    m->exit = FALSE;
    m->filMapa = m->filMapa % 2 == 0 ? m->filMapa - 1 : m->filMapa;  /// truncamos si es par, ya que el algoritmo utilizadi
    m->colMapa = m->colMapa % 2 == 0 ? m->colMapa - 1 : m->colMapa;  /// unicamente funciona si se utiliza un tamaño impar :(
    m->mat = crearMatriz(m->filMapa, m->colMapa);
    if (!m->mat)
        return FALSE;
    llenarMat(m->mat, m->filMapa, m->colMapa);
    crearLaberinto(m, m->filMapa, m->colMapa, p, fant, prem, ext);
    return VERDADERO;
}

