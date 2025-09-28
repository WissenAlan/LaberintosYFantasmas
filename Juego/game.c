#include "main.h"
#include "jugador.h"
#include "mapa.h"
#include "game.h"
bool game_new(struct game **game)
{
   // tCola Colamov;
    *game=malloc(sizeof(struct game));
    if(*game == NULL)
    {
        printf("ERROR en malloc");
        return false;
    }
    (*game)->is_running=true;
    (*game)->p=malloc(sizeof(struct player));
    if((*game)-> p == NULL)
    {
        printf("ERROR en malloc");
        return false;
    }
    (*game)->m=malloc(sizeof(struct mapaL));
    if((*game)->m == NULL)
    {
        printf("ERROR en malloc");
        return false;
    }
    (*game)->m->exit=false;
    //leer el archivo de config en un futuro
    (*game)->m->mat=crearMatriz(21,21); // modificar por constantes dentro del struct game
//crearCola(&Colamov);
    //(*game)->colaMov=&Colamov;
    //aca agregamos la conecion con el servidor todo eso etc
    return true;
}
void game_free(struct game **game)
{
    eliminarMatriz((*game)->m->mat,21,21);
    free(*game);
    *game=NULL;
    printf("todo bien!");
}
void game_run(struct game *g)
{
    while(g->is_running)
    {

      //menu de inicio
      llenarMat(g->m->mat,21,21);
      crearLaberinto(g->m,21,21,g->p);
      //
        while(g->is_running && !g->m->exit)
        {
            mostrarMat(g->m->mat,21,21);
            game_update(g);
            checkend(g->m,g->p);
            system("cls");
        }




          // logica para cuando tengamos graficos
         /*game_start(g);
        while(g->is_running && !g->inicio)
        {
            game_events(g);
            game_update(g);
            game_draw(g);

            SDL_Delay(32);
        }*/
    }
}
void game_update(struct game *g)
{
    tCola cola ;
    crearCola(&cola);
    char tecla;
    tecla= getch(); // cambiar por eventos;
    moverjugador(&cola,g->m->mat,tecla,g->p);
    moverfantasmas(g,&cola);
    desencolarmovs(&cola,g->m->mat,g->p);
}
void moverfantasmas(struct game *g,tCola *cola)
{
    int i,j;
    struct ghost fant;
    for(i=0;i<21;i++)
    {
        for(j=0;j<21;j++)
        {
            if(g->m->mat[i][j] == 'F')
            {
                fant.posx=i;
                fant.posy=j;
                ai(cola,g->m->mat,g->p,&fant);
            }
        }
    }
}
void intercambiar(void *a,void*b,unsigned tam)
{
    int i;
    char aux;
    char *e1= (char*)a;
    char *e2= (char*)b;
    for(i=0;i<tam;i++)
    {
        aux= *e1;
        *e1 = *e2;
        *e2 = aux;
        e1++;
        e2++;
    }
}
void desencolarmovs(tCola *cola,char ** mat,struct player *p)
{
    struct moves movi;
    while (!colaVacia(cola))
    {
        /* sacarDeCola rellena 'movimientos' */
        sacarDeCola(cola, &movi, sizeof(struct moves));
        if(movi.move == ARRIBA)
        {
            if(mat[movi.posx-1][movi.posy] != FANTASMA && mat[movi.posx-1][movi.posy] != '#')
            {
                intercambiar(&mat[movi.posx][movi.posy],&mat[movi.posx-1][movi.posy],sizeof(char));
            }
            if(mat[movi.posx-1][movi.posy] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA)
            {
                mat[movi.posx][movi.posy] = CAMINO;
                p->lifes--;
            }

        }
        if(movi.move == ABAJO)
        {
            if(mat[movi.posx+1][movi.posy] != FANTASMA && mat[movi.posx+1][movi.posy] != '#')
            {
                intercambiar(&mat[movi.posx][movi.posy],&mat[movi.posx+1][movi.posy],sizeof(char));
            }
            if(mat[movi.posx+1][movi.posy] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA)
            {
                mat[movi.posx][movi.posy] = CAMINO;
                p->lifes--;
            }

        }
        if(movi.move == IZQUIERDA)
        {
            if(mat[movi.posx][movi.posy-1] != FANTASMA && mat[movi.posx][movi.posy-1] != '#')
            {
                intercambiar(&mat[movi.posx][movi.posy],&mat[movi.posx][movi.posy-1],sizeof(char));
            }
            if(mat[movi.posx][movi.posy-1] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA)
            {
                mat[movi.posx][movi.posy] = CAMINO;
                p->lifes--;
            }

        }
        if(movi.move == DERECHA)
        {
            if(mat[movi.posx][movi.posy+1] != FANTASMA && mat[movi.posx][movi.posy+1] != '#')
            {
                intercambiar(&mat[movi.posx][movi.posy],&mat[movi.posx][movi.posy+1],sizeof(char));
            }
            if(mat[movi.posx][movi.posy+1] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA)
            {
                mat[movi.posx][movi.posy] = CAMINO;
                p->lifes--;
            }

        }




}
}
