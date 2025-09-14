#include <unistd.h>
#include "main.h"
#include "mapa.h"

int main() {
    int cont = 0;
    char tecla='w';
    char**mat;
    struct player p;
    struct mapaL m;
    m.mat = crearMatriz(21,21);
    if(!m.mat)
        return -1;
    llenarMat(m.mat,21,21);
    crearLaberinto(&m,21,21,&p);
    while(m.exit != true)
    {
        mostrarMat(m.mat,21,21);
        tecla= getch();
        moverjugador(m.mat,tecla,&p);
        //ai(m.mat,&p);
        checkend(&m,&p);
        system("cls");
    }
    eliminarMatriz(m.mat,21,21);
    return 0;
}
