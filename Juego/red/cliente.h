#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include <winsock2.h>
#include <stdio.h>

#define SERVER_IP "192.168.0.9"
#define PUERTO 5302
#define TAM_BUFFER 1024
#define TAM_NOMBRE 50
#define TODO_OK 0
#define MENSAJE_INVALIDO -1
#define ERROR_WINSOCK -2
#define ERROR_CONEXION -3

// Inicializa Winsock
int init_winsock();

// Conecta al servidor y devuelve el socket
SOCKET connect_to_server(const char *server_ip, int puerto);

// Envía una solicitud y recibe la respuesta
int send_request(SOCKET soc, const char *request, char *response);

// Cierra la conexión y limpia Winsock
void close_connection(SOCKET soc);

#endif // CLIENTE_H_INCLUDED
