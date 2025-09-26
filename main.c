#include <unistd.h>
#define TAM_FIL 23
#define TAM_COL 29
#include "main.h"
#include "mapa.h"

int main() {
    int cont = 0;
    char tecla='w';
    char**mat;
    struct player p;
    struct mapaL m;
    m.mat = crearMatriz(TAM_FIL,TAM_COL);
    if(!m.mat)
        return -1;
    llenarMat(m.mat,TAM_FIL,TAM_COL);
    crearLaberinto(&m,TAM_FIL,TAM_COL,&p);
//    while(m.exit != true)
//    {
        mostrarMat(m.mat,TAM_FIL,TAM_COL);
//        tecla= getch();
//        moverjugador(m.mat,tecla,&p);
//        //ai(m.mat,&p);
//        checkend(&m,&p);
//        system("cls");
//    }
    eliminarMatriz(m.mat,TAM_FIL,TAM_COL);
    return 0;
}
