#include <unistd.h>
#include "main.h"


int main() {
    int cont = 0;
    char tecla='w';
    char**mat;
    struct player p;
    mat = crearMatriz(21,21);
    if(!mat)
        return -1;
    llenarMat(mat,21,21);
    crearLaberinto(mat,21,21,&p);
    while(tecla != 'l')
    {
        mostrarMat(mat,21,21);
        //tecla= getch();
        //moverjugador(mat,tecla,&p);
        ai(mat,&p);
        system("cls");
    }
    eliminarMatriz(mat,21,21);
    return 0;
}
