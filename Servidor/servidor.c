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

void process_request(const char* request, char* response) {
    char operacion[16], text[TAM_BUFFER];
    sscanf(request, "%15s %[^\n]", operacion, text);
    if (strcmp(operacion, "HOLA") == 0) {
        printf("HOLA AMIGO");
        snprintf(response, TAM_BUFFER, "%s", text);
    } else
        snprintf(response, TAM_BUFFER, "Operacion no valida");
}
void run_server() {
    if (init_winsock() != 0) {
        printf("Error al inicializar Winsock\n");
        return;
    }
    SOCKET server_socket = create_server_socket();
    if (server_socket == INVALID_SOCKET) {
        printf("Error al crear socket del servidor\n");
        WSACleanup();
        return;
    }
    printf("Servidor escuchando en puerto %d...\n", PUERTO);
    struct sockaddr_in dirCli;
    int tamCli = sizeof(dirCli);
    SOCKET socCli = accept(server_socket, (struct sockaddr *)&dirCli, &tamCli);
    if (socCli == INVALID_SOCKET) {
        printf("Error en accept()\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }
    printf("Cliente conectado.\n");
    char buffer[TAM_BUFFER], response[TAM_BUFFER];
    int bytes_recib;
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


