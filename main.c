#include <unistd.h>
#include "main.h"


int main() {
    int cont = 0;
    char**mat;
    mat = crearMatriz(21,21);
    if(!mat)
        return -1;
    llenarMat(mat,21,21);
    crearLaberinto(mat,21,21);
    mostrarMat(mat,21,21);
    eliminarMatriz(mat,21,21);
//    while (1) {
//        printf("#E########\n#..#P....#\n#..####..#\n#F..#....#\n#V#......#\n#..#####.#\n#...#....#\n#...#F#..#\n#F.......#\n########S#");
//        sleep(1);
//        cont++;
//        system("cls");
//        printf("\n%d\n", cont);
//    }
    return 0;
}
