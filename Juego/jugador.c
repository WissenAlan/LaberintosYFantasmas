#include "header/jugador.h"
#include "header/SDL_INIT.h"
void crearJugador(tJugador *p, int cantVidas)
{
    p->puntos = 0;
    p->vidas = cantVidas;
    p->posx = 0;
    p->posy = 0;
    p->roundBuff = 0;
}
unsigned getVidasJugador(tJugador *pJug)
{
    return pJug->vidas;
}
unsigned getPuntosJugador(tJugador *pJug)
{
    return pJug->puntos;
}
void moverJugador(tCola *colamov, char **mat, tJugador *pJug, int tecla)
{
    tMovimiento movimiento;
    int seMovio = VERDADERO;
    movimiento.entidad = JUGADOR;
    switch(tecla)
    {
    case SDL_SCANCODE_W:
        if(mat[pJug->posx - 1][pJug->posy] != PARED && mat[pJug->posx - 1][pJug->posy] != ENTRADA)
            pJug->posx--;
        break;
    case SDL_SCANCODE_S:
        if(mat[pJug->posx + 1][pJug->posy] != PARED)
            pJug->posx++;
        break;
    case SDL_SCANCODE_A:
        if(mat[pJug->posx][pJug->posy - 1] != PARED)
            pJug->posy--;
        break;
    case SDL_SCANCODE_D:
        if(mat[pJug->posx][pJug->posy + 1] != PARED)
            pJug->posy++;
        break;
    default:
        seMovio = INCORRECTO;
        break;
    }
    if(seMovio == VERDADERO)
    {
        movimiento.posx = pJug->posx;
        movimiento.posy = pJug->posy;
        movimiento.movimiento = tecla;
        colaInsertar(colamov, &movimiento, sizeof(tMovimiento));
    }
    if(pJug->roundBuff > 0)
        pJug->roundBuff--;
}
void ai(tCola *colamov, tMapa *mapa, tJugador *pJug, tFantasma *pFant)
{
    int try_w = state(mapa, ARRIBA, pJug, pFant);
    int try_s = state(mapa, ABAJO, pJug, pFant);
    int try_a = state(mapa, IZQUIERDA, pJug, pFant);
    int try_d = state(mapa, DERECHA, pJug, pFant);
    int lado;
    tMovimiento movimiento;
    if(try_w >= try_s && try_w >= try_a && try_w >= try_d)
    {
        pFant->posx--;
        lado = SDL_SCANCODE_W;
    }
    else if(try_s >= try_a && try_s >= try_d)
    {
        pFant->posx++;
        lado = SDL_SCANCODE_S;
    }
    else if(try_a >= try_d)
    {
        pFant->posy--;
        lado = SDL_SCANCODE_A;
    }
    else
    {
        pFant->posy++;
        lado = SDL_SCANCODE_D;
    }
    movimiento.entidad = FANTASMA;
    movimiento.posx = pFant->posx;
    movimiento.posy = pFant->posy;
    movimiento.movimiento = lado;
    colaInsertar(colamov, &movimiento, sizeof(movimiento));
}
int state(tMapa *mapa, int trypos, tJugador *pJug, const tFantasma *pFant)
{
    int reward = 0, auxCol;
    int try_x = pFant->posx;
    int try_y = pFant->posy;
    int diff_x, diff_y, try_diff_x, try_diff_y;
    srand(time(NULL));
    trypos == ARRIBA ? try_x-- : (trypos == ABAJO ? try_x++ : (trypos == IZQUIERDA ? try_y-- : try_y++));
    if(mapa->mat[try_x][try_y] == PARED || mapa->mat[try_x][try_y] == ENTRADA || mapa->mat[try_x][try_y] == SALIDA || mapa->mat[try_x][try_y] == BONIFICACION)
        return -1000;
    //if para que detecte algo lo modificamos para que detecte a el jugador;
    if(mapa->mat[try_x][try_y] == JUGADOR)
        reward += 100;
    if(pJug->roundBuff > 0)
    {
        auxCol = mapa->colMapa - 1, auxfila = mapa->filMapa - 1;
        coordenadaEsquinaMasLejana(pJug, auxfila, auxCol, &auxfila, &auxCol);
        diff_x = ABS(pFant->posx - (auxfila));
        diff_y = ABS(pFant->posy - (auxCol));
        try_diff_x = ABS(try_x - auxfila);
        try_diff_y = ABS(try_y - auxCol);
        if(mapa->mat[try_x][try_y] == JUGADOR)
            reward -= 500;
    }
    else
    {
        if(rand() % 5 == 2)
        {
            diff_x = ABS(pFant->posx - (pJug->posx + 4));
            diff_y = ABS(pFant->posy - (pJug->posy + 4));
            try_diff_x = ABS(try_x - (pJug->posx + 4));
            try_diff_y = ABS(try_y - (pJug->posy + 4));
        }
        else
        {
            diff_x = ABS(pFant->posx - pJug->posx);
            diff_y = ABS(pFant->posy - pJug->posy);
            try_diff_x = ABS(try_x - pJug->posx);
            try_diff_y = ABS(try_y - pJug->posy);
        }
    }
    if(try_diff_x < diff_x)
        reward += 50;
    if(try_diff_y < diff_y)
        reward += 50;
    // Penalización leve por alejarse
    if(try_diff_x > diff_x)
        reward -= 20;
    if(try_diff_y > diff_y)
        reward -= 20;
    return reward;
}
void coordenadaEsquinaMasLejana(tJugador *jugador, int max_filas, int max_columnas, int* esquina_fila_out, int* esquina_columna_out)
{
    double centro_fila = (max_filas - 1) / 2.0;
    double centro_columna = (max_columnas - 1) / 2.0;
    if(jugador->posx < centro_fila)
        *esquina_fila_out = max_filas - 1;
    else
        *esquina_fila_out = 0;
    if(jugador->posy < centro_columna)
        *esquina_columna_out = max_columnas - 1;
    else
        *esquina_columna_out = 0;
}
int contarMovs(tCola* colaMovsJugador)
{
    tMovimiento tMov;
    int contador;
    while(colaVacia(colaMovsJugador) != COLA_VACIA)
    {
        contador++;
        sacarDeCola(colaMovsJugador, &tMov, sizeof(tMov));
    }
    return contador;
}
