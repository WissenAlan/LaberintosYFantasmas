#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED
#include "main.h"
struct player
{
    unsigned posx;
    unsigned posy;
    unsigned last_x;
    unsigned last_y;
};
void moverjugador(char**mat,char movimiento,struct player *p);
void move_up(char**mat,struct player * p);
void move_down(char**mat,struct player * p);
void move_left(char**mat,struct player * p);
void move_right(char**mat,struct player * p);
void ai(char **mat,struct player *p);
int state(char **mat,int trypos,struct player *p);
#endif // JUGADOR_H_INCLUDED
