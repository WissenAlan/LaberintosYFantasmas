#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED
#define VISITADO '.'
#define CAMINO '.'
#define ENTRADA 'E'
#define SALIDA 'S'

typedef struct {
    int fil;
    int col;
}Celdas;

void llenarMat(char**mat,int fil, int col);
void mostrarMat(char**mat,int fil, int col);
char**crearMatriz(int filas, int columnas);
void eliminarMatriz(char**mat,int filas,int columnas);
int crearLaberinto(char**mat, int fil, int col);
int buscarVecinos(char**mat,int fil, int col,Celdas*act,Celdas*vecinos);

#endif // FUNCIONES_H_INCLUDED
