#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED

#include <winsock2.h>
#include <stdio.h>
#define PUERTO 5302
#define TAM_BUFFER 1024
#define DESCONECTADO 0
#define CONECTADO 1
#define ARCH_JUG "jugadores.dat"
#define ARCH_IDX "jugadores.idx"
#define ARCH_PARTIDAS "partidas.dat"

int init_winsock();

SOCKET create_server_socket();

int process_request(const char* request, char* response);

void run_server();


int inicializarNextIdDesdeArchivo();


#endif // SERVIDOR_H_INCLUDED
