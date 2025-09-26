#include <unistd.h>
#define TAM_FIL 23
#define TAM_COL 29
#include "main.h"
#include "mapa.h"

int main() {
    int fil, col;
    int cont = 0;
    char tecla='w';
    char**mat;
    struct player p;
    struct mapaL m;
    fil = TAM_FIL % 2 == 0 ? TAM_FIL -1 : TAM_FIL;
    col = TAM_COL % 2 == 0 ? TAM_COL -1 : TAM_COL;
    m.mat = crearMatriz(TAM_FIL,TAM_COL);
    if(!m.mat)
        return -1;
    llenarMat(m.mat,TAM_FIL,TAM_COL);
    crearLaberinto(&m,fil,col,&p);
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
