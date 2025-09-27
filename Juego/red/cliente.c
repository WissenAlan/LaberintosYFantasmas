#include "cliente.h"

int init_winsock() {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}
SOCKET connect_to_server(const char *server_ip, int puerto) {
    SOCKET soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc == INVALID_SOCKET)
        return INVALID_SOCKET;
    struct sockaddr_in dirServer;
    dirServer.sin_family = AF_INET;
    dirServer.sin_port = htons(puerto);
    dirServer.sin_addr.s_addr = inet_addr(server_ip);
    if (connect(soc, (struct sockaddr *)&dirServer, sizeof(dirServer)) < 0) {
        closesocket(soc);
        return INVALID_SOCKET;
    }
    return soc;
}

int send_request(SOCKET soc, const char *request, char *response) {
    if (send(soc, request, strlen(request), 0) < 0)
        return MENSAJE_INVALIDO;
    int bytesRecib = recv(soc, response, TAM_BUFFER - 1, 0);
    if (bytesRecib <= 0)
        return MENSAJE_INVALIDO;
    response[bytesRecib] = '\0';
    return TODO_OK;
}

void close_connection(SOCKET soc) {
    closesocket(soc);
    WSACleanup();
}
