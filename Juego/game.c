#include "header/game.h"

int game_new(tGame *g) {
    char linea[TAM_BUFFER];
    char *valor;
    crearMapa(&g->m);
    FILE*config = fopen(ARCHIVO_CONFIG, "r");
    if (!config) {
        printf("No hay archivo de configuracion de mapa.\nSe genera el juego en predeterminado.\n");
        g->cantFant = 3;
        g->cantVidas = 3;
        g->cantPremios = 2;
        g->m.filMapa = 21;
        g->m.colMapa = 21;
        g->cantVidasExt = 1;
    } else {
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
    if (llenarMapa(&g->p, &g->m, g->cantFant, g->cantPremios, g->cantVidasExt) == FALSE){
        return FALSE;
    }
    crearCola(&(g->colaMov));
    g->is_running = VERDADERO;
    return VERDADERO;
}
void asignarConfig(char* linea, int* parametro) {
    char*valor = strchr(linea, ':');
    if (valor) {
        valor++; // avanzar para saltar los dos puntos ':'
        valor[strcspn(valor, "\n")] = '\0';// eliminar salto de línea
        *parametro = atoi(valor);
    }
}
void game_free(tGame *g) {
    eliminarMatriz(g->m.mat, g->m.filMapa, g->m.colMapa);
    if(colaVacia(&(g->colaMov))!=COLA_VACIA)
        vaciarCola(&(g->colaMov));
    printf("\t\tFin del juego!");
}
void game_run(tGame *g) {
    crearConexion(g);
    if(getVidasJugador(&g->p)<=0)
        printf("\n\t\tPERDISTE\n");
    else
        printf("\n\t\tGANASTE\n");
}
void game_update(tGame *g) {
    moverJugador(&(g->colaMov), g->m.mat, &g->p);
    moverFantasmas(g);
    desencolarMovs(&(g->colaMov), g->m.mat, &g->p);
}
void moverFantasmas(tGame *g) {
    int i, j;
    tFantasma fant;
    for (i = 0; i < g->m.filMapa; i++) {
        for (j = 0; j < g->m.colMapa; j++) {
            if (g->m.mat[i][j] == FANTASMA) {
                fant.posx = i;
                fant.posy = j;
                ai(&(g->colaMov), g->m.mat, &g->p, &fant);
            }
        }
    }
}
void intercambiar(void *a, void*b, unsigned tam) {
    int i;
    char aux;
    char *e1 = (char*)a;
    char *e2 = (char*)b;
    for (i = 0; i < tam; i++) {
        aux = *e1;
        *e1 = *e2;
        *e2 = aux;
        e1++;
        e2++;
    }
}
void desencolarMovs(tCola *cola, char ** mat, tJugador *pJug) {
    tMovimiento movi;
    int x, y;
    while (colaVacia(cola) != COLA_VACIA) {
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
        if (mat[movi.posx][movi.posy] == JUGADOR) {
            mat[movi.posx+x][movi.posy+y] = CELDA;
            pJug->vidas--;
        }else if (mat[movi.posx][movi.posy] == CELDA)
            intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx + x][movi.posy + y], sizeof(char));
    }
}

int crearConexion(tGame* g) {
    if (init_winsock() != 0) {
        printf("Error con WINSOCK\n");
        return ERROR_WINSOCK;
    }
    SOCKET soc = connect_to_server(SERVER_IP, PUERTO);
    if (soc == INVALID_SOCKET) {
        printf("Error de conexion\n");
        iniciarJuego(g);
        WSACleanup();
        return ERROR_CONEXION;
    }
    printf("Conectado al servidor\nMenu:\n1)Jugar nueva partida.\n2)Ver ranking de jugadores\n3)Salir.\n");
    char nombre[TAM_NOMBRE], buffer[TAM_BUFFER], response[TAM_BUFFER];
    int opc;
    while (opc != 3) {
        printf("> ");
        scanf("%d", &opc);
        getchar();
        switch (opc) {
        case 1:
            printf("Ingrese su nombre:\n");
            scanf("%s", nombre);
            nombre[strcspn(nombre, "\n")] = '\0'; //quitar salto de linea
            if (strlen(nombre) == 0)
                continue;
            strcpy(buffer, "NOMBRE|");
            buffer[strcspn(buffer, "\n")] = '\0'; //quitar salto de linea
            strcat(buffer, nombre);
            printf("Enviado: %s\n", buffer);
            if (send_request(soc, buffer, response) == 0)
                iniciarJuego(g);
//                printf("Respuesta: %s\n", response);
            else {
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
    }
    close_connection(soc);
    return TODO_OK;
}

void iniciarJuego(tGame* g) {
    while (g->m.exit == FALSE && getVidasJugador(&g->p) > 0) {
        printf("Cantidad de vidas: %d \tCantidad de puntos %d\n",getVidasJugador(&g->p),getPuntosJugador(&g->p));
        mostrarMat(g->m.mat, g->m.filMapa, g->m.colMapa);
        game_update(g);
        checkend(&g->m, &g->p);
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
