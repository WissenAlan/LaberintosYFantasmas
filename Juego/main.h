#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include "mapa.h"
#include "jugador.h"
#include "estructuras/colaDin.h"
#define ARRIBA 1
#define ABAJO 2
#define IZQUIERDA 3
#define DERECHA 4
//aca no van funciones sino todas las macros osea los #define

int crearMapa();
int crearConexion();

#endif // MAIN_H_INCLUDED
