#include "main.h"


int main() {
    char**mat;
    mat = crearMatriz(21,21);
    if(!mat)
        return -1;
    llenarMat(mat,21,21);
    crearLaberinto(mat,21,21);
    mostrarMat(mat,21,21);
    eliminarMatriz(mat,21,21);
}
