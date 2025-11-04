#include "header/game.h"
#include "header/SDL_INIT.h"
#include "header/menus.h"
#define ARCH_JUG "jugadores.dat"
#define ARCH_IDX "jugadores.idx"
#define ARCH_PARTIDAS "partidas.dat"

int game_new(tGame *g)
{
    char linea[TAM_BUFFER];
    char *valor;
    crearMapa(&g->m);
    FILE*config = fopen(ARCHIVO_CONFIG, "r");
    g->is_running = VERDADERO;
    g->is_pausing = INCORRECTO;
    g->inicio = VERDADERO;
    g->ranking = INCORRECTO;
    g->is_writing = INCORRECTO;
    if(!game_init_sdl(g))
    {
        return 0;
        printf("error");
    }
    if(!config)
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
    balancearConfig(g);
    crearJugador(&g->p, g->cantVidas);
    if(llenarMapa(&g->p, &g->m, g->cantFant, g->cantPremios, g->cantVidasExt) == INCORRECTO)
        return INCORRECTO;
    crearCola(&(g->colaMov));
    crearCola(&(g->colaMovsJugador));
    crearConexion(g);
    return VERDADERO;
}
void balancearConfig(tGame *g)
{
    int totalCasillas;
    if(g->m.filMapa < 10)
        g->m.filMapa = 10;
    if(g->m.colMapa < 10)
        g->m.colMapa = 10;
    if(g->m.filMapa > 40)
        g->m.filMapa = 40;
    if(g->m.colMapa > 40)
        g->m.colMapa = 40;
    totalCasillas = g->m.filMapa * g->m.filMapa;
    if(g->cantPremios > totalCasillas / 100)
        g->cantPremios = totalCasillas / 100 - 1;
    if((g->cantFant - 1) > totalCasillas / 100)
        g->cantFant = totalCasillas / 100;
    if(g->cantVidasExt > totalCasillas / 100)
        g->cantVidasExt = totalCasillas / 200;
    if(g->cantVidas > 3)
        g->cantVidas = 3;
}
void asignarConfig(char* linea, int* parametro)
{
    char*valor = strchr(linea, ':');
    if(valor)
    {
        valor++; // avanzar para saltar los dos puntos ':'
        valor[strcspn(valor, "\n")] = '\0';// eliminar salto de línea
        *parametro = ABS(atoi(valor));
    }
}
void game_free(tGame *g)
{
    eliminarMatriz(g->m.mat, g->m.filMapa, g->m.colMapa);
    if(colaVacia(&(g->colaMov)) != COLA_VACIA)
        vaciarCola(&(g->colaMov));
    if(g->renderer)
    {
        SDL_DestroyRenderer(g->renderer);
        g->renderer = NULL;
    }
    if(g->window)
    {
        SDL_DestroyWindow(g->window);
        g->window = NULL;
    }
    SDL_DestroyTexture(g->personaje);
    SDL_DestroyTexture(g->fantasmas);
    SDL_DestroyTexture(g->rank);
    SDL_DestroyTexture(g->fondo);
    SDL_DestroyTexture(g->premio);
    SDL_DestroyTexture(g->pared);
    SDL_DestroyTexture(g->piso);
    SDL_DestroyTexture(g->fondonombre);
    SDL_DestroyTexture(g->salida);
    SDL_DestroyTexture(g->vidaextra);
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
    menuIngresarNombre(g);
    while(g->is_running)
    {
        if(g->inicio)
        {
            menu_inicio(g);
            Mix_HaltMusic();
            Mix_PlayMusic(g->musicajuego, -1);
        }
        if(g->is_running)
        {
            //parche
            game_events(g);
            game_draw(g);
        }
        SDL_Delay(32);
        if(g->m.exit == VERDADERO || g->m.jugadorMuerto)
        {
            g->is_pausing = VERDADERO;
            if(g->m.exit == VERDADERO)
                procesarGuardarPartida(g);
            menu_pausa(g);
        }
    }
}
void procesarGuardarPartida(tGame*g)
{
    char buffer[TAM_BUFFER], response[TAM_BUFFER];
    sprintf(buffer, "|NOMBRE|%s|%d", g->p.nombre, g->p.puntos);
    enviarMensaje(g->soc, buffer, response);
}
void game_draw(tGame *g)
{
    int i, j;
    int sizec = WINDOW_WIDTH / g->m.colMapa;
    int sizef = WINDOW_HEIGHT / g->m.filMapa;
    SDL_Rect rect = {0, 0, sizec, sizef};
    SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
    SDL_RenderClear(g->renderer);
    SDL_SetRenderDrawColor(g->renderer, CUADRADO_COLOR);
    for(i = 0; i < g->m.filMapa; i++)
    {
        rect.y = sizef * i;
        for(j = 0; j < g->m.colMapa; j++)
        {
            rect.x = sizec * j;
            if(g->m.mat[i][j] == JUGADOR)
                SDL_RenderCopy(g->renderer, g->personaje, NULL, &rect);
            if(g->m.mat[i][j] == FANTASMA)
                SDL_RenderCopy(g->renderer, g->fantasmas, NULL, &rect);
            if(g->m.mat[i][j] == BONIFICACION)
                SDL_RenderCopy(g->renderer, g->premio, NULL, &rect);
            if(g->m.mat[i][j] == ENTRADA)
                SDL_RenderCopy(g->renderer, g->entrada, NULL, &rect);
            if(g->m.mat[i][j] == SALIDA)
                SDL_RenderCopy(g->renderer, g->salida, NULL, &rect);
            if(g->m.mat[i][j] == PARED)
                SDL_RenderCopy(g->renderer, g->pared, NULL, &rect);
            if(g->m.mat[i][j] == CELDA)
                SDL_RenderCopy(g->renderer, g->piso, NULL, &rect);
            if(g->m.mat[i][j] == VIDAEXT)
                SDL_RenderCopy(g->renderer, g->vidaextra, NULL, &rect);
        }
    }
    SDL_RenderPresent(g->renderer);
}
void game_update(tGame *g)
{
    moverFantasmas(g);
    desencolarMovs(&(g->colaMov), g->m.mat, &(g->p));
    checkend(&(g->m), &(g->p));
    checklifes(&(g->m), &(g->p));
}
void moverFantasmas(tGame *g)
{
    int i, j;
    tFantasma fant;
    for(i = 0; i < g->m.filMapa; i++)
    {
        for(j = 0; j < g->m.colMapa; j++)
        {
            if(g->m.mat[i][j] == FANTASMA)
            {
                fant.posx = i;
                fant.posy = j;
                ai(&(g->colaMov), &g->m, &g->p, &fant);
            }
        }
    }
}
void game_events(tGame *g)
{
    tMovimiento tMov;
    while(SDL_PollEvent(&g->eventos))
    {
        switch(g->eventos.type)
        {
        case SDL_QUIT:
            g->is_running = INCORRECTO;
            break;
        case SDL_KEYDOWN:
            switch(g->eventos.key.keysym.scancode)
            {
            case SDL_SCANCODE_M:
                Mix_HaltMusic();
                break;
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                moverJugador(&g->colaMov, g->m.mat, &g->p, g->eventos.key.keysym.scancode);
                verPrimero(&g->colaMov, &tMov, sizeof(tMov));
                colaInsertar(&g->colaMovsJugador, &tMov, sizeof(tMov));
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
    for(i = 0; i < tam; i++)
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
    int x, y, checkeoDobleMovimiento = 0;
    while(colaVacia(cola) != COLA_VACIA)
    {
        x = 0;
        y = 0;
        /* sacarDeCola rellena 'movimientos' */
        sacarDeCola(cola, &movi, sizeof(tMovimiento));
        if(movi.movimiento == SDL_SCANCODE_W)
            x++;
        else if(movi.movimiento == SDL_SCANCODE_S)
            x--;
        else if(movi.movimiento == SDL_SCANCODE_A)
            y++;
        else
            y--;
        if(!(movi.entidad == FANTASMA && mat[movi.posx + x][movi.posy + y] == JUGADOR))  //FANTASMA PISADO
        {
            if(movi.entidad == JUGADOR && mat[movi.posx][movi.posy] == SALIDA)
            {
                pJug->puntos += 300;
                return;
            }
            if(movi.entidad == FANTASMA && mat[movi.posx][movi.posy] == JUGADOR)
            {
                mat[movi.posx + x][movi.posy + y] = CELDA;
                pJug->vidas--;
                mat[pJug->posx][pJug->posy] = CELDA;
                pJug->posx = 1;
                pJug->posy = 1;
                mat[pJug->posx][pJug->posy] = JUGADOR;
            }
            if(movi.entidad == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA)
            {
                pJug->vidas--;
                mat[movi.posx][movi.posy] = CELDA;
                mat[pJug->posx + x][pJug->posy + y] = CELDA;
                pJug->posx = 1;
                pJug->posy = 1;
                mat[pJug->posx][pJug->posy] = JUGADOR;
            }
            if(movi.entidad == JUGADOR && mat[movi.posx][movi.posy] == BONIFICACION)
            {
                mat[movi.posx][movi.posy] = CELDA;
                pJug->puntos += 100;
                pJug->roundBuff += 8;
            }
            if(movi.entidad == JUGADOR && mat[movi.posx][movi.posy] == VIDAEXT)
            {
                mat[movi.posx][movi.posy] = CELDA;
                pJug->vidas++;
            }
            if(mat[movi.posx][movi.posy] == CELDA)
                intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx + x][movi.posy + y], sizeof(char));
        }
    }
}
void mandarJugadorAEntrada(tJugador *jugador, char **mat)
{
    jugador->vidas--;
    mat[jugador->posx][jugador->posy] = CELDA;
    jugador->posx = 1;
    jugador->posy = 1;
    mat[jugador->posx][jugador->posy] = JUGADOR;
}
int crearConexion(tGame *g)
{
    if(init_winsock() != 0)
    {
        printf("Error al inicializar Winsock.\n");
        printf("No se pudo conectar al servidor. Jugando en Modo local.\n");
        return INCORRECTO;
    }
    g->soc = connect_to_server(SERVER_IP, PUERTO);
    if(g->soc == INVALID_SOCKET)
    {
        printf("No se pudo conectar al servidor. Jugando en Modo local.\n");
        WSACleanup();
        return INCORRECTO;
    }
    printf("[DEBUG] Conexion establecida y mantenida abierta.\n");
    return VERDADERO;
}

