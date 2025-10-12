#include "main.h"

int main() {
    tGame game;
    if (game_new(&game) == VERDADERO) {
        game_run(&game);
    }
    game_free(&game);
    return 0;
}
