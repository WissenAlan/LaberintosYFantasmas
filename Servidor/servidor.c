#include "../Juego/estructuras/Arbol.h"
#include "../Juego/estructuras/Arbol.c"

tArbol arbolJugadores;   // árbol de jugadores en memoria
int next_id = 1;

int init_winsock() {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET create_server_socket() {
    SOCKET soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc == INVALID_SOCKET)
        return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PUERTO);

    if (bind(soc, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        closesocket(soc);
        return INVALID_SOCKET;
    }

    if (listen(soc, 1) == SOCKET_ERROR) {
        closesocket(soc);
        return INVALID_SOCKET;
    }

    return soc;
}

// Reescribe el archivo índice completo en base a jugadores.dat
void generarIndice() {
    FILE *pfJug = fopen(ARCH_JUG, "rb");
    FILE *pfIdx = fopen(ARCH_IDX, "wb");
    if (!pfJug || !pfIdx) {
        if (pfJug) fclose(pfJug);
        if (pfIdx) fclose(pfIdx);
        return;
    }

    tJugadorDatos jug;
    tIndice idx;
    long pos = 0;

    while (fread(&jug, sizeof(tJugadorDatos), 1, pfJug) == 1) {
        idx.clave = jug.id;
        idx.pos = pos;
        fwrite(&idx, sizeof(tIndice), 1, pfIdx);
        pos += sizeof(tJugadorDatos);
    }

    fclose(pfJug);
    fclose(pfIdx);
}

// Busca jugador por nombre, devuelve posición si existe o -1 si no
long buscarJugadorPorNombre(const char *nombre, tJugadorDatos *outJugador) {
    FILE *pf = fopen(ARCH_JUG, "rb");
    if (!pf)
        return -1;

    tJugadorDatos j;
    long pos = 0;
    while (fread(&j, sizeof(tJugadorDatos), 1, pf) == 1) {
        if (strcmp(j.nombre, nombre) == 0) {
            if (outJugador) *outJugador = j;
            fclose(pf);
            return pos;
        }
        pos += sizeof(tJugadorDatos);
    }

    fclose(pf);
    return -1;
}


int process_request(const char *request, char *response) {
    char comando[TAM_BUFFER];
    strcpy(comando, request);

    char *cmd = strtok(comando, "|");
    char *nombre = strtok(NULL, "|");

    static int next_id = 1;
    FILE *pf;
    tJugadorDatos jugador;

    if (strcmp(cmd, "NOMBRE") == 0) {
        if (!nombre || strlen(nombre) == 0) {
            snprintf(response, TAM_BUFFER, "Nombre inválido");
            return 1;
        }

        long pos = buscarJugadorPorNombre(nombre, &jugador);
        pf = fopen(ARCH_JUG, "rb+");
        if (!pf)
            pf = fopen(ARCH_JUG, "wb+");

        if (pos >= 0) {
            snprintf(response, TAM_BUFFER, "Jugador existente: %s (ID %d)", jugador.nombre, jugador.id);
        } else {
            // crear nuevo jugador
            jugador.id = next_id++;
            strcpy(jugador.nombre, nombre);
            jugador.total_puntos = 0;
            jugador.partidas_jugadas = 0;
            fseek(pf, 0, SEEK_END);
            fwrite(&jugador, sizeof(tJugadorDatos), 1, pf);
            snprintf(response, TAM_BUFFER, "Jugador creado: %s (ID %d)", nombre, jugador.id);
        }

        fclose(pf);
        generarIndice(); // cada vez que se modifica, actualizamos índice
        return 1;
    }

    else if (strcmp(cmd, "RANKING") == 0) {
        FILE *pfIdx = fopen(ARCH_IDX, "rb");
        FILE *pfJug = fopen(ARCH_JUG, "rb");
        if (!pfIdx || !pfJug) {
            snprintf(response, TAM_BUFFER, "No hay jugadores registrados.");
            if (pfIdx) fclose(pfIdx);
            if (pfJug) fclose(pfJug);
            return 1;
        }

        strcpy(response, "Ranking (por ID):\n");
        tIndice idx;
        char temp[128];
        tJugadorDatos jug;

        while (fread(&idx, sizeof(tIndice), 1, pfIdx) == 1) {
            fseek(pfJug, idx.pos, SEEK_SET);
            fread(&jug, sizeof(tJugadorDatos), 1, pfJug);
            sprintf(temp, "ID %d) %s - Puntos: %d - Partidas: %d\n",
                    jug.id, jug.nombre, jug.total_puntos, jug.partidas_jugadas);
            strcat(response, temp);
        }

        fclose(pfIdx);
        fclose(pfJug);
        return 1;
    }

    else if (strcmp(cmd, "SALIR") == 0) {
        snprintf(response, TAM_BUFFER, "Desconectando del servidor...");
        return 0;
    }

    snprintf(response, TAM_BUFFER, "Comando invalido");
    return 1;
}

void run_server() {
    SOCKET server_socket, socCli;
    int tamCli;
    struct sockaddr_in dirCli;
    char buffer[TAM_BUFFER], response[TAM_BUFFER];
    int bytes_recib;

    if (init_winsock() != 0) {
        printf("Error al inicializar Winsock\n");
        return;
    }

    server_socket = create_server_socket();
    if (server_socket == INVALID_SOCKET) {
        printf("Error al crear socket del servidor\n");
        WSACleanup();
        return;
    }

    printf("Servidor escuchando en puerto %d...\n", PUERTO);
    tamCli = sizeof(dirCli);

    while ((socCli = accept(server_socket, (struct sockaddr*)&dirCli, &tamCli)) != INVALID_SOCKET) {
        printf("Cliente conectado.\n");

        while ((bytes_recib = recv(socCli, buffer, TAM_BUFFER - 1, 0)) > 0) {
            buffer[bytes_recib] = '\0';
            printf("Recibido: %s\n", buffer);

            int seguir = process_request(buffer, response);
            send(socCli, response, strlen(response), 0);

            if (!seguir) {
                printf("Cliente pidio salir.\n");
                break;
            }
        }

        printf("Cliente desconectado.\n");
        closesocket(socCli);
    }

    closesocket(server_socket);
    WSACleanup();
}

