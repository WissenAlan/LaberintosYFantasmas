#include "main.h"
#include "jugador.h"
#include "mapa.h"
#include "game.h"

int game_new(tGame *game) {
    // tCola Colamov;
    game->p = malloc(sizeof(player));
    if (!game->p) {
        printf("ERROR en malloc");
        return FALSE;
    }
    game->m = malloc(sizeof(tMapa));
    if (game->m == NULL) {
        printf("ERROR en malloc");
        free(game->p);
        return FALSE;
    }
    game->m->exit = FALSE;
    //leer el archivo de config en un futuro
    game->m->mat = crearMatriz(21, 21); // modificar por constantes dentro del game
//crearCola(&Colamov);
    //(*game)->colaMov=&Colamov;
    //aca agregamos la conecion con el servidor todo eso etc
    game->is_running = VERDADERO;
    return VERDADERO;
}
void game_free(tGame *game) {
    printf("todo bien!");
}
void game_run(tGame *g) {
    while (g->is_running) {
        //menu de inicio
        int op;
        printf("MENU:\n1) Conectar al servidor.\n2) Generar mapa.");
        scanf("%d", &op);
        switch (op) {
        case 1:
            crearConexion();
            break;
        case 2:
            crearMapa(g->p, g->f, g->m);
            break;
        default:
            break;
        }
        g->is_running = FALSE;
//      llenarMat(g->m->mat,21,21);
//      crearLaberinto(g->m,21,21,g->p);
//      //
//        while(g->is_running && !g->m->exit)
//        {
//            mostrarMat(g->m->mat,21,21);
//            game_update(g);
//            checkend(g->m,g->p);
//            system("cls");
//        }
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
void game_update(tGame *g) {
    tCola cola ;
    crearCola(&cola);
    char tecla;
    tecla = getch(); // cambiar por eventos;
    moverjugador(&cola, g->m->mat, tecla, g->p);
    moverfantasmas(g, &cola);
    desencolarmovs(&cola, g->m->mat, g->p);
}
void moverfantasmas(tGame *g, tCola *cola) {
    int i, j;
    ghost fant;
    for (i = 0; i < 21; i++) {
        for (j = 0; j < 21; j++) {
            if (g->m->mat[i][j] == 'F') {
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
    while (!colaVacia(cola)) {
        /* sacarDeCola rellena 'movimientos' */
        sacarDeCola(cola, &movi, sizeof(moves));
        if (movi.move == ARRIBA) {
            if (mat[movi.posx - 1][movi.posy] != FANTASMA && mat[movi.posx - 1][movi.posy] != '#')
                intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx - 1][movi.posy], sizeof(char));
            if (mat[movi.posx - 1][movi.posy] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA) {
                mat[movi.posx][movi.posy] = CAMINO;
                p->lifes--;
            }
        }
        if (movi.move == ABAJO) {
            if (mat[movi.posx + 1][movi.posy] != FANTASMA && mat[movi.posx + 1][movi.posy] != '#')
                intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx + 1][movi.posy], sizeof(char));
            if (mat[movi.posx + 1][movi.posy] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA) {
                mat[movi.posx][movi.posy] = CAMINO;
                p->lifes--;
            }
        }
        if (movi.move == IZQUIERDA) {
            if (mat[movi.posx][movi.posy - 1] != FANTASMA && mat[movi.posx][movi.posy - 1] != '#')
                intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx][movi.posy - 1], sizeof(char));
            if (mat[movi.posx][movi.posy - 1] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA) {
                mat[movi.posx][movi.posy] = CAMINO;
                p->lifes--;
            }
        }
        if (movi.move == DERECHA) {
            if (mat[movi.posx][movi.posy + 1] != FANTASMA && mat[movi.posx][movi.posy + 1] != '#')
                intercambiar(&mat[movi.posx][movi.posy], &mat[movi.posx][movi.posy + 1], sizeof(char));
            if (mat[movi.posx][movi.posy + 1] == JUGADOR && mat[movi.posx][movi.posy] == FANTASMA) {
                mat[movi.posx][movi.posy] = CAMINO;
                p->lifes--;
            }
        }
    }
}

int crearConexion() {
    if (init_winsock() != 0) {
        printf("Error con WINSOCK\n");
        return ERROR_WINSOCK;
    }
    SOCKET soc = connect_to_server(SERVER_IP, PUERTO);
    if (soc == INVALID_SOCKET) {
        printf("Error de conexion\n");
        WSACleanup();
        return ERROR_CONEXION;
    }
    printf("Conectado al servidor\n Puede dercirle HOLA.\n");
    char buffer[TAM_BUFFER], response[TAM_BUFFER];
    while (1) {
        printf("> ");
        if (fgets(buffer, TAM_BUFFER, stdin) == NULL)
            break;
        buffer[strcspn(buffer, "\n")] = '\0'; //quitar salto de linea
        if (strlen(buffer) == 0)
            continue;
        if (send_request(soc, buffer, response) == 0)
            printf("Respuesta: %s\n", response);
        else {
            printf("Error al enviar o recibir datos\n");
            break;
        }
    }
    close_connection(soc);
    return TODO_OK;
}
