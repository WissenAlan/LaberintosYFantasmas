#include "header/mapa.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estructuras/pilaDin.h"
int crearMapa(tMapa *m)
{
    m->exit = INCORRECTO;
    m->posxS = m->posyS = m->colMapa = m->filMapa = 0;
    m->mat = NULL;
}
char**crearMatriz(int filas, int columnas)
{
    int i, j;
    char**mat = (char**)malloc(filas * sizeof(char*));
    if(!mat)
        return NULL;
    for(i = 0; i < filas; i++)
    {
        mat[i] = (char*) malloc(columnas * sizeof(char));
        if(mat[i] == NULL)
        {
            for(j = 0; j < i; j++)
                free(mat[j]);
            free(mat);
            return NULL;
        }
    }
    return mat;
}

void eliminarMatriz(char**mat, int filas, int columnas)
{
    int i;
    for(i = 0; i < filas; i++)
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

void llenarMat(char** mat, int fil, int col)
{
    int x, y, i;
    for(x = 0; x < fil; x++)
    {
        memset(mat[x], PARED, col);  /// Llenar fila completa de una vez
    }
    for(x = ESPACIADO; x < fil - ESPACIADO; x += ESPACIADO + 1)
    {
        for(y = ESPACIADO; y < col - ESPACIADO; y += ESPACIADO + 1)
        {
            for(i = 0; i < ESPACIADO; i++)
                memset(&mat[x + i][y], CELDA, ESPACIADO);
        }
    }
}

void mostrarMat(char**mat, int fil, int col)
{
    int x;
    for(x = 0; x < fil; x++)
        printf("     %s\n", mat[x]);
}

void generarEstructuraLaberinto(tMapa *m, int filMod, int colMod)
{
    int cantVecinos, r;
    tPila p;
    Celdas act = {1, 1}; // Celda inicial
    Celdas vecinos[4], pared;
    crearPila(&p);
    apilar(&p, &act, sizeof(Celdas));
    while(pilaVacia(&p) == TODO_OK)
    {
        desapilar(&p, &act, sizeof(Celdas));
        m->mat[act.fil][act.col] = VISITADO;
        cantVecinos = buscarVecinos(m->mat, filMod, colMod, &act, vecinos);
        if(cantVecinos > 0)
        {
            apilar(&p, &act, sizeof(Celdas)); // La volvemos a apilar
            r = rand() % cantVecinos;
            // Romper la pared
            pared.fil = act.fil + vecinos[r].fil / 2;
            pared.col = act.col + vecinos[r].col / 2;
            m->mat[pared.fil][pared.col] = CAMINO;
            // Moverse a la siguiente celda
            vecinos[r].col += act.col;
            vecinos[r].fil += act.fil;
            apilar(&p, &vecinos[r], sizeof(Celdas));
        }
    }
    vaciarPila(&p);
}
int recolectarSpotsLibres(tMapa *m, int filMod, int colMod, Celdas spotsDisponibles[], int safeDist)
{
    int cantSpots = 0, i, j, distAlInicio;
    for(i = 0; i < filMod; i++)
    {
        for(j = 0; j < colMod; j++)
        {
            if(m->mat[i][j] == VISITADO || m->mat[i][j] == CAMINO)
            {
                distAlInicio = (i - 1) + (j - 1); // Distancia desde (1,1)
                // Si está fuera de la zona segura, es un spot válido
                if(distAlInicio > safeDist)
                {
                    spotsDisponibles[cantSpots].fil = i;
                    spotsDisponibles[cantSpots].col = j;
                    cantSpots++;
                }
            }
        }
    }
    return cantSpots;
}
void barajarSpots(Celdas spotsDisponibles[], int cantSpots)
{
    Celdas tempSwap;
    int i, j;
    for(i = cantSpots - 1; i > 0; i--)
    {
        j = rand() % (i + 1);
        // Swap spotsDisponibles[i] con spotsDisponibles[j]
        tempSwap = spotsDisponibles[i];
        spotsDisponibles[i] = spotsDisponibles[j];
        spotsDisponibles[j] = tempSwap;
    }
}
void colocarItems(tMapa *m, Celdas spotsDisponibles[], int cantSpots, int prem, int ext, int fant)
{
    int spotIndex = 0, i;
    Celdas act;
    // Colocar Vidas
    for(i = 0; i < ext && spotIndex < cantSpots; i++, spotIndex++)
    {
        act = spotsDisponibles[spotIndex];
        m->mat[act.fil][act.col] = VIDAEXT; // 'v'
    }
    // Colocar Premios
    for(i = 0; i < prem && spotIndex < cantSpots; i++, spotIndex++)
    {
        act = spotsDisponibles[spotIndex];
        m->mat[act.fil][act.col] = BONIFICACION;
    }
    // Colocar Fantasmas
    for(i = 0; i < fant && spotIndex < cantSpots; i++, spotIndex++)
    {
        act = spotsDisponibles[spotIndex];
        m->mat[act.fil][act.col] = FANTASMA;
        // ubicar en vector de fantasmas!
    }
}
void limpiarMapa(tMapa *m, int filMod, int colMod)
{
    int i, j;
    for(i = 0; i < filMod; i++)
    {
        for(j = 0; j < colMod; j++)
        {
            if(m->mat[i][j] == VISITADO || m->mat[i][j] == CAMINO)
                m->mat[i][j] = CELDA;
        }
    }
}
void colocarEntradaSalida(tMapa *m, int filMod, int colMod, tJugador *pJug)
{
    // Colocar ENTRADA y JUGADOR en (1,1)
    int colSalida, posiblesSalidas[colMod], cantPosibles = 0, j;
    m->mat[0][1] = ENTRADA; // (0, 1)
    m->mat[1][1] = JUGADOR; // (1, 1)
    pJug->posx = 1;
    pJug->posy = 1;
    // Colocar SALIDA y FANTASMA GUARDIÁN
    for(j = 0; j < colMod; j++)
    {
        if(m->mat[filMod - 2][j] == CELDA)    // Busca camino en la penúltima fila
        {
            posiblesSalidas[cantPosibles] = j;
            cantPosibles++;
        }
    }
    if(cantPosibles > 0)
        colSalida = posiblesSalidas[rand() % cantPosibles];
    else
        colSalida = (colMod / 2) | 1;
    m->mat[filMod - 1][colSalida] = SALIDA;
    m->mat[filMod - 2][colSalida] = FANTASMA;
    // Guardar posición de la salida en el mapa
    m->posxS = filMod - 1;
    m->posyS = colSalida;
}
int crearLaberinto(tMapa *m, int filMod, int colMod, tJugador *pJug, int fant, int prem, int ext)
{
    int cantFan = fant - 1; // 1 fantasma se reserva para la salida
    int safeDist = (filMod + colMod) / 8;
    int cantSpots = 0;
    // 1. Inicialización
    srand(time(NULL));
    if(safeDist < 4) safeDist = 4;
    // Arreglo para guardar celdas libres (spots)
    Celdas spotsDisponibles[filMod * colMod];
    // 2. Generar Estructura del Laberinto (DFS)
    generarEstructuraLaberinto(m, filMod, colMod);
    // 3. Recolectar celdas libres (fuera de la zona segura)
    cantSpots = recolectarSpotsLibres(m, filMod, colMod, spotsDisponibles, safeDist);
    // 4. Barajar las celdas libres
    if(cantSpots > 0)
        barajarSpots(spotsDisponibles, cantSpots);
    // 5. Colocar ítems (premios, vidas, fantasmas)
    colocarItems(m, spotsDisponibles, cantSpots, prem, ext, cantFan);
    // 6. Limpiar el mapa (VISITADO -> CELDA)
    limpiarMapa(m, filMod, colMod);
    // 7. Colocar Entradas, Salidas y Jugador
    colocarEntradaSalida(m, filMod, colMod, pJug);
    // 8. Inicializar estado del juego y guardar
    m->jugadorMuerto = INCORRECTO;
    m->exit = INCORRECTO;
    pJug->roundBuff = 0;
    guardarMapaEnArchivo(*m, m->filMapa, m->colMapa, "Laberinto.txt");
    return VERDADERO;
}
void guardarMapaEnArchivo(tMapa m, int filMod, int colMod, char* nombreArchivo)
{
    int i, j;
    FILE*f = fopen(nombreArchivo, "w");
    if(f == NULL)
    {
        perror("Error al abrir el archivo del mapa");
        return;
    }
    for(i = 0; i < filMod; i++)
    {
        for(j = 0; j < colMod; j++)
        {
            char c = '?';
            switch(m.mat[i][j])
            {
            case FANTASMA:
                c = 'F';
                break;
            case CELDA:
                c = '.';
                break;
            case BONIFICACION:
                c = 'P';
                break;
            case ENTRADA:
                c = 'E';
                break;
            case SALIDA:
                c = 'S';
                break;
            case PARED:
                c = '#';
                break;
            case JUGADOR:
                c = '.';
                break;
            case VIDAEXT:
                c = 'V';
                break;
            default:
                break;
            }
            fprintf(f, "%c", c);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}
int buscarVecinos(char**mat, int fil, int col, Celdas*act, Celdas*vecinos)
{
    int posx[] = { -2, 0, 2, 0 }; /// Posiciones validas que se pueden dar    ##############
    int posy[] = { 0, 2, 0, -2 }; /// solo se pueden ver considerar aquellas  ##v ##ac##  ##
    /// que no se encuentren visitadas y sean   ##############
    /// nodos validos                           ##  ##v ##  ##
    ///                                         ##############
    ///
    /// pd: el algoritmo esta modificado para que rompa mas de lo que
    /// deberia para poder generar mas espacio, de lo contrario solo habria
    /// un camino y seria mas complejo
    int cant = 0, i;
    for(i = 0; i < 4; i++)    /// en esta condicion bastante larga revisamos:
    {
        if((act->fil + posx[i]) > 0  ///<------ que no se salga de la matriz
                && (act->fil + posx[i]) < fil
                && (act->col + posy[i]) > 0
                && (act->col + posy[i]) < col
                && mat[act->fil + posx[i]][act->col + posy[i]] != VISITADO ///<------ y que no este:visitado
                && mat[act->fil + posx[i]][act->col + posy[i]] != FANTASMA/// NO sea un fntasma
                && mat[act->fil + posx[i]][act->col + posy[i]] != BONIFICACION)    /// NI sea una bonificacion
        {
            vecinos[cant].fil = posx[i];
            vecinos[cant].col = posy[i];
            cant++;
        }
    }
    return cant;
}
void checkend(tMapa *m, tJugador *pJug)
{
    if(pJug->posx == m->posxS && pJug->posy == m->posyS)
        m->exit = VERDADERO;
}
void checklifes(tMapa *m, tJugador *pJug)
{
    if(pJug->vidas == 0)
        m->jugadorMuerto = VERDADERO;
}
int llenarMapa(tJugador *p, tMapa* m, int fant, int prem, int ext)
{
    int fil, col, cont = 0;
    m->exit = INCORRECTO;
    m->filMapa = m->filMapa % 2 == 0 ? m->filMapa - 1 : m->filMapa;  /// truncamos si es par, ya que el algoritmo utilizadi
    m->colMapa = m->colMapa % 2 == 0 ? m->colMapa - 1 : m->colMapa;  /// unicamente funciona si se utiliza un tamaño impar :(
    m->mat = crearMatriz(m->filMapa, m->colMapa);
    if(!m->mat)
        return INCORRECTO;
    llenarMat(m->mat, m->filMapa, m->colMapa);
    crearLaberinto(m, m->filMapa, m->colMapa, p, fant, prem, ext);
    return VERDADERO;
}

