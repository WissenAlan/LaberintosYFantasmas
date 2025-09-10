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
        mat[i] =(char*) malloc(columnas * sizeof(char));
        if (mat[i] == NULL) {
            for (int j = 0; j < i; j++)
                free(mat[j]);
            free(mat);
            return NULL;
        }
    }
    return mat;
}

void eliminarMatriz(char**mat,int filas,int columnas)
{
    for (int i = 0; i < filas; i++)
        free(mat[i]);
    free(mat);
}

void llenarMat(char**mat, int fil, int col) {
    char cosa1 = ' ';
    char cosa2 = '#';
    for (int x = 0; x < fil; x++) {
        for (int y = 0; y < col; y++) {
            if (x != fil && y != col && x % 2 == 1 && y % 2 == 1)
                strcpy(&mat[x][y], &cosa1);
            else
                strcpy(&mat[x][y], &cosa2);
        }
    }
}

void mostrarMat(char**mat, int fil, int col) {
    for (int x = 0; x < fil; x++) {
        printf("     ");
        for (int y = 0; y < col; y++)
            printf("%c", mat[x][y]);
        printf("\n");
    }
}

int crearLaberinto(char**mat, int fil, int col) {
    int cantVecinos;
    tPila p;
    Celdas act = {1,1};
    Celdas vecinos[4];
    Celdas pared;
    srand(time(NULL));
    mat[1][1] = VISITADO;
    crearPila(&p);
    apilar(&p, &act, sizeof(Celdas));
    while (pilaVacia(&p) == TODO_OK) {
        desapilar(&p, &act, sizeof(Celdas));
        cantVecinos = buscarVecinos(mat, fil, col, &act, vecinos);
        if (cantVecinos) {
            apilar(&p, &act, sizeof(Celdas));
            int r = rand() % cantVecinos;
            pared.fil = act.fil + vecinos[r].fil / 2;
            pared.col = act.col + vecinos[r].col / 2;
            mat[pared.fil][pared.col] = CAMINO;
            apilar(&p, &vecinos[r], sizeof(Celdas));
        }
    }
    vaciarPila(&p);
    return 0;
}

int buscarVecinos(char**mat,int fil, int col,Celdas*act,Celdas*vecinos)
{
    int posx[] = { -2, 0, 2, 0 };
    int posy[] = { 0, 2, 0, -2 };
    int cant = 0;
    for (int i = 0; i < 4; i++) {
        if((act->fil + posx[i])>0
            && (act->fil + posx[i])<fil
            && (act->col + posy[i])>0
            && (act->col + posy[i])<col
            && mat[act->fil+posx[i]][act->col+posy[i]] != VISITADO)
        {
            vecinos[cant].fil = posx[i];
            vecinos[cant].col = posy[i];
            cant++;
        }
    }
    return cant;
}
