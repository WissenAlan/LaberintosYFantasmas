#include "header/jugador.h"
#include "header/SDL_INIT.h"
void crearJugador(tJugador *p, int cantVidas)
{
    p->puntos = 0;
    p->vidas = cantVidas;
    p->posx = 0;
    p->posy = 0;
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
    switch (tecla)
    {
    case SDL_SCANCODE_W:
        if (mat[pJug->posx - 1][pJug->posy] != PARED && mat[pJug->posx - 1][pJug->posy] != ENTRADA)
            pJug->posx--;
        break;
    case SDL_SCANCODE_S:
        if (mat[pJug->posx + 1][pJug->posy] != PARED)
            pJug->posx++;
        break;
    case SDL_SCANCODE_A:
        if (mat[pJug->posx][pJug->posy - 1] != PARED)
            pJug->posy--;
        break;
    case SDL_SCANCODE_D:
        if (mat[pJug->posx][pJug->posy + 1] != PARED)
            pJug->posy++;
        break;
    default:
        seMovio = FALSE;
        break;
    }
    if (seMovio == VERDADERO)
    {
        movimiento.posx = pJug->posx;
        movimiento.posy = pJug->posy;
        movimiento.movimiento = tecla;
        colaInsertar(colamov, &movimiento, sizeof(tMovimiento));
    }
}

void ai(tCola *colamov, char **mat, tJugador *pJug, tFantasma *pFant)
{
    int try_w = state(mat, ARRIBA, pJug, pFant);
    int try_s = state(mat, ABAJO, pJug, pFant);
    int try_a = state(mat, IZQUIERDA, pJug, pFant);
    int try_d = state(mat, DERECHA, pJug, pFant);
    tMovimiento movimiento;
    int lado;
    if (try_w >= try_s && try_w >= try_a && try_w >= try_d)
    {
        pFant->posx--;
        lado = SDL_SCANCODE_W;
    }
    else if (try_s >= try_a && try_s >= try_d)
    {
        pFant->posx++;
        lado = SDL_SCANCODE_S;
    }
    else if (try_a >= try_d)
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
int abs(int numero)
{
    if (numero > 0)
        return numero;
    else
        return -numero;
}
int state(char **mat, int trypos, const tJugador *pJug, const tFantasma *pFant)
{
    int reward = 0;
    int try_x = pFant->posx;
    int try_y = pFant->posy;
    int diff_x, diff_y, try_diff_x, try_diff_y;
    trypos == ARRIBA ? try_x-- : (trypos == ABAJO ? try_x++ : (trypos == IZQUIERDA ? try_y-- : try_y++));
    if (mat[try_x][try_y] == PARED || mat[try_x][try_y] == ENTRADA || mat[try_x][try_y] == SALIDA || mat[try_x][try_y] == BONIFICACION)
        return -1000;
    //if para que detecte algo lo modificamos para que detecte a el jugador;
    if (mat[try_x][try_y] == JUGADOR)
        return 1000;
    diff_x = abs(pFant->posx - pJug->posx);
    diff_y = abs(pFant->posy - pJug->posy);
    try_diff_x = abs(try_x - pJug->posx);
    try_diff_y = abs(try_y - pJug->posy);
    // Recompensa por acercarse al jugador
    if (try_diff_x < diff_x)
        reward += 5;
    if (try_diff_y < diff_y)
        reward += 5;
    // Penalización leve por alejarse
    if (try_diff_x > diff_x)
        reward -= 2;
    if (try_diff_y > diff_y)
        reward -= 2;
    return reward;
}
int contarMovs(tCola* colaMovsJugador)
{
    tMovimiento tMov;
    int contador;
    while (colaVacia(colaMovsJugador) != COLA_VACIA)
    {
        contador++;
        sacarDeCola(colaMovsJugador, &tMov, sizeof(tMov));
    }
    return contador;
}
