#include "main.h"
#include "jugador.h"
#include "mapa.h"
#include "game.h"

int game_new(tGame *g) {
    // tCola Colamov;
    char linea[TAM_BUFFER];
    char *valor;
    g->m = malloc(sizeof(tMapa));
    if (!g->m) {
        printf("ERROR en malloc");
        return FALSE;
    }
    FILE*config = fopen(ARCHIVO_CONFIG, "r");
    if (!config) {
        printf("No hay archivo de configuracion de mapa.\nSe genera el juego en predeterminado.\n");
        g->cantFant = 3;
        g->cantVidas = 3;
        g->cantPremios = 2;
        g->m->filMapa = 21;
        g->m->colMapa = 21;
        g->cantVidasExt = 1;
    } else {
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->m->filMapa);
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->m->colMapa);
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
    if (crearJugador(g) == FALSE) {
        free(g->m);
        return FALSE;
    }
    if (crearMapa(g->p, g->m, g->cantFant, g->cantPremios, g->cantVidasExt) == FALSE)
        return FALSE;
    crearCola(&(g->colaMov));
    //aca agregamos la conecion con el servidor todo eso etc
    g->is_running = VERDADERO;
    return VERDADERO;
}
void asignarConfig(char* linea, int* parametro) {
    char*valor = strchr(linea, ':');
    if (valor) {
        valor++; // avanzar para saltar los dos puntos ':'
        // eliminar salto de línea si existe
        valor[strcspn(valor, "\n")] = '\0';
        *parametro = atoi(valor);
    }
}

int crearJugador(tGame *g) {
    g->p = (player*)malloc(sizeof(player));
    if (!g->p) {
        printf("ERROR en malloc");
        return FALSE;
    }
    g->p->vidas = g->cantVidas;
    return VERDADERO;
}
void game_free(tGame *g) {
    free(g->p);
    eliminarMatriz(g->m->mat, g->m->filMapa, g->m->colMapa);
    free(g->m);
//    free();
    printf("todo bien!");
}
void game_run(tGame *g) {
    while (g->is_running == VERDADERO) {
        crearConexion(g);
        g->is_running = FALSE;
    }
}
void game_update(tGame *g) {
    char tecla;
    tecla = getch(); // cambiar por eventos;
    moverjugador(&(g->colaMov), g->m->mat, tecla, g->p);
    moverfantasmas(g, &(g->colaMov));
    desencolarmovs(&(g->colaMov), g->m->mat, g->p);
    //destruir
}
void moverfantasmas(tGame *g, tCola *cola) {
    int i, j;
    ghost fant;
    for (i = 0; i < g->m->filMapa; i++) {
        for (j = 0; j < g->m->colMapa; j++) {
            if (g->m->mat[i][j] == FANTASMA) {
                fant.posx = i;
                fant.posy = j;
                ai(cola, g->m->mat, g->p, &fant);
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
void desencolarmovs(tCola *cola, char ** mat, player *p) {
    moves movi;
    while (colaVacia(cola)!=COLA_VACIA) {
        /* sacarDeCola rellena 'movimientos' */
        sacarDeCola(cola, &movi, sizeof(moves));
        if (movi.move == ARRIBA) {
            if (mat[movi.posx - 1][movi.posy] != FANTASMA && mat[movi.posx - 1][movi.posy] != '#')
                intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx - 1][movi.posy], sizeof(char));
            if (mat[movi.posx - 1][movi.posy] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA) {
                mat[movi.posx][movi.posy] = CAMINO;
                p->vidas--;
            }
        }
        if (movi.move == ABAJO) {
            if (mat[movi.posx + 1][movi.posy] != FANTASMA && mat[movi.posx + 1][movi.posy] != '#')
                intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx + 1][movi.posy], sizeof(char));
            if (mat[movi.posx + 1][movi.posy] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA) {
                mat[movi.posx][movi.posy] = CAMINO;
                p->vidas--;
            }
        }
        if (movi.move == IZQUIERDA) {
            if (mat[movi.posx][movi.posy - 1] != FANTASMA && mat[movi.posx][movi.posy - 1] != '#')
                intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx][movi.posy - 1], sizeof(char));
            if (mat[movi.posx][movi.posy - 1] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA) {
                mat[movi.posx][movi.posy] = CAMINO;
                p->vidas--;
            }
        }
        if (movi.move == DERECHA) {
            if (mat[movi.posx][movi.posy + 1] != FANTASMA && mat[movi.posx][movi.posy + 1] != '#')
                intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx][movi.posy + 1], sizeof(char));
            if (mat[movi.posx][movi.posy + 1] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA) {
                mat[movi.posx][movi.posy] = CAMINO;
                p->vidas--;
            }
        }
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
//        flush;
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
    while (g->is_running == VERDADERO && g->m->exit == FALSE) {
        mostrarMat(g->m->mat, g->m->filMapa, g->m->colMapa);
        game_update(g);
        checkend(g->m, g->p);
        system("cls");
    }
//    while (m->exit != VERDADERO) {
//        mostrarMat(m->mat, TAM_FIL, TAM_COL);
    //tecla= getch();
    //moverjugador(m->mat,tecla,p);
//        ai(colaMov, m->mat, p, f);
//        checkend(m, p);
//        system("cls");
//    }
//    eliminarMatriz(g->m->mat, g->m->filMapa, g->m->colMapa);
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
