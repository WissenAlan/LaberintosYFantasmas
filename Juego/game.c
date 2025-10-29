#include "header/game.h"
#include "header/SDL_INIT.h"
#include "header/menus.h"
int game_new(tGame *g)
{
    char linea[TAM_BUFFER];
    char *valor;
    crearMapa(&g->m);
    FILE*config = fopen(ARCHIVO_CONFIG, "r");
    g->is_running=true;
    g->is_pausing=false;
    g->inicio=true;
    g->ranking=false;
    g->is_writing=false;
    if(!game_init_sdl(g))
    {
        return 0;
        printf("error");
    }
    if (!config)
    {
        printf("No hay archivo de configuracion de mapa.\nSe genera el juego en predeterminado.\n");
        g->cantFant = 3;
        g->cantVidas = 3;
        g->cantPremios = 2;
        g->m.filMapa = 21;
        g->m.colMapa = 21;
        g->cantVidasExt = 1;
    }
    else
    {
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &(g->m.filMapa));
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->m.colMapa);
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->cantVidas);
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->cantFant);
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->cantPremios);
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->cantVidasExt);
        fclose(config);
    }
    crearJugador(&g->p, g->cantVidas);
    if (llenarMapa(&g->p, &g->m, g->cantFant, g->cantPremios, g->cantVidasExt) == FALSE)
    {
        return FALSE;
    }
    crearCola(&(g->colaMov));
    return VERDADERO;
}
void asignarConfig(char* linea, int* parametro)
{
    char*valor = strchr(linea, ':');
    if (valor)
    {
        valor++; // avanzar para saltar los dos puntos ':'
        valor[strcspn(valor, "\n")] = '\0';// eliminar salto de línea
        *parametro = atoi(valor);
    }
}
void game_free(tGame *g)
{
    eliminarMatriz(g->m.mat, g->m.filMapa, g->m.colMapa);
    if(colaVacia(&(g->colaMov))!=COLA_VACIA)
        vaciarCola(&(g->colaMov));
    if(g->renderer)
    {
        SDL_DestroyRenderer(g->renderer);
        g->renderer=NULL;
    }
    if(g->window)
    {
        SDL_DestroyWindow(g->window);
        g->window=NULL;
    }
    SDL_DestroyTexture(g->personaje);
    SDL_DestroyTexture(g->fantasmas);
    SDL_DestroyTexture(g->rank);
    SDL_DestroyTexture(g->fondo);
    Mix_HaltChannel(-1);
    Mix_FreeMusic(g->musica);
    Mix_FreeChunk(g->sonidomenu);
    TTF_CloseFont(g->text_f);
    TTF_CloseFont(g->titulo_f);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    Mix_Quit();
    printf("\t\tFin del juego!");
}
void game_run(tGame *g)
{
    //crearConexion(g);
    //menuIngresarNombre(g);
    while(g->is_running)
    {
        if (g->inicio)
        {
            menu_inicio(g);
            SDL_Delay(32);
            Mix_HaltMusic();
            Mix_PlayMusic(g->musicajuego, -1);
        }
        game_events(g);
        game_draw(g);
        SDL_Delay(32);
        if(g->m.exit || g->m.jugadorMuerto)
        {
            g->is_pausing=true;
            menu_pausa(g);
        }
    }
}
void game_draw(tGame *g)
{
    int i,j;
    int sizec= WINDOW_WIDTH / g->m.colMapa;
    int sizef= WINDOW_HEIGHT / g->m.filMapa;
    SDL_Rect rect = {0,0,sizec-3,sizef-3}; // padding 3
    SDL_SetRenderDrawColor(g->renderer,0,0,0,255);
    SDL_RenderClear(g->renderer);
    SDL_SetRenderDrawColor(g->renderer,CUADRADO_COLOR);
    for(i=0; i<g->m.filMapa; i++)
    {
        rect.y= sizef*i;
        for(j=0; j<g->m.colMapa; j++)
        {
            rect.x=sizec*j;
            if(g->m.mat[i][j] == PARED)
            {

                SDL_RenderFillRect(g->renderer,&rect);
            }
            if(g->m.mat[i][j] == JUGADOR)
            {
                SDL_RenderCopy(g->renderer,g->personaje, NULL, &rect);
            }
            if(g->m.mat[i][j] == FANTASMA)
            {
                SDL_RenderCopy(g->renderer,g->fantasmas, NULL, &rect);
            }

        }
    }
   SDL_RenderPresent(g->renderer);
}
void game_start(tGame *g)
{
}
void game_update(tGame *g)
{
    moverFantasmas(g);
    desencolarMovs(&(g->colaMov), g->m.mat, &g->p);
    checkend(&g->m,&g->p);
    checklifes(&g->m,&g->p);
}
void moverFantasmas(tGame *g)
{
    int i, j;
    tFantasma fant;
    for (i = 0; i < g->m.filMapa; i++)
    {
        for (j = 0; j < g->m.colMapa; j++)
        {
            if (g->m.mat[i][j] == FANTASMA)
            {
                fant.posx = i;
                fant.posy = j;
                ai(&(g->colaMov), g->m.mat, &g->p, &fant);
            }
        }
    }
}
void game_events(tGame *g)
{
    while(SDL_PollEvent(&g->eventos))
    {
        switch(g->eventos.type)
        {
        case SDL_QUIT:
            g->is_running=false;
            break;
        case SDL_KEYDOWN:
            switch(g->eventos.key.keysym.scancode)
            {
            case SDL_SCANCODE_M:
                Mix_HaltMusic();
                break;
            case SDL_SCANCODE_W:
                moverJugador(&g->colaMov,g->m.mat,&g->p,'w');
                game_update(g);
                break;
            case SDL_SCANCODE_S:
                moverJugador(&g->colaMov,g->m.mat,&g->p,'s');
                game_update(g);
                break;
            case SDL_SCANCODE_A:
                moverJugador(&g->colaMov,g->m.mat,&g->p,'a');
                game_update(g);
                break;
            case SDL_SCANCODE_D:
                moverJugador(&g->colaMov,g->m.mat,&g->p,'d');
                game_update(g);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

}
void intercambiar(void *a, void*b, unsigned tam)
{
    int i;
    char aux;
    char *e1 = (char*)a;
    char *e2 = (char*)b;
    for (i = 0; i < tam; i++)
    {
        aux = *e1;
        *e1 = *e2;
        *e2 = aux;
        e1++;
        e2++;
    }
}
void desencolarMovs(tCola *cola, char ** mat, tJugador *pJug)
{
    tMovimiento movi;
    int x, y;
    while (colaVacia(cola) != COLA_VACIA)
    {
        x = 0;
        y = 0;
        /* sacarDeCola rellena 'movimientos' */
        sacarDeCola(cola, &movi, sizeof(tMovimiento));
        if (movi.movimiento == ARRIBA)
            x++;
        else if (movi.movimiento == ABAJO)
            x--;
        else if (movi.movimiento == IZQUIERDA)
            y++;
        else
            y--;
        if (mat[movi.posx][movi.posy] == JUGADOR)
        {
            mat[movi.posx+x][movi.posy+y] = CELDA;
            pJug->vidas--;
        }
        else if (mat[movi.posx][movi.posy] == CELDA)
            intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx + x][movi.posy + y], sizeof(char));
    }
}

char* crearConexion(tGame* g, int mensaje)
{
    if (init_winsock() != 0)
    {
        printf("Error con WINSOCK\n");
        return NULL;
    }
    SOCKET soc = connect_to_server(SERVER_IP, PUERTO);
    if (soc == INVALID_SOCKET)
    {
        printf("Error de conexion\n");
//        iniciarJuego(g);
        WSACleanup();
        return NULL;
    }
    printf("Conectado al servidor\n");
    char nombre[TAM_NOMBRE], buffer[TAM_BUFFER], response[TAM_BUFFER];
    printf("> ");
    switch (mensaje)
    {
    case 1:
        printf("Ingrese su nombre:\n");
        scanf("%s", nombre);
        nombre[strcspn(nombre, "\n")] = '\0'; //quitar salto de linea
        strcpy(buffer, "NOMBRE|");
        buffer[strcspn(buffer, "\n")] = '\0'; //quitar salto de linea
        strcat(buffer, nombre);
        printf("Enviado: %s\n", buffer);
        if (send_request(soc, buffer, response) == 0)
            iniciarJuego(g);
//                printf("Respuesta: %s\n", response);
        else
        {
            printf("Error al enviar o recibir datos\n");
            break;
        }
        break;
    case 2:
        strcpy(buffer, "RANKING|");
        buffer[strcspn(buffer, "\n")] = '\0'; //quitar salto de linea
        printf("Enviado: %s\n", buffer);
        if (send_request(soc, buffer, response) == 0)
            printf("Respuesta: %s\n", response);
        else
            printf("Error al enviar o recibir datos\n");
        break;
    default:
        printf("Opcion invalida\n");
        break;
    }
    close_connection(soc);
    return response;
}

void iniciarJuego(tGame* g)
{
    while (g->m.exit == FALSE && getVidasJugador(&g->p) > 0)
    {
        printf("Cantidad de vidas: %d \tCantidad de puntos %d\n",getVidasJugador(&g->p),getPuntosJugador(&g->p));
        mostrarMat(g->m.mat, g->m.filMapa, g->m.colMapa);
        game_update(g);
        checkend(&g->m, &g->p);
        system("cls");
    }
}
