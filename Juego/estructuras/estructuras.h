#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#define TODO_OK 0
#define COLA_LLENA 1
#define COLA_VACIA 2
#define PILA_LLENA 1
#define PILA_VACIA 2
#define SIN_MEM 3
#define MIN(X,Y) ((X)>(Y)?(X):(Y))

typedef struct sNodo
{
    void*dato;
    unsigned tamInfo;
    struct sNodo * sig;
}tNodo;
typedef struct
{
    char nombre[7];
    int puntaje;
} tRanking;

typedef struct
{

    unsigned clave;
    unsigned numRegistro;

} tClave;

typedef struct
{

    int id;
    char nombre[40];
    int total_puntos;
    int partidas_jugadas;

} tJugadorDatos;

typedef struct
{
    int id_partida;         // ID autoincremental
    int id_jugador;         // ID del jugador
    int puntaje;            // Puntos obtenidos
    int movimientos;        // Movimientos realizados
} tPartidaDatos;

typedef struct
{
    int clave;
    long pos;

} tIndice;


#endif // ESTRUCTURAS_H_INCLUDED
