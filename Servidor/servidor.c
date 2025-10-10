#include "servidor.h"
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
    if (bind(soc, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        closesocket(soc);
        return INVALID_SOCKET;
    }
    if (listen(soc, 1) == SOCKET_ERROR) {
        closesocket(soc);
        return INVALID_SOCKET;
    }
    return soc;
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
    socCli = accept(server_socket, (struct sockaddr *)&dirCli, &tamCli);
    if (socCli == INVALID_SOCKET) {
        printf("Error en accept()\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }
    printf("Cliente conectado.\n");
    while ((bytes_recib = recv(socCli, buffer, TAM_BUFFER - 1, 0)) > 0) {
        buffer[bytes_recib] = '\0';
        printf("Recibido: %s\n", buffer);
        process_request(buffer, response);
        send(socCli, response, strlen(response), 0);
        printf("Enviado: %s\n", response);
    }
    printf("Conexion cerrada.\n");
    closesocket(socCli);
    closesocket(server_socket);
    WSACleanup();
}

void process_request(const char* request, char* response) {
    char funcion[TAM_BUFFER];
    char *cmd, *nombre;
    FILE*jugadores, indiceBin, indiceIdx;
    strcpy(funcion, request);
    cmd = strtok(funcion, "|");
    if (strcmp(cmd, "NOMBRE") == 0) {
        strcpy(funcion, funcion + strlen(cmd) + 1);
        nombre = strtok(funcion, "");
        printf("Se ingreso el nombre: %s", nombre);
//        indiceIdx = fopen(ARCH_IDX,"");
        jugadores = fopen(ARCH_JUG, "ab");
        snprintf(response, TAM_BUFFER, "%s", funcion);
    } else
        snprintf(response, TAM_BUFFER, "nombre no valida");
}

