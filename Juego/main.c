#include <unistd.h>
#define TAM_FIL 23
#define TAM_COL 29
#include "main.h"
#include "mapa.h"
#include "red/cliente.h"
#include "estructuras/estructuras.h"

int main() {
    int op;
    printf("MENU:\n1) Conectar al servidor.\n2) Generar mapa.");
    scanf("%d", &op);
    switch (op) {
    case 1:
        crearConexion();
        break;
    case 2:
        crearMapa();
        break;
    default:
        break;
    }
    return 0;
}

int crearMapa() {
    int fil, col;
    int cont = 0;
    char tecla = 'w';
    char**mat;
    struct player p;
    struct mapaL m;
    fil = TAM_FIL % 2 == 0 ? TAM_FIL - 1 : TAM_FIL;
    col = TAM_COL % 2 == 0 ? TAM_COL - 1 : TAM_COL;
    m.mat = crearMatriz(TAM_FIL, TAM_COL);
    if (!m.mat)
        return SIN_MEM;
    llenarMat(m.mat, TAM_FIL, TAM_COL);
    crearLaberinto(&m, fil, col, &p);
//    while(m.exit != true)
//    {
    mostrarMat(m.mat, TAM_FIL, TAM_COL);
//        tecla= getch();
//        moverjugador(m.mat,tecla,&p);
//        //ai(m.mat,&p);
//        checkend(&m,&p);
//        system("cls");
//    }
    eliminarMatriz(m.mat, TAM_FIL, TAM_COL);
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
    while(1){
        printf("> ");
        if(fgets(buffer, TAM_BUFFER, stdin) == NULL)
            break;
        buffer[strcspn(buffer, "\n")] = '\0'; //quitar salto de linea
        if(strlen(buffer) == 0)
            continue;
        if(send_request(soc, buffer, response) == 0)
            printf("Respuesta: %s\n",response);
        else{
            printf("Error al enviar o recibir datos\n");
            break;
        }

    }
    close_connection(soc);
    return TODO_OK;
}
