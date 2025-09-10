#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../estructuras/pila.h"


int main() {
    int cont = 0;
    while (1) {
        printf("#E########\n#..#P....#\n#..####..#\n#F..#....#\n#V#......#\n#..#####.#\n#...#....#\n#...#F#..#\n#F.......#\n########S#");
        sleep(1);
        cont++;
        system("cls");
        printf("\n%d\n", cont);
    }
    return 0;
}
