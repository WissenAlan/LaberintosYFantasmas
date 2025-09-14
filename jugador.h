#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED
#include "main.h"
struct player
{
    unsigned posx;
    unsigned posy;
};
void moverjugador(char**mat,char movimiento,unsigned * posx,unsigned * posy);

#endif // JUGADOR_H_INCLUDED
