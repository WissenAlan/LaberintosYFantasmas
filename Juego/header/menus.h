#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED
#include "game.h"
#define ANCHOCUADRADO 500
#define LARGOCUADRADO 250
#define LARGOTEXTO 50
#define ANCHOPARAMETRO 100
#define ALTOPARAMETRO 30
#define LARGONOMBRERANK 130
#define ALTONOMBRERANK 30
#define ALTOPUNTOS 20
#define POSWTOP4 (WINDOW_WIDTH-1160)
#define POSWTOP2 (WINDOW_WIDTH-930)
#define POSWTOP1 (WINDOW_WIDTH-695)
#define POSWTOP3 (WINDOW_WIDTH-455)
#define POSWTOP5 (WINDOW_WIDTH-230)
#define NAMEHEIGHT (WINDOW_HEIGHT-280)
#define POINTSHEIGHT (WINDOW_HEIGHT-250)
void menu_pausa(tGame *g);
void cargarTexturasMenu(tGame *g,SDL_Rect botonP,SDL_Rect botonR,SDL_Rect botonS);
void menu_inicio(tGame* g);
void submenuranking(tGame *g);
int reiniciarJuego(tGame *g);
void menuIngresarNombre(tGame *g);
#endif // MENUS_H_INCLUDED
