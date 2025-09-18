#include <unistd.h>
#define TAM 11
#include "main.h"
#include "mapa.h"

int main() {
    int cont = 0;
    char tecla='w';
    char**mat;
    struct player p;
    struct mapaL m;
    m.mat = crearMatriz(TAM,TAM);
    if(!m.mat)
        return -1;
    llenarMat(m.mat,TAM,TAM);
    crearLaberinto(&m,TAM,TAM,&p);
    while(m.exit != true)
    {
        mostrarMat(m.mat,TAM,TAM);
        tecla= getch();
        moverjugador(m.mat,tecla,&p);
        //ai(m.mat,&p);
        checkend(&m,&p);
        system("cls");
    }
    eliminarMatriz(m.mat,TAM,TAM);
    return 0;
}
