#include "jugador.h"
void moverjugador(tCola *colamov,char**mat,char movimiento,struct player *p)
{
    struct moves move;
    move.posx= p->posx;
    move.posy= p->posy;
    if(movimiento == 'w' && mat[p->posx-1][p->posy] != '#' && mat[p->posx-1][p->posy] != 'E')
    {
        p->posx--;
        encolarMov(colamov,&move,ARRIBA);
    }
    if(movimiento == 's'  && mat[p->posx+1][p->posy] != '#')
    {
        p->posx++;
        encolarMov(colamov,&move,ABAJO);
    }
    if(movimiento == 'a' && mat[p->posx][p->posy-1] != '#')
    {
        p->posy--;
        encolarMov(colamov,&move,IZQUIERDA);
    }
    if(movimiento =='d' && mat[p->posx][p->posy+1] != '#')
    {
        p->posy++;
        encolarMov(colamov,&move,DERECHA);
    }
}
void encolarMov(tCola *cola,struct moves *movimiento,int mov)
{
    movimiento->move=mov;
    colaInsertar(cola,movimiento,sizeof(struct moves));
}
void ai(tCola *colamov,char **mat,struct player *player,struct ghost *p)
{
    int try_w=state(mat,1,player,p);
    int try_s=state(mat,2,player,p);
    int try_a=state(mat,3,player,p);
    int try_d=state(mat,4,player,p);
    struct moves move;
    move.posx= p->posx;
    move.posy= p->posy;
    if(try_w >= try_s && try_w >= try_a && try_w >= try_d)
    {
       encolarMov(colamov,&move,ARRIBA);
    }
    else
    {
        if(try_s >= try_a && try_s >=try_d)
        {
            encolarMov(colamov,&move,ABAJO);
        }
        else
        {
            if(try_a > try_d)
            {
                encolarMov(colamov,&move,IZQUIERDA);
            }
            else
            {
                encolarMov(colamov,&move,DERECHA);
            }
        }
    }
}
int abs(int numero)
{
    if(numero > 0)
    {
        return numero;
    }
    else
    {
        return -numero;
    }
}
int state(char **mat,int trypos,struct player *p,struct ghost *f)
{
    int reward = 0;
    int try_x=f->posx;
    int try_y=f->posy;
    int diff_x;
    int diff_y;
    int try_diff_x;
    int try_diff_y;
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
    if(mat[try_x][try_y]== '#' || mat[try_x][try_y] == 'E' || mat[try_x][try_y] == 'S')
    {
        reward+= -100;
    }
    //if para que detecte algo lo modificamos para que detecte a el jugador;
    if(mat[try_x][try_y] == 'P')
    {
        reward+=100;
    }
    diff_x=abs(f->posx - p->posx);
    diff_y =abs(f->posy - p->posy);
    try_diff_x= abs(try_x - p->posx);
    try_diff_y =abs(try_y - p->posy);
    if(try_diff_x < diff_x)
    {
        reward+=5;
    }
    if(try_diff_y < diff_y)
    {
        reward+=5;
    }
    return reward;
}

