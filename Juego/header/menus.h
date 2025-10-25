#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED
#include "game.h"
#define ANCHOCUADRADO 500
#define LARGOCUADRADO 250
#define LARGOTEXTO 50
void menu_pausa(tGame *g);
void cargarTexturasMenu(tGame *g,SDL_Rect botonP,SDL_Rect botonR,SDL_Rect botonS);
void menu_inicio(tGame* g);
void submenuranking(tGame *g);
int reiniciarJuego(tGame *g);
#endif // MENUS_H_INCLUDED
