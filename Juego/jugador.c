#include "header/jugador.h"
#include "header/SDL_INIT.h"
void crearJugador(tJugador *p, int cantVidas) {
    p->puntos = 0;
    p->vidas = cantVidas;
    p->posx = 0;
    p->posy = 0;
}
unsigned getVidasJugador(tJugador *pJug){
    return pJug->vidas;
}
unsigned getPuntosJugador(tJugador *pJug){
    return pJug->puntos;
}
void moverJugador(tCola *colamov, char **mat, tJugador *pJug, int tecla) {
    tMovimiento tmov;
    tmov.posx = pJug->posx;
    tmov.posy = pJug->posy;
    tmov.entidad = JUGADOR;
        switch (tecla) {
        case SDL_SCANCODE_W:
            if (mat[pJug->posx - 1][pJug->posy] != PARED && mat[pJug->posx - 1][pJug->posy] != ENTRADA && mat[pJug->posx - 1][pJug->posy] != FANTASMA) {
                pJug->posx--;
                tmov.posx--;
                encolarMov(colamov, &tmov, ARRIBA);
            }
            break;
        case SDL_SCANCODE_S:
            if (mat[pJug->posx + 1][pJug->posy] != PARED && mat[pJug->posx + 1][pJug->posy] != FANTASMA) {
                pJug->posx++;
                tmov.posx++;
                encolarMov(colamov, &tmov, ABAJO);
            }
            break;
        case SDL_SCANCODE_A:
            if (mat[pJug->posx][pJug->posy - 1] != PARED && mat[pJug->posx][pJug->posy-1] != FANTASMA) {
                tmov.posy--;
                pJug->posy--;
                encolarMov(colamov, &tmov, IZQUIERDA);
            }
            break;
        case SDL_SCANCODE_D:
            if (mat[pJug->posx][pJug->posy + 1] != PARED && mat[pJug->posx][pJug->posy + 1] != FANTASMA) {
                tmov.posy++;
                pJug->posy++;
                encolarMov(colamov, &tmov, DERECHA);
            }
            break;
        default:
            break;
        }
}

void encolarMov(tCola *cola, tMovimiento *pmov, int mov) {
    pmov->movimiento = mov;
    colaInsertar(cola, pmov, sizeof(tMovimiento));
}

void ai(tCola *colamov, char **mat, tJugador *pJug, tFantasma *pFant) {
    int try_w = state(mat, 1, pJug, pFant);
    int try_s = state(mat, 2, pJug, pFant);
    int try_a = state(mat, 3, pJug, pFant);
    int try_d = state(mat, 4, pJug, pFant);
    tMovimiento movimiento;
    int lado;
    if (try_w >= try_s && try_w >= try_a && try_w >= try_d) {
        pFant->posx--;
        lado = ARRIBA;
    } else if (try_s >= try_a && try_s >= try_d) {
        pFant->posx++;
        lado = ABAJO;
    } else if (try_a > try_d) {
        pFant->posy--;
        lado = IZQUIERDA;
    } else {
        pFant->posy++;
        lado = DERECHA;
    }
    movimiento.entidad = FANTASMA;
    movimiento.posx = pFant->posx;
    movimiento.posy = pFant->posy;
    encolarMov(colamov, &movimiento, lado);
}
int abs(int numero) {
    if (numero > 0)
        return numero;
    else
        return -numero;
}
int state(char **mat, int trypos, const tJugador *pJug, const tFantasma *pFant) {
    int reward = 0;
    int try_x = pFant->posx;
    int try_y = pFant->posy;
    int diff_x, diff_y, try_diff_x, try_diff_y;
    switch (trypos) {
    case 1:
        try_x--;
        break;
    case 2:
        try_x++;
        break;
    case 3:
        try_y--;
        break;
    case 4:
        try_y++;
        break;
    default:
        break;
    }
    if (mat[try_x][try_y] == PARED || mat[try_x][try_y] == ENTRADA || mat[try_x][try_y] == SALIDA)
        reward += -1000;
    //if para que detecte algo lo modificamos para que detecte a el jugador;
    if (mat[try_x][try_y] == 'P')
        reward += 1000;
    diff_x = abs(pFant->posx - pJug->posx);
    diff_y = abs(pFant->posy - pJug->posy);
    try_diff_x = abs(try_x - pJug->posx);
    try_diff_y = abs(try_y - pJug->posy);
    if (try_diff_x < diff_x)
        reward += 5;
    if (try_diff_y < diff_y)
        reward += 5;
    return reward;
}

