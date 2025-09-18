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
//    for (int x = 0; x < fil; x++) {
//        for (int y = 0; y < col; y++) {
//            if (x != fil && y != col && x % 2 == 1 && y % 2 == 1)
//                strcpy(&mat[x][y], &cosa1);
//            else
//                strcpy(&mat[x][y], &cosa2);
//        }
//    }
    int x, y, i, j;
    for ( x = 0; x < fil; x++)
    {
        for ( y = 0; y < col; y++)
        {
                mat[x][y] = PARED;
        }
    }

    for ( x = ESPACIADO ; x < fil - ESPACIADO; x+= ESPACIADO + 1) {
        for ( y = ESPACIADO ; y < col - ESPACIADO; y += ESPACIADO + 1) {
            for (int i = 0; i < ESPACIADO; i++)
            {
                for (size_t j = 0; j < ESPACIADO; j++)
                {
                    mat[x+i][y+j] = CAMINO_FINAL;
                }

            }

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

//int buscarVecinos(char**mat,int fil, int col,Celdas*act,Celdas*vecinos)
//{
//    int posx[] = { -2, 0, 2, 0 };
//    int posy[] = { 0, 2, 0, -2 };
//    int cant = 0;
//    for (int i = 0; i < 4; i++) {
//        if((act->fil + posx[i])>0
//            && (act->fil + posx[i])<fil
//            && (act->col + posy[i])>0
//            && (act->col + posy[i])<col
//            && mat[act->fil+posx[i]][act->col+posy[i]] != VISITADO)
//        {
//            vecinos[cant].fil = posx[i];
//            vecinos[cant].col = posy[i];
//            cant++;
//        }
//    }
//    return cant;
//}
int crearLaberinto(struct mapaL *m, int fil, int col,struct player *pla) {
//    int cantVecinos;
//    int r;
//    tPila p;
//    Celdas act = {1,1};
//    Celdas vecinos[4];
//    Celdas pared;
//    srand(time(NULL));
//    crearPila(&p);
//    apilar(&p, &act, sizeof(Celdas));
//    while (pilaVacia(&p) == TODO_OK) {
//        desapilar(&p, &act, sizeof(Celdas));
//        m->mat[act.fil][act.col] = VISITADO;
//        cantVecinos = buscarVecinos(m->mat, fil, col, &act, vecinos);
//        if (cantVecinos) {
//            apilar(&p, &act, sizeof(Celdas));
//            r = rand() % cantVecinos;
//            pared.fil = act.fil + vecinos[r].fil / 2;
//            pared.col = act.col + vecinos[r].col / 2;
//            m->mat[pared.fil][pared.col] = CAMINO;
//            m->mat[pared.fil][pared.col] = CAMINO;
//            vecinos[r].col +=act.col;
//            vecinos[r].fil +=act.fil;
//            apilar(&p, &vecinos[r], sizeof(Celdas));
//        }
//    }
    int cantVecinos;
    int r;
    tPila p;
    Celdas act = {1,1};
    Celdas vecinos[4];
    Celdas pared;
    srand(time(NULL));
    crearPila(&p);
    apilar(&p, &act, sizeof(Celdas));
    while (pilaVacia(&p) == TODO_OK) {
        desapilar(&p, &act, sizeof(Celdas));
        m->mat[act.fil][act.col] = VISITADO;
        cantVecinos = buscarVecinos(m->mat, fil, col, &act, vecinos);
        if (cantVecinos) {
            apilar(&p, &act, sizeof(Celdas));
            r = rand() % cantVecinos;
            pared.fil = act.fil + vecinos[r].fil / 2;
            pared.col = act.col + vecinos[r].col / 2;
            m->mat[pared.fil][pared.col] = CAMINO;
            m->mat[pared.fil][pared.col] = CAMINO;
            vecinos[r].col +=act.col;
            vecinos[r].fil +=act.fil;
            apilar(&p, &vecinos[r], sizeof(Celdas));
        }
    }

    for (int i = 0; i < fil; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (m->mat[i][j] == VISITADO || m->mat[i][j] == CAMINO)
            m->mat[i][j] = CAMINO_FINAL;
        }
    }

    act.col = 1;
    act.fil = 1;
    apilar(&p, &act, sizeof(Celdas));
    while (pilaVacia(&p) == TODO_OK) {
        desapilar(&p, &act, sizeof(Celdas));
        m->mat[act.fil][act.col] = VISITADO;
        cantVecinos = buscarVecinos(m->mat, fil, col, &act, vecinos);
        if (cantVecinos) {
            apilar(&p, &act, sizeof(Celdas));
            r = rand() % cantVecinos;
            pared.fil = act.fil + vecinos[r].fil / 2;
            pared.col = act.col + vecinos[r].col / 2;
            m->mat[pared.fil][pared.col] = CAMINO;
            m->mat[pared.fil][pared.col] = CAMINO;
            vecinos[r].col +=act.col;
            vecinos[r].fil +=act.fil;
            apilar(&p, &vecinos[r], sizeof(Celdas));
        }
    }

    for (int i = 0; i < fil; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (m->mat[i][j] == VISITADO || m->mat[i][j] == CAMINO)
            m->mat[i][j] = CAMINO_FINAL;
        }
    }

    m->mat[act.fil-1][act.col] = ENTRADA;
    m->mat[act.fil][act.col] = JUGADOR;
    pla->posx=act.fil;
    pla->posy=act.col;
    m->mat[fil-1][vecinos[r].col]= SALIDA ;
    m->posxS=fil- 1;
    m->posyS=vecinos[r].col;
    m->exit=false;
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

void checkend(struct mapaL *m,struct player *p)
{
    if(p->posx == m->posxS && p->posy == m->posyS)
    {
        m->exit=true;
    }
}


