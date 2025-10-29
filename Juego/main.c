#include "header/main.h"

int main(int argc, char* argv[]) {

    tGame game;
    if (game_new(&game) == VERDADERO) {
        game_run(&game);
    }
    game_free(&game);
    return 0;
}
