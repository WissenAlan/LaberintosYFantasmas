#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED
#include "estructuras/colaDin.h"

#define ARRIBA 1
#define ABAJO 2
#define IZQUIERDA 3
#define DERECHA 4
typedef struct {
    int posx;
    int posy;
    unsigned puntos;
    unsigned lifes;
} player;
typedef struct {
    int posx;
    int posy;
    int estado;
} ghost;
typedef struct {
    int posx;
    int posy;
    int move;
} moves;
void moverjugador(tCola *colamov, char** mat, char movimiento, player* p);
void move_up(char**mat, player * p);
void move_down(char**mat, player * p);
void move_left(char**mat, player * p);
void move_right(char**mat, player * p);
int abs(int numero);
void ai(tCola *colamov, char **mat, player *p, ghost *f);
int state(char **mat, int trypos, player *p, ghost *f);

void desencolarmovs(tCola *cola, char ** mat, player *p);
void encolarMov(tCola *cola, moves *movimiento, int mov);
#endif // JUGADOR_H_INCLUDED
