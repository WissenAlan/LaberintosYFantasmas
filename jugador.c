#include "jugador.h"
void moverjugador(char**mat,char movimiento,unsigned * posx,unsigned * posy)
{
    if(movimiento == 'w' && mat[(*posx)-1][*posy] != '#' && mat[(*posx)-1][*posy] != 'E')
    {
        mat[*posx][*posy]= CAMINO;
        (*posx)-=1;
        mat[*posx][*posy]=JUGADOR;
    }
    if(movimiento == 's' && mat[(*posx)+1][*posy] != '#' && mat[(*posx)+1][*posy] != 'S')
    {
        mat[*posx][*posy]= CAMINO;
        (*posx)+=1;
        mat[*posx][*posy]=JUGADOR;
    }
    if(movimiento == 'a' && mat[*posx][(*posy)-1] != '#')
    {
        mat[*posx][*posy]= CAMINO;
        (*posy)-=1;
        mat[*posx][*posy]=JUGADOR;
    }
    if(movimiento =='d' && mat[*posx][(*posy)+1] != '#')
    {
        mat[*posx][*posy]= CAMINO;
        (*posy)+=1;
        mat[*posx][*posy]=JUGADOR;
    }
}
