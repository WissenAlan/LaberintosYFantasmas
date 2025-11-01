#include "header/jugador.h"
#include "header/SDL_INIT.h"
void crearJugador(tJugador *p, int cantVidas)
{
    p->puntos = 0;
    p->vidas = cantVidas;
    p->posx = 0;
    p->posy = 0;
    p->roundBuff=0;
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
    if(pJug->roundBuff > 0)
    {
        pJug->roundBuff--;
    }
}

void ai(tCola *colamov,tMapa *mapa, tJugador *pJug, tFantasma *pFant)
{
    int try_w = state(mapa, ARRIBA, pJug, pFant);
    int try_s = state(mapa, ABAJO, pJug, pFant);
    int try_a = state(mapa, IZQUIERDA, pJug, pFant);
    int try_d = state(mapa, DERECHA, pJug, pFant);
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
int state(tMapa *mapa, int trypos,tJugador *pJug, const tFantasma *pFant)
{
    int reward = 0;
    int try_x = pFant->posx;
    int try_y = pFant->posy;
    int diff_x, diff_y, try_diff_x, try_diff_y;
    srand(time(NULL));
    trypos == ARRIBA ? try_x-- : (trypos == ABAJO ? try_x++ : (trypos == IZQUIERDA ? try_y-- : try_y++));
    if (mapa->mat[try_x][try_y] == PARED || mapa->mat[try_x][try_y] == ENTRADA || mapa->mat[try_x][try_y] == SALIDA || mapa->mat[try_x][try_y] == BONIFICACION)
        reward+= -1000;
    //if para que detecte algo lo modificamos para que detecte a el jugador;
    if (mapa->mat[try_x][try_y] == JUGADOR)
        reward+= 100;

    printf("%d",pJug->roundBuff);
    if(pJug->roundBuff > 0)
    {
        int auxcolumna=mapa->colMapa-1,auxfila=mapa->filMapa-1;
        CoordenadasesquinaMasLejosJugador(pJug,&auxfila,&auxcolumna);
        diff_x = abs(pFant->posx - (auxfila));
        diff_y = abs(pFant->posy - (auxcolumna));
        try_diff_x = abs(try_x - auxfila);
        try_diff_y = abs(try_y - auxcolumna);
        if (mapa->mat[try_x][try_y] == JUGADOR)
        reward-= 500;
    }
    else
    {
        if(rand() % 3 == 2)
        {
            diff_x = abs(pFant->posx - (pJug->posx+4));
            diff_y = abs(pFant->posy - (pJug->posy+4));
            try_diff_x = abs(try_x - pJug->posx);
            try_diff_y = abs(try_y - pJug->posy);


        }
        else
        {
            diff_x = abs(pFant->posx - pJug->posx);
            diff_y = abs(pFant->posy - pJug->posy);
            try_diff_x = abs(try_x - pJug->posx);
            try_diff_y = abs(try_y - pJug->posy);
        }
    }
    if (mapa->mat[try_x][try_y] == JUGADOR)
        reward+= 100;
    if (try_diff_x < diff_x)
        reward += 50;
    if (try_diff_y < diff_y)
        reward += 50;
    // Penalización leve por alejarse
    if (try_diff_x > diff_x)
        reward -= 20;
    if (try_diff_y > diff_y)
        reward -= 20;
    return reward;
}
void CoordenadasesquinaMasLejosJugador(tJugador *jugador,int* filas,int* columnas)
{
    int esquina1 = abs(jugador->posx);
    int esquina2 = abs(jugador->posy - *columnas);
    int esquina3 = abs(jugador->posx - *filas);
    int esquina4 = abs(jugador->posy);
    if (esquina1 >= esquina2 && esquina1 >= esquina3 && esquina1 >= esquina4)
    {
        *filas=esquina1;
        *columnas=0;
    }
    else if (esquina2 >= esquina3 && esquina2 >= esquina4)
    {
        *filas=0;
        *columnas=esquina2;
    }
    else if (esquina3 >= esquina4)
    {
        *filas=esquina3;
    }
    else
    {
        *columnas=0;
        *filas=esquina4;
    }
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
