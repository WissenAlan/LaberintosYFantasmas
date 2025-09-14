#include "jugador.h"
void moverjugador(char**mat,char movimiento,struct player *p)
{
    if(movimiento == 'w' && mat[p->posx -1][p->posy] != '#' && mat[p->posx-1][p->posy] != 'E')
    {
        move_up(mat,p);
    }
    if(movimiento == 's'  && mat[p->posx +1][p->posy] != '#')
    {
        move_down(mat,p);
    }
    if(movimiento == 'a' && mat[p->posx][p->posy-1] != '#')
    {
        move_left(mat,p);
    }
    if(movimiento =='d' && mat[p->posx][p->posy+1] != '#')
    {
        move_right(mat,p);
    }
}
void move_up(char**mat,struct player * p)
{
    mat[p->posx][p->posy]= CAMINO;
    p->last_x=p->posx;
    p->last_y=p->posy;
    p->posx--;
    mat[p->posx][p->posy]=JUGADOR;
}
void move_down(char**mat,struct player * p)
{
    mat[p->posx][p->posy]= CAMINO;
    p->last_x=p->posx;
    p->last_y=p->posy;
    p->posx++;
    mat[p->posx][p->posy]=JUGADOR;
}
void move_left(char**mat,struct player * p)
{
    mat[p->posx][p->posy]= CAMINO;
    p->last_x=p->posx;
    p->last_y=p->posy;
    p->posy--;
    mat[p->posx][p->posy]=JUGADOR;
}
void move_right(char**mat,struct player * p)
{
    mat[p->posx][p->posy]= CAMINO;
    p->last_x=p->posx;
    p->last_y=p->posy;
    p->posy++;
    mat[p->posx][p->posy]=JUGADOR;
}
void ai(char **mat,struct player *p)
{
    int try_w=state(mat,1,p);
    int try_s=state(mat,2,p);
    int try_a=state(mat,3,p);
    int try_d=state(mat,4,p);
    if(try_w >= try_s && try_w >= try_a && try_w >= try_d)
    {
        move_up(mat,p);
    }
    else
    {
        if(try_s >= try_a && try_s >=try_d)
        {
            move_down(mat,p);
        }
        else
        {
            if(try_a > try_d)
            {
                move_left(mat,p);
            }
            else
            {
                move_right(mat,p);
            }
        }
    }
}
int state(char **mat,int trypos,struct player *p)
{
    int reward = 0;
    int try_x=p->posx;
    int try_y=p->posy;
    int dif_x;
    int dif_y;
    switch(trypos)
    {
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
    if(mat[try_x][try_y]== '#' || mat[try_x][try_y] == 'E')
    {
        reward+= -100;
    }
    //if para que detecte algo lo modificamos para que detecte a el jugador;
    if(mat[try_x][try_y] == 'S')
    {
        reward+=100;
    }
    if(try_x > p->posx && mat[try_x][try_y] != '#')
    {
        reward+=50;
    }
    if(try_x  == p->last_x && try_y == p->last_y)
    {
        reward-=51;
    }
    return reward;
}

