#include "../Juego/estructuras/Arbol.h"
#include "../Juego/estructuras/Arbol.c"
#include "../Juego/estructuras/colaDin.h"
#include "../Juego/estructuras/colaDin.c"
#include "../Juego/estructuras/Lista.c"
#include "../Juego/estructuras/Lista.h"
#include "servidor.h"

int init_winsock()
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET create_server_socket()
{
    SOCKET soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc == INVALID_SOCKET)
        return INVALID_SOCKET;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PUERTO);
    if (bind(soc, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        closesocket(soc);
        return INVALID_SOCKET;
    }
    if (listen(soc, 1) == SOCKET_ERROR)
    {
        closesocket(soc);
        return INVALID_SOCKET;
    }
    return soc;
}

// Reescribe el archivo índice completo en base a jugadores.dat
void generarIndice()
{
    FILE *pfJug = fopen(ARCH_JUG, "rb");
    FILE *pfIdx = fopen(ARCH_IDX, "wb");
    if (!pfIdx)
    {
        if (pfJug) fclose(pfJug);
        return;
    }
    if (!pfJug)
    {
        // si no existe jugadores, simplemente cerramos idx y salimos (idx queda vacío)
        fclose(pfIdx);
        return;
    }
    tJugadorDatos jug;
    tIndice idx;
    long pos = 0;
    while (fread(&jug, sizeof(tJugadorDatos), 1, pfJug) == 1)
    {
        idx.clave = jug.id;
        idx.pos = pos;
        fwrite(&idx, sizeof(tIndice), 1, pfIdx);
        pos += sizeof(tJugadorDatos);
    }
    fclose(pfJug);
    fclose(pfIdx);
}

// Busca jugador por nombre, devuelve posición si existe o -1 si no
long buscarJugador(const char *nombre, tJugadorDatos *outJugador)
{
    FILE *pf = fopen(ARCH_JUG, "rb");
    if (!pf)
        return -1;
    tJugadorDatos j;
    long pos = 0;
    while (fread(&j, sizeof(tJugadorDatos), 1, pf) == 1)
    {
        if (strcmp(j.nombre, nombre) == 0)
        {
            if (outJugador) *outJugador = j;
            fclose(pf);
            return pos;
        }
        pos += sizeof(tJugadorDatos);
    }
    fclose(pf);
    return -1;
}
// calcula next_id leyendo el archivo (para evitar duplicados tras reinicio)
int inicializarNextIdDesdeArchivo()
{
    int next_id = 1;
    tJugadorDatos j;
    int max_id = 0;

    FILE *pf = fopen(ARCH_JUG, "rb");
    if (!pf)
    {
        next_id = 1;
        return next_id;
    }
    while (fread(&j, sizeof(tJugadorDatos), 1, pf) == 1)
    {
        if (j.id > max_id) max_id = j.id;
    }
    fclose(pf);
    return (max_id + 1);
}

int process_request(const char *request, char *response)
{
//    char nombre[1024];
    int i = 0, j;
    tLista lista;
    char comando[TAM_BUFFER];
    char nombre_clean[30];
    strncpy(comando, request, sizeof(comando) - 1);
    comando[sizeof(comando) - 1] = '\0';
    // separar comando
    char *cmd = strtok(comando, "|");

    FILE *pf;
    tJugadorDatos jugador;
    if (cmd == NULL)
    {
        snprintf(response, TAM_BUFFER, "Comando invalido");
        return 1;
    }
    /// ---- NUEVAS PETICIONES DESDE EL CLIENTE SDL ----
    else if (cmd && strcmp(cmd, "INICIO_PARTIDA") == 0)
    {
        printf("[SERVIDOR] El cliente inició una partida.\n");
        snprintf(response, TAM_BUFFER, "Inicio de partida recibido correctamente");
    }
    else if (strcmp(cmd, "MOSTRAR_RANKING") == 0)
    {
        printf("[SERVIDOR] El cliente solicitó el ranking.\n");
        snprintf(response, TAM_BUFFER, "Solicitud de ranking recibida");
    }
    else if (strcmp(cmd, "SALIR_JUEGO") == 0)
    {
        printf("[SERVIDOR] El cliente salió del juego.\n");
        snprintf(response, TAM_BUFFER, "Cliente desconectado del servidor");
        return DESCONECTADO; // fuerza cierre del cliente en el servidor
    }
    /// ---- RESTO DE COMANDOS EXISTENTES ----
    else if (strcmp(cmd, "NOMBRE") == 0)
    {
        cmd = strtok(NULL, "|");  // continuar
//        strncpy(nombre_clean, cmd, sizeof(nombre_clean) - 1);
//        nombre_clean[sizeof(nombre_clean) - 1] = '\0';
//        nombre_clean[strcspn(nombre_clean, "\r\n")] = '\0';
        printf("Procesando Nombre: %s", cmd);
        long pos = buscarJugador(cmd, &jugador);
        pf = fopen(ARCH_JUG, "rb+");
        if (!pf)
            pf = fopen(ARCH_JUG, "wb+");
        if (!pf)
        {
            snprintf(response, TAM_BUFFER, "Error al abrir archivo de jugadores");
            return CONECTADO;
        }
        if (pos >= 0)
            snprintf(response, TAM_BUFFER, "Jugador existente: %s (ID %d)", jugador.nombre, jugador.id);
        else
        {
            jugador.id = inicializarNextIdDesdeArchivo();
            strncpy(jugador.nombre, cmd, sizeof(jugador.nombre) - 1);
            jugador.nombre[sizeof(jugador.nombre) - 1] = '\0';
            cmd = strtok(NULL,"|");
            jugador.total_puntos = atoi(cmd);
            jugador.partidas_jugadas = 0;
            fseek(pf, 0, SEEK_END);
            fwrite(&jugador, sizeof(tJugadorDatos), 1, pf);
            snprintf(response, TAM_BUFFER, "Jugador creado: %s (ID %d)", jugador.nombre, jugador.id);
        }
        fclose(pf);
        generarIndice();
    }
    else if (strcmp(cmd, "RANKING") == 0)
    {
        printf("[SERVIDOR] Petición de ranking (Top 5 por puntos).\n");
        pf = fopen(ARCH_JUG, "rb");
        if (!pf)
        {
            strcpy(response, ":");
            return CONECTADO;
        }
        crearLista(&lista);
        tJugadorDatos jug;
        while (fread(&jug, sizeof(tJugadorDatos), 1, pf)){
            insertarOrdenado(&lista, &jug, sizeof(tJugadorDatos), cmp_jugador_puntos_desc);
        }
        fclose(pf);
//        snprintf(response, TAM_BUFFER, "🏆 Top 5 Jugadores:\n");
        // 1.NOMBRE-1000|
        // RANKING|nombre1-1000|nombre2-500
        while (!listaVacia(&lista) && i < 5)
        {
            sacarPrimero(&lista, &jug, sizeof(tJugadorDatos));
            sprintf(response, "%s%s+%d|", response, jug.nombre, jug.total_puntos);
            i++;
        }
//        printf("\nRESPONSE: %s\n",response);
        vaciarLista(&lista);
        char vacios[20] = "";
        for(j = i; j < 5; j++)
            strcat(vacios, "-+-|");
        strcat(response, vacios);
    }
    else if (strcmp(cmd, "SALIR") == 0)
    {
        printf("[SERVIDOR] Cliente pidió salir.\n");
        snprintf(response, TAM_BUFFER, "Desconectando del servidor...");
        return DESCONECTADO;
    }else
        snprintf(response, TAM_BUFFER, "Comando invalido");

    response = NULL;
    return CONECTADO;
}

void run_server()
{
    char buffer[TAM_BUFFER];
    char response[TAM_BUFFER] = "";
    char peticion[TAM_BUFFER];
    int bytes_received;
    tCola cola;
    struct sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);
    int err;
    int cliente_activo = 1;
    if (init_winsock() != 0)
    {
        printf("Error al inicializar Winsock\n");
        return;
    }
    SOCKET server_socket = create_server_socket();
    if (server_socket == INVALID_SOCKET)
    {
        printf("Error al crear socket del servidor\n");
        WSACleanup();
        return;
    }
    printf("Servidor escuchando en puerto %d...\n", PUERTO);
    SOCKET client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == INVALID_SOCKET)
    {
        printf("Error en accept()\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }
    printf("Cliente conectado desde %s:%d\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    // Configuramos socket en modo no bloqueante
    u_long mode = 1;
    ioctlsocket(client_socket, FIONBIO, &mode);
    crearCola(&cola);
    while (cliente_activo)
    {
        buffer[0] = '\0';
        // Intentamos recibir datos sin bloquear
        bytes_received = recv(client_socket, buffer, TAM_BUFFER - 1, 0);
        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';
            printf("[RX] Recibido: %s\n", buffer);
            // Encolamos solo lo recibido + terminador
            colaInsertar(&cola, buffer, bytes_received + 1);
            memset(buffer, 0, TAM_BUFFER);
        }
        else if (bytes_received == 0)
        {
            printf("Cliente desconectado.\n");
            cliente_activo = 0;
        }
        else
        {
            err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK)
            {
                printf("Error en recv(): %d\n", err);
                cliente_activo = 0;
            }
            // Si es WSAEWOULDBLOCK, no hay datos, seguimos con la cola
        }
        // Procesamos la cola
        while (!colaVacia(&cola))
        {
            buffer[0] = '\0';
            response[0] = '\0';
            memset(peticion, 0, TAM_BUFFER);
            sacarDeCola(&cola, peticion, TAM_BUFFER);
            printf("[PROC] Procesando: %s\n", peticion);
            // Limpieza extra: eliminamos posibles retornos de carro o saltos de línea
            for (int i = 0; i < (int)strlen(peticion); i++)
            {
                if (peticion[i] == '\n' || peticion[i] == '\r')
                    peticion[i] = '\0';
            }
            int seguir = process_request(peticion, response);
            send(client_socket, response, (int)strlen(response) + 1, 0);
            printf("[TX] Enviado: %s\n", response);
            if (!seguir)
            {
                printf("[SERVIDOR] Cliente pidió salir.\n");
                cliente_activo = 0;
                break; // Salimos del while de la cola
            }
        }
        Sleep(50);
    }
    vaciarCola(&cola);
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
}

