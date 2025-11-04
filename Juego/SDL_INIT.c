#include "header/SDL_INIT.h"
int game_init_sdl(tGame *g)
{
    if(SDL_Init(SDL_FLAGS))
    {
        printf("Error iniciando sdl %s",SDL_GetError());
        return 0;
    }

    if((IMG_Init(IMG_FLAGS)& IMG_FLAGS) != IMG_FLAGS)
    {
        printf("Error iniciando la imagen %s",IMG_GetError());
        return 0;
    }

    if(TTF_Init())
    {
        printf("ERROR iniciando el texto n %s",TTF_GetError());
        return 0;
    }
    g->window=SDL_CreateWindow(WINDOW_TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,0);
    if(!g->window)
    {
        printf("ERROR al crear la ventana %s",SDL_GetError());
        return 0;
    }

    g->renderer = SDL_CreateRenderer(g->window,-1,RENDERER_FLAGS);
    if(!g->renderer)
    {
        printf("ERROR al crear el renderer %s",SDL_GetError());
        return 0;
    }
    SDL_Surface* icon_surface=IMG_Load("assets/logo.png");
    if(!icon_surface)
    {
        printf("ERROR al crear la superficie de el icono %s",IMG_GetError());
        return 0;
    }
    int mix_init = Mix_Init(MIXER_FLAGS);
    if((mix_init & MIXER_FLAGS )!= MIXER_FLAGS)
    {
        printf("ERROR iniciando SDL_MIXER %s",Mix_GetError());
        return 0;
    }
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024))
    {
        printf("ERROR abriendo el audio %s",Mix_GetError());
        return 0;
    }
    load_media(g);
    SDL_SetWindowIcon(g->window,icon_surface);
    SDL_FreeSurface(icon_surface);
    icon_surface=NULL;
    return 1;


}
void load_media(tGame *g)
{
    SDL_Surface * superficie=NULL;
    g->sonidomenu=Mix_LoadWAV("assets/sonidomenu.ogg");
    g->musicajuego= Mix_LoadMUS("assets/musicajuego.ogg");
    g->musica= Mix_LoadMUS("assets/musicamenu.ogg");
    Mix_VolumeMusic(25);
    Mix_VolumeChunk(g->sonidomenu,45);
    g->fondo=IMG_LoadTexture(g->renderer,"assets/fondo.png");
    if(!g->fondo)
    {
        printf("ERROR creacion textura %s\n",IMG_GetError());
    }
    g->fantasmas=IMG_LoadTexture(g->renderer,"assets/fantasma.png");
    if(!g->fantasmas)
    {
         printf("ERROR creacion textura %s\n",IMG_GetError());
    }
    g->fondonombre=IMG_LoadTexture(g->renderer,"assets/fondonombre.png");
    if(!g->fondonombre)
    {
        printf("ERROR creacion textura %s\n",IMG_GetError());
    }
    g->premio=IMG_LoadTexture(g->renderer,"assets/premio.png");
    if(!g->premio)
    {
         printf("ERROR creacion textura %s\n",IMG_GetError());
    }
    g->entrada=IMG_LoadTexture(g->renderer,"assets/entrada.png");
    if(!g->entrada)
    {
         printf("ERROR creacion textura %s\n",IMG_GetError());
    }
    g->salida=IMG_LoadTexture(g->renderer,"assets/salida.png");
    if(!g->salida)
    {
         printf("ERROR creacion textura %s\n",IMG_GetError());
    }
    g->vidaextra=IMG_LoadTexture(g->renderer,"assets/VidaExt.png");
    if(!g->vidaextra)
    {
         printf("ERROR creacion textura %s\n",IMG_GetError());
    }
    g->personaje=IMG_LoadTexture(g->renderer,"assets/personaje.png");
    if(!g->personaje)
    {
         printf("ERROR creacion textura %s\n",IMG_GetError());
    }

     g->piso=IMG_LoadTexture(g->renderer,"assets/piso.png");
    if(!g->piso)
    {
         printf("ERROR creacion textura %s\n",IMG_GetError());
    }
     g->pared=IMG_LoadTexture(g->renderer,"assets/pared.png");
    if(!g->pared)
    {
         printf("ERROR creacion textura %s\n",IMG_GetError());
    }

    g->rank=IMG_LoadTexture(g->renderer,"assets/ranking.png");
    if(!g->rank)
    {
        printf("ERROR creacion textura %s\n",IMG_GetError());
    }
    SDL_FreeSurface(superficie);
    if(!g->fondo)
    {
        printf("Error creacion de imagen fondo\n");
    }
    g->text_f=TTF_OpenFont("assets/freesansbold.ttf",24);
    if(!g->text_f)
    {
        printf("ERROR creacion texto %s\n",TTF_GetError());
    }
    g->titulo_f=TTF_OpenFont("assets/freesansbold.ttf",23);
    if(!g->titulo_f)
    {
        printf("ERROR creacion texto %s\n",TTF_GetError());
    }
}
