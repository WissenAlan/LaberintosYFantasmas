#include "mapa.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estructuras/pilaDin.h"

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

void llenarMat(char**mat, int fil, int col) {
    int x, y, i, j;
    for (x = 0; x < fil; x++) {
        for (y = 0; y < col; y++)
            mat[x][y] = PARED;
    }
    for (x = ESPACIADO ; x < fil - ESPACIADO; x += ESPACIADO + 1) {
        for (y = ESPACIADO ; y < col - ESPACIADO; y += ESPACIADO + 1) {
            for (i = 0; i < ESPACIADO; i++) {
                for (j = 0; j < ESPACIADO; j++)
                    mat[x + i][y + j] = CELDA;
            }
        }
    }
}

void mostrarMat(char**mat, int fil, int col) {
    for (int x = 0; x < fil; x++) {
        printf("     ");
//        for (int y = 0; y < col; y++)
            printf("%s", mat[x]);
        printf("\n");
    }
}

int crearLaberinto(tMapa *m, int filMod, int colMod, player *jugador, int fant, int prem, int ext) {
    int cantVecinos;
    int r;
    int cantBoni = prem + ext;
    int cantFan = fant-1;
    tPila p;
    jugador->posx = 1;
    Celdas act = {1, 1};
    Celdas vecinos[4];
    Celdas pared;
    srand(time(NULL));
    crearPila(&p);
    apilar(&p, &act, sizeof(Celdas));
    while (pilaVacia(&p) == TODO_OK) {
        desapilar(&p, &act, sizeof(Celdas));
        m->mat[act.fil][act.col] = VISITADO;
        cantVecinos = buscarVecinos(m->mat, filMod, colMod, &act, vecinos);
        if (cantVecinos) {
            apilar(&p, &act, sizeof(Celdas));
            r = rand() % cantVecinos;
            pared.fil = act.fil + vecinos[r].fil / 2;
            pared.col = act.col + vecinos[r].col / 2;
            m->mat[pared.fil][pared.col] = CAMINO;
            m->mat[pared.fil][pared.col] = CAMINO;
            vecinos[r].col += act.col;
            vecinos[r].fil += act.fil;
            apilar(&p, &vecinos[r], sizeof(Celdas));
        }
    }
    for (int i = 0; i < filMod; i++) {
        for (int j = 0; j < colMod; j++) {
            if (m->mat[i][j] == VISITADO || m->mat[i][j] == CAMINO)
                m->mat[i][j] = CELDA;
        }
    }
    act.col = 1;
    act.fil = 1;
    apilar(&p, &act, sizeof(Celdas));
    while (pilaVacia(&p) == TODO_OK) {
        desapilar(&p, &act, sizeof(Celdas));
        m->mat[act.fil][act.col] = VISITADO;
        cantVecinos = buscarVecinos(m->mat, filMod, colMod, &act, vecinos);
        if (cantVecinos) {
            apilar(&p, &act, sizeof(Celdas));
            r = rand() % cantVecinos;
            pared.fil = act.fil + vecinos[r].fil / 2;
            pared.col = act.col + vecinos[r].col / 2;
            m->mat[pared.fil][pared.col] = CAMINO;
            m->mat[pared.fil][pared.col] = CAMINO;
            vecinos[r].col += act.col;
            vecinos[r].fil += act.fil;
            apilar(&p, &vecinos[r], sizeof(Celdas));
        } else {
            if (cantBoni && !(rand() % 10)) {
                m->mat[act.fil][act.col] = BONIFICACION;
                cantBoni --;
            } else if (cantFan
                       && act.fil > filMod / 10
                       && act.col > colMod / 10
                       && rand() % 3 == 0) {
                m->mat[act.fil][act.col] = FANTASMA;
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
    m->mat[act.fil - 1][act.col] = ENTRADA;
    m->mat[act.fil][act.col] = JUGADOR;
    jugador->posx = act.fil;
    jugador->posy = act.col;
    m->mat[filMod - 1][vecinos[r].col] = SALIDA ;
    m->mat[filMod - 2][vecinos[r].col] = FANTASMA ;
    m->posxS = filMod - 1;
    m->posyS = vecinos[r].col;
    m->exit = FALSE;
    vaciarPila(&p);
    return VERDADERO;
}

int buscarVecinos(char**mat, int fil, int col, Celdas*act, Celdas*vecinos) {
    int posx[] = { -2, 0, 2, 0 };
    int posy[] = { 0, 2, 0, -2 };
    int cant = 0;
    for (int i = 0; i < 4; i++) {
        if ((act->fil + posx[i]) > 0
                && (act->fil + posx[i]) < fil
                && (act->col + posy[i]) > 0
                && (act->col + posy[i]) < col
                && mat[act->fil + posx[i]][act->col + posy[i]] != VISITADO
                && mat[act->fil + posx[i]][act->col + posy[i]] != FANTASMA
                && mat[act->fil + posx[i]][act->col + posy[i]] != BONIFICACION) {
            vecinos[cant].fil = posx[i];
            vecinos[cant].col = posy[i];
            cant++;
        }
    }
    return cant;
}

void checkend(tMapa *m, player *p) {
    if (p->posx == m->posxS && p->posy == m->posyS)
        m->exit = VERDADERO;
}
int crearMapa(player *p, tMapa* m, int fant, int prem, int ext) {
    int fil, col;
    int cont = 0;
    m->exit = FALSE;
    m->filMapa = m->filMapa % 2 == 0 ? m->filMapa - 1 : m->filMapa;
    m->colMapa = m->colMapa % 2 == 0 ? m->colMapa - 1 : m->colMapa;
    m->mat = crearMatriz(m->filMapa, m->colMapa);
    if (!m->mat)
        return FALSE;
    llenarMat(m->mat, m->filMapa, m->colMapa);
    crearLaberinto(m, m->filMapa, m->colMapa, p, fant, prem, ext);
    return VERDADERO;
}

