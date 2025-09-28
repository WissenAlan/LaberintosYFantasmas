#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED
#include "main.h"
#include "mapa.h"
#include "game.h"
#include "estructuras/colaDin.h"
struct player
{
    int posx;
    int posy;
    unsigned puntos;
    unsigned lifes;
};
struct ghost
{
    int posx;
    int posy;
    bool estado;
};
struct moves
{
    int posx;
    int posy;
    int move;
};
void moverjugador(tCola *colamov,char** mat,char movimiento ,struct player* p);
void move_up(char**mat,struct player * p);
void move_down(char**mat,struct player * p);
void move_left(char**mat,struct player * p);
void move_right(char**mat,struct player * p);
int abs(int numero);
void ai(tCola *colamov,char **mat,struct player *player,struct ghost *p);
int state(char **mat,int trypos,struct player *p,struct ghost *f);
void moverfantasmas(struct game *g,tCola *cola);
void desencolarmovs(tCola *cola,char ** mat,struct player *p);
void encolarMov(tCola *cola,struct moves *movimiento,int mov);
#endif // JUGADOR_H_INCLUDED
