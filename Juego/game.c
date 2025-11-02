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
    g->is_running = true;
    g->is_pausing = false;
    g->inicio = true;
    g->ranking = false;
    g->is_writing = false;
    if (!game_init_sdl(g))
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
        return FALSE;
    crearCola(&(g->colaMov));
    crearCola(&(g->colaMovsJugador));
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
    if (colaVacia(&(g->colaMov)) != COLA_VACIA)
        vaciarCola(&(g->colaMov));
    if (g->renderer)
    {
        SDL_DestroyRenderer(g->renderer);
        g->renderer = NULL;
    }
    if (g->window)
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
     crearConexion(g);
    while (g->is_running)
    {
        if (g->inicio)
        {
            menu_inicio(g);
            Mix_HaltMusic();
            Mix_PlayMusic(g->musicajuego, -1);
        }
        game_events(g);
        game_draw(g);
        SDL_Delay(32);
        if (g->m.exit || g->m.jugadorMuerto)
        {
            g->is_pausing = true;
            menu_pausa(g);
        }
    }
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
    for (i = 0; i < g->m.filMapa; i++)
    {
        rect.y = sizef * i;
        for (j = 0; j < g->m.colMapa; j++)
        {
            rect.x = sizec * j;
            if (g->m.mat[i][j] == JUGADOR)
                SDL_RenderCopy(g->renderer, g->personaje, NULL, &rect);
            if (g->m.mat[i][j] == FANTASMA)
                SDL_RenderCopy(g->renderer, g->fantasmas, NULL, &rect);
            if (g->m.mat[i][j] == BONIFICACION)
                SDL_RenderCopy(g->renderer, g->premio, NULL, &rect);
            if (g->m.mat[i][j] == ENTRADA)
                SDL_RenderCopy(g->renderer, g->entrada, NULL, &rect);
            if (g->m.mat[i][j] == SALIDA)
                SDL_RenderCopy(g->renderer, g->salida, NULL, &rect);
            if (g->m.mat[i][j] == PARED)
                SDL_RenderCopy(g->renderer, g->pared, NULL, &rect);
            if (g->m.mat[i][j] == CELDA)
                SDL_RenderCopy(g->renderer, g->piso, NULL, &rect);
        }
    }
    SDL_RenderPresent(g->renderer);
}
void game_update(tGame *g)
{
    moverFantasmas(g);
    desencolarMovs(&(g->colaMov), g->m.mat, &g->p);
    checkend(&g->m, &g->p);
    checklifes(&g->m, &g->p);
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
                ai(&(g->colaMov),&g->m, &g->p, &fant);
            }
        }
    }
}
void game_events(tGame *g)
{
    tMovimiento tMov;
    while (SDL_PollEvent(&g->eventos))
    {
        switch (g->eventos.type)
        {
        case SDL_QUIT:
            g->is_running = false;
            break;
        case SDL_KEYDOWN:
            switch (g->eventos.key.keysym.scancode)
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
        if (movi.movimiento == SDL_SCANCODE_W)
            x++;
        else if (movi.movimiento == SDL_SCANCODE_S)
            x--;
        else if (movi.movimiento == SDL_SCANCODE_A)
            y++;
        else
            y--;
        if (!(movi.entidad == FANTASMA && mat[movi.posx + x][movi.posy + y] == JUGADOR)) //FANTASMA PISADO
        {
            if (movi.entidad == FANTASMA && mat[movi.posx][movi.posy] == JUGADOR)
            {
                mat[movi.posx + x][movi.posy + y] = CELDA;
                pJug->vidas--;
            }
            if (movi.entidad == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA)
            {
                mat[movi.posx][movi.posy] = CELDA;
                pJug->vidas--;
            }
            if (movi.entidad == JUGADOR && mat[movi.posx][movi.posy] == BONIFICACION)
            {
                mat[movi.posx][movi.posy] = CELDA;
                pJug->puntos += 100;
                pJug->roundBuff+=6;
            }
            if (mat[movi.posx][movi.posy] == CELDA)
                intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx + x][movi.posy + y], sizeof(char));
        }
    }
}

int crearConexion(tGame *g) {
    SOCKET soc;
    char buffer[TAM_BUFFER], response[TAM_BUFFER];

    if (init_winsock() != 0) {
        printf("Error al inicializar Winsock.\n");
        return 0;
    }

    soc = connect_to_server(SERVER_IP, PUERTO);
    if (soc == INVALID_SOCKET) {
        printf("No se pudo conectar al servidor. Modo local.\n");
        WSACleanup();
        return 0;
    }

    // guarda el socket global
    setSocketCliente(soc);

    // manda el nombre del jugador al servidor
    snprintf(buffer, sizeof(buffer), "NOMBRE|%s", g->p.nombre);

    if (send_request(soc, buffer, response) == TODO_OK)
        printf("[Servidor] %s\n", response);
    else
        printf("No se pudo registrar el nombre.\n");

    printf("[DEBUG] Conexion establecida y mantenida abierta.\n");
    return 1;
}

void iniciarJuego(tGame* g)
{
    while (g->m.exit == FALSE && getVidasJugador(&g->p) > 0)
    {
        printf("Cantidad de vidas: %d \tCantidad de puntos %d\n", getVidasJugador(&g->p), getPuntosJugador(&g->p));
        mostrarMat(g->m.mat, g->m.filMapa, g->m.colMapa);
        game_update(g);
        checkend(&g->m, &g->p);
        system("cls");
    }
}

void guardarPartida(tGame *g) {
    // Buscar jugador real
    tJugadorDatos jugadorReal;

    if (!buscarJugadorPorNombre("..\\Servidor\\bin\\Debug\\jugadores.dat", g->p.nombre, &jugadorReal)) {
        printf("Jugador '%s' no encontrado, no se guarda la partida.\n", g->p.nombre);
        return;
    }

    FILE *pf = fopen("partidas.dat", "ab+");
    if (!pf) {
        printf("Error al abrir partidas.dat\n");
        return;
    }

    tPartidaDatos partida;
    memset(&partida, 0, sizeof(partida));

    // Asignar id de partida
    fseek(pf, 0, SEEK_END);
    long tam = ftell(pf);
    int cant = tam / sizeof(tPartidaDatos);
    partida.id_partida = cant + 1;

    // Usar datos correctos
    partida.id_jugador = jugadorReal.id;
    partida.puntaje = g->p.puntos;
    partida.movimientos = g->p.movimientos;

    fwrite(&partida, sizeof(tPartidaDatos), 1, pf);
    fclose(pf);

    printf("✅ Partida guardada correctamente:\n");
    printf("Jugador: %s (ID: %d)\n", jugadorReal.nombre, jugadorReal.id);
    printf("Partida: %d | Puntos: %d | Movimientos: %d\n",
           partida.id_partida, partida.puntaje, partida.movimientos);
}


void mostrarPartidas()
{
    FILE *pf = fopen(ARCH_PARTIDAS, "rb");
    if (!pf) {
        printf(" No hay partidas guardadas.\n");
        return;
    }

    tPartida p;
    printf("\n=== PARTIDAS GUARDADAS ===\n");
    while (fread(&p, sizeof(tPartida), 1, pf) == 1) {
        printf("ID Partida: %-3d | Jugador: %-3d | Puntaje: %-5d | Movimientos: %-3d\n",
               p.idPartida, p.idJugador, p.puntaje, p.movimientos);
    }
    fclose(pf);
}
