#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include "main.h"
#include "estructuras/colaDin.h"
struct game {

    bool is_running;
    struct player *p;
    struct mapaL *m;
    struct fantasma *f;
    tCola *colaMov;
};
bool game_new(struct game **game);
void game_free(struct game **game);
void game_run(struct game *game);
void game_update(struct game *g);
#endif // GAME_H_INCLUDED
