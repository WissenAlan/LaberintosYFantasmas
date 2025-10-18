#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED
#include "game.h"
void menu_pausa(tGame *g);
void game_drawlab(tGame *g);
void subMenuCustom(tGame* g);
void cargarTexturasMenu(tGame *g,SDL_Rect botonP,SDL_Rect botonC,SDL_Rect botonI,SDL_Rect botonS);
void menu_inicio(tGame* g);
void submenuguia(tGame *g);


#endif // MENUS_H_INCLUDED
